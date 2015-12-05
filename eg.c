/*

     Expert Guide - A Text Mode Norton Guide Reader
     Copyright (C) 1997-2015 David A Pearson
   
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the license, or 
     (at your option) any later version.
     
     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
     
     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <stdio.h>
#include <slang.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "eg.h"

#ifdef DEBUG
int egDebug    = 0;             /* -D */
#endif

#ifndef DJGPP
int iForceSane = 0;             /* -s */
#endif

static void ShowVersionInfo( void );
static void ShowHelp( void );

/*
 */

int main( int argc, char **argv )
{
    int iOk = 1;
    int iOpt;
    PNG ng;

    /* See if the guide path has been defined in the environment. */
    if ( getenv( EG_ENV_PATH ) )
    {
        GuidePath( getenv( EG_ENV_PATH ) );
    }
#ifndef DJGPP
    else
    {
        /* If there isn't a path setting in the environment use the
           following as the default. */
        GuidePath( "/usr/local/share/norton-guides:/usr/share/norton-guides" );
    }
#endif

    /* Read the config file. */
    ConfigureEG();

    /* Check the command line parameters. */
    while ( ( iOpt = getopt( argc, argv,
#ifdef DEBUG
                             "D"
#endif
                             "vh"
#ifndef DJGPP
                             "s"
#endif
                             "p:" ) ) != -1 )
    {
        switch ( iOpt )
        {
#ifdef DEBUG
            case 'D' :
                egDebug = 1;
                break;
#endif

            case 'v' :
                ShowVersionInfo();
                iOk = 0;
                break;

            case 'h' :
                ShowHelp();
                iOk = 0;
                break;
            
            case 'p' :
                GuidePath( optarg );
                break;
                
#ifndef DJGPP            
            case 's' :
                iForceSane = 1;
                break;
#endif
            
            case '?' :
                ShowHelp();
                iOk = 0;
                break;
            
        }
    }

    if ( iOk )
    {
        /* If a guide was passed on the command line, use it. */
        if ( optind < argc )
        {
            CurrentGuide( argv[ optind ] );
        }
        else
        {
            /* Otherwise, see if we can work out the last guide used. */
            LoadLastGuide();
        }

        InitSignals();
        InitScreen( 1 );

        if ( CurrentGuide( NULL ) )
        {
            int iFirstTime = 1;
            int iExit      = 0;

            while ( !iExit )
            {
                if ( ( ng = OpenGuide() ) != NULL )
                {
                    DrawBackground( ng, 0 );
                    ShowStdMsg( 0 );
                
                    iExit = Navigate( ng, iFirstTime );

                    RememberLastGuide( ng );
                    
                    CloseGuide( ng );

                    iFirstTime = 0;
                }
                else
                {
                    iExit = 1;
                }
            }
        }
        else
        {
            printf( "Nothing to read!\n" );
        }
        
        KillScreen();
    }

    return( iOk ? 0 : 1 );
}

/*
 */

static void ShowVersionInfo( void )
{
    struct utsname utsn;

    uname( &utsn );

    printf( "Expert Guide Version " EG_VERSION "\n\n"
            "\
     Expert Guide - A Text Mode Norton Guide Reader\
     Copyright (C) 1997-2015 David A Pearson\
\
     This program is free software; you can redistribute it and/or modify\
     it under the terms of the GNU General Public License as published by\
     the Free Software Foundation; either version 2 of the license, or\
     (at your option) any later version.\
\
     This program is distributed in the hope that it will be useful,\
     but WITHOUT ANY WARRANTY; without even the implied warranty of\
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\
     GNU General Public License for more details.\
\
     You should have received a copy of the GNU General Public License\
     along with this program; if not, write to the Free Software\
     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\
\n" );
    
    printf( "System details:\n"
            "\tSystem.......: %s %s\n"
            "\tGuide path...: %s\n",
            utsn.sysname, utsn.release,
            GuidePath( NULL ) );
}

/*
 */

static void ShowHelp( void )
{
    printf( "Expert Guide v" EG_VERSION "\n"
            "\n"
            "usage: eg [options] [<norton-guide>]\n"
            "\n"
#ifdef DEBUG
            "  -D          Write debug messages to stderr\n"
#endif
            "  -h          Usage information\n"
            "  -p <path>   Specify the path for finding Norton guides\n"
#ifndef DJGPP
            "  -s          Don't attempt to preserve IBM characters\n"
#endif
            "  -v          Display version information\n" );
}
