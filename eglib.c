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

#include <pwd.h>
#include <stdio.h>
#include <slang.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fnmatch.h>
#include <sys/stat.h>
#ifdef DJGPP
#include <fcntl.h>
#endif

#include "eg.h"
#include "cfgfile.h"

/*
 */

char *GuidePath( char *szNewPath )
{
    static char *szPath = NULL;

    if ( szNewPath )
    {
        if ( szPath )
        {
            free( szPath );
        }

        szPath = (char *) egmalloc( strlen( szNewPath ) + 1 );

        strcpy( szPath, szNewPath );
    }

    return( szPath );
}

/*
 */

char *CurrentGuide( char *szNewPath )
{
    static char szPath[ _POSIX_PATH_MAX ];

    if ( szNewPath )
    {
        strcpy( szPath, szNewPath );
        MakeAbsFilename( szPath );
    }

    return( szPath );
}

/*
 */

long LastGuidePosition( long lOffset )
{
    static int lLastOffset = 0L;

    if ( lOffset != 0L )
    {
        lLastOffset = lOffset;
    }

    return( lLastOffset );
}

/*
 */

void *egmalloc( size_t size )
{
    void *p = malloc( size );

    if ( !p )
    {
        egerror( "\nmalloc() error, can't allocate %d bytes"
                 " of memory.\n", size );
    }

    return( p );
}

/*
 */

void *egcalloc( size_t nmemb, size_t size )
{
    void *p = calloc( nmemb, size );

    if ( !p )
    {
        egerror( "\ncalloc() error, can't allocate %d*%d bytes"
                 " of memory.\n", nmemb, size );
    }

    return( p );
}

/*
 */

char *egstrdup( const char *s )
{
    char *p = strdup( s );

    if ( !p )
    {
        egerror( "\nstrdup() error, can't allocate %d bytes"
                 " of memory.\n", strlen( s ) );
    }

    return( p );
}

/*
 */

void egerror( char *format, ... )
{
    va_list args;

    KillScreen();

    va_start( args, format );

    vfprintf( stderr, format, args );

    va_end( args );

    exit( 1 );
}

/*
 */

void egperror( char *format, ... )
{
    char    szErr[ 256 ];
    va_list args;

    va_start( args, format );

    vsprintf( szErr, format, args );

    va_end( args );

    KillScreen();

    perror( szErr );
}

/*
 */

void GetStrZ( PNG ng, char *pszBuffer, int iMax )
{
    long lSavPos = ftell( ng->file );
    int  fEOS    = 0;
    int  i;

    fread( pszBuffer, iMax, 1, ng->file );

    for ( i = 0; i < iMax && !fEOS; i++, pszBuffer++ )
    {
        *pszBuffer = NG_DECRYPT( *pszBuffer );
        fEOS       = ( *pszBuffer == 0 );
    }

    fseek( ng->file, (long) lSavPos + i, SEEK_SET );
}

/*
 */

int ReadWord( PNG ng, int iDecrypt )
{
    unsigned char b1 = getc( ng->file );
    unsigned char b2 = getc( ng->file );

    if ( iDecrypt )
    {
        b1 = NG_DECRYPT( b1 );
        b2 = NG_DECRYPT( b2 );
    }

    return( ( b2 << 8 ) + b1 );
}

/*
 */

long ReadLong( PNG ng, int iDecrypt )
{
    int i1 = ReadWord( ng, iDecrypt );
    int i2 = ReadWord( ng, iDecrypt );

    return( ( i2 << 16 ) + i1 );
}

/*
 */

void LoadLastGuide( void )
{
    char szLastFile[ _POSIX_PATH_MAX ];

    if ( MakeHomeDotFile( "eglast", szLastFile ) )
    {
        if ( cfgReadFile( szLastFile ) )
        {
            CurrentGuide( cfgGetSetting( "LastGuide" ) );
            LastGuidePosition( atol( cfgGetSetting( "LastEntry" ) ) );
            cfgReset();
        }
        else
        {
            CurrentGuide( "/usr/share/norton-guides/eg.ng" );
        }
    }
}

/*
 */

void RememberLastGuide( PNG ng )
{
    char szLastFile[ PATH_MAX ];
    FILE *f;

    if ( MakeHomeDotFile( "eglast", szLastFile ) )
    {
        if ( ( f = fopen( szLastFile, "w" ) ) != NULL )
        {
            fprintf( f, "LastGuide=%s\n", CurrentGuide( NULL ) );
            fprintf( f, "LastEntry=%ld\n", ng->entry->lOffset );
            fclose( f );
        }
    }
}

/*
 */

char *MakeBufferSane( char *pszBuffer )
{
    char *p = pszBuffer;
    int  iChar;

    while ( *p )
    {
        iChar = (unsigned char) *p;

        switch ( iChar )
        {
            case 0xDB :         /* Block graphics */
            case 0xDC :
            case 0xDD :
            case 0xDE :
            case 0xDF :
            case 0xB0 :
            case 0xB1 :
            case 0xB2 :
            case 0x08 :
            case 0x0A :
                *p = '#';
                break;

            case 0xB3 :         /* Vertical graphics. */
            case 0xBA :
            case 0xB4 :         /* T leaning right */
            case 0xB5 :
            case 0xB6 :
            case 0xB9 :
            case 0xC3 :         /* T leaning left */
            case 0xC6 :
            case 0xC7 :
            case 0xCC :
                *p = '|';
                break;

            case 0xC4 :         /* Horizontal graphics. */
            case 0xCD :
            case 0xC2 :         /* T the right way up */
            case 0xCB :
            case 0xD1 :
            case 0xD2 :
            case 0xC1 :         /* T upside down */
            case 0xCA :
            case 0xCF :
            case 0xD0 :
                *p = '-';
                break;

            case 0xC5 :         /* Cross bar */
            case 0xCE :
            case 0xD7 :
            case 0xD8 :
            case 0xC9 :         /* Upper left corner */
            case 0xD5 :
            case 0xD6 :
            case 0xDA :
            case 0xB7 :         /* Upper right corner */
            case 0xB8 :
            case 0xBB :
            case 0xBF :
            case 0xC0 :         /* Lower left corner */
            case 0xC8 :
            case 0xD3 :
            case 0xD4 :
            case 0xBC :         /* Lower right corner */
            case 0xBD :
            case 0xBE :
            case 0xD9 :
                *p = '+';
                break;

            case 0x10 :         /* Right arrows */
            case 0x1A :
            case 0xAF :
            case 0xF2 :
                *p = '>';
                break;

            case 0x11 :         /* Left arrows */
            case 0x1B :
            case 0xAE :
            case 0xF3 :
                *p = '<';
                break;

            case 0x18 :         /* Up arrows */
            case 0x1E :
                *p = '^';
                break;

            case 0x19 :         /* Down arrows */
            case 0x1F :
                *p = 'v';
                break;

            case 0xFC :         /* Power of 'n' */
                *p = 'n';
                break;

            case 0xFD :         /* Squared */
                *p = '2';
                break;

            default :
                if ( iChar < ' ' || iChar > 0x7E )
                {
                    *p = '.';
                }
        }

        ++p;
    }

    return( pszBuffer );
}

/*
 */

void WriteStringSane( char *s )
{
    int iR = SLsmg_get_row();
    int iC = SLsmg_get_column();

    while ( *s )
    {
        SaneDisplay( iR, iC++, *s++ );
    }
}

/* I'm trying to do a couple of things here, and probably not very well.
   If we are compiling with DJGPP for Dos, pass just about everything
   thru to the display "as is". There are a number of characters that
   the slang display functions escape, so convert them.

   If we are running under Linux, do pretty much the same as above
   if we are running on the console (checking $TERM for "linux" was
   the easiest method I could think of for doing this). If $TERM
   isn't "linux" then do a "sane" translation on all control characters
   and everything that isn't "ASCII".

   This is code that WorksForMe(TM). If it doesn't work for you, let
   me know and lets fix it.
   */

void SaneDisplay( int r, int c, unsigned char bChar )
{
    SLsmg_gotorc( r, c );

#ifndef DJGPP
    if ( strcmp( getenv( "TERM" ), "linux" ) == 0 && !iForceSane )
    {
#endif
        switch ( bChar )
        {

            case 0x10 :         /* Right arrows */
            case 0x1A :
                SLsmg_write_char( '>' );
                break;

            case 0x11 :         /* Left arrows */
            case 0x1B :
                SLsmg_write_char( '<' );
                break;

            case 0x18 :         /* Up arrows */
            case 0x1E :
                SLsmg_write_char( '^' );
                break;

            case 0x19 :         /* Down arrows */
            case 0x1F :
                SLsmg_write_char( 'v' );
                break;

            default :
#ifdef DJGPP
                if ( bChar < ' ' || bChar == 0x7F )
#else
                    if ( bChar < ' ' || ( bChar > 0x7E && bChar < iEightBitStart ) )
#endif
                    {
                        SLsmg_write_char( '.' );
                    }
                    else
                    {
                        SLsmg_write_char( bChar );
                    }
        }
#ifndef DJGPP
    }
    else
    {
        switch ( bChar )
        {
            case 0xDB :         /* Block graphics */
            case 0xDC :
            case 0xDD :
            case 0xDE :
            case 0xDF :
            case 0xB0 :
            case 0xB1 :
            case 0xB2 :
            case 0x08 :
            case 0x0A :
                SLsmg_draw_object( r, c, SLSMG_CKBRD_CHAR );
                break;

            case 0xB3 :         /* Vertical graphics. */
            case 0xBA :
                SLsmg_draw_object( r, c, SLSMG_VLINE_CHAR );
                break;

            case 0xC4 :         /* Horizontal graphics. */
            case 0xCD :
                SLsmg_draw_object( r, c, SLSMG_HLINE_CHAR );
                break;

            case 0xB4 :         /* T leaning right */
            case 0xB5 :
            case 0xB6 :
            case 0xB9 :
                SLsmg_draw_object( r, c, SLSMG_RTEE_CHAR );
                break;

            case 0xC3 :         /* T leaning left */
            case 0xC6 :
            case 0xC7 :
            case 0xCC :
                SLsmg_draw_object( r, c, SLSMG_LTEE_CHAR );
                break;

            case 0xC2 :         /* T the right way up */
            case 0xCB :
            case 0xD1 :
            case 0xD2 :
                SLsmg_draw_object( r, c, SLSMG_UTEE_CHAR );
                break;

            case 0xC1 :         /* T upside down */
            case 0xCA :
            case 0xCF :
            case 0xD0 :
                SLsmg_draw_object( r, c, SLSMG_DTEE_CHAR );
                break;

            case 0xC5 :         /* Cross bar */
            case 0xCE :
            case 0xD7 :
            case 0xD8 :
                SLsmg_draw_object( r, c, SLSMG_PLUS_CHAR );
                break;

            case 0xC9 :         /* Upper left corner */
            case 0xD5 :
            case 0xD6 :
            case 0xDA :
                SLsmg_draw_object( r, c, SLSMG_ULCORN_CHAR );
                break;

            case 0xB7 :         /* Upper right corner */
            case 0xB8 :
            case 0xBB :
            case 0xBF :
                SLsmg_draw_object( r, c, SLSMG_URCORN_CHAR );
                break;

            case 0xC0 :         /* Lower left corner */
            case 0xC8 :
            case 0xD3 :
            case 0xD4 :
                SLsmg_draw_object( r, c, SLSMG_LLCORN_CHAR );
                break;

            case 0xBC :         /* Lower right corner */
            case 0xBD :
            case 0xBE :
            case 0xD9 :
                SLsmg_draw_object( r, c, SLSMG_LRCORN_CHAR );
                break;

            case 0x10 :         /* Right arrows */
            case 0x1A :
            case 0xAF :
            case 0xF2 :
                SLsmg_write_char( '>' );
                break;

            case 0x11 :         /* Left arrows */
            case 0x1B :
            case 0xAE :
            case 0xF3 :
                SLsmg_write_char( '<' );
                break;

            case 0x18 :         /* Up arrows */
            case 0x1E :
                SLsmg_write_char( '^' );
                break;

            case 0x19 :         /* Down arrows */
            case 0x1F :
                SLsmg_write_char( 'v' );
                break;

            case 0xFC :         /* Power of 'n' */
                SLsmg_write_char( 'n' );
                break;

            case 0xFD :         /* Squared */
                SLsmg_write_char( '2' );
                break;

            default :
                if ( bChar < ' ' || bChar > 0x7E )
                {
                    SLsmg_write_char( '.' );
                }
                else
                {
                    SLsmg_write_char( bChar );
                }
        }
    }
#endif
}

/*
 */

void RightCompress( char *p )
{
    char *pComp = p + ( strlen( p ) - 1 );

    while ( isspace( *pComp ) && pComp != p )
    {
        *pComp-- = 0;
    }
}

/*
 */

int MakeHomeDotFile( char *pszName, char *pszBuffer )
{
    char *pszHome = HomeDir();
    int iMadeIt   = 0;

    if ( pszHome && *pszHome )
    {
#ifndef DJGPP
        sprintf( pszBuffer, "%s/.%s", pszHome, pszName );
#else
        sprintf( pszBuffer, _use_lfn( NULL ) ? "%s/.%s" : "%s/_%s",
                 pszHome, pszName );
#endif
        iMadeIt = 1;
    }

    return( iMadeIt );
}

/*
 */

int IsAnNG( char *pszFile, int iReportErr )
{
    int    iIsAnNG = 0;
    FILE   *f;
    struct stat s;

    /* For some stupid reason Expert Help's INI file uses the same magic number as
       it's own compiled guides, so, filter it out as a special case <sigh>. */

    if ( fnmatch( "*eh.ini", pszFile, 0 ) == FNM_NOMATCH )
    {
        if ( stat( pszFile, &s ) == 0 )
        {
            if ( !S_ISDIR( s.st_mode ) )
            {
                if ( ( f = fopen( pszFile, "rb" ) ) != NULL )
                {
                    int b1 = fgetc( f );
                    int b2 = fgetc( f );

                    if ( b1 != EOF && b2 != EOF )
                    {
                        iIsAnNG = ( ( b1 == 'N' && b2 == 'G' ) ||
                                    ( b1 == 'E' && b2 == 'H' ) );
                    }

                    if ( !iIsAnNG && iReportErr )
                    {
                        ShowFileError( pszFile, "That doesn't look like a "
                                       "Norton Guide" );
                    }

                    fclose( f );
                }
                else if ( iReportErr )
                {
                    ShowFileError( pszFile, NULL );
                }
            }
            else if ( iReportErr )
            {
                ShowFileError( pszFile, "It's a directory!" );
            }
        }
        else
        {
            ShowFileError( pszFile, NULL );
        }
    }

    return( iIsAnNG );
}


/*
 */

char *HomeDir( void )
{
    static char szHomeDir[ _POSIX_PATH_MAX ] = "";

    if ( !*szHomeDir )
    {
        char *pszHome = getenv( "HOME" );

        if ( pszHome )
        {
            strcpy( szHomeDir, pszHome );
        }
        else
        {
            struct passwd *pw = getpwuid( getuid() );

            strcpy( szHomeDir, pw->pw_dir );
        }
    }

    return( szHomeDir );
}

/*
 */

void ExpandPath( char *pszPath )
{
    char *pszNewPath = egmalloc( _POSIX_PATH_MAX );

    *pszNewPath = 0;

    if ( *pszPath == '~' )
    {
        if ( *( pszPath + 1 ) == SLASHC || !*( pszPath + 1 ) )
        {
            sprintf( pszNewPath, "%s" SLASHS "%s", HomeDir(),
                     *( pszPath + 1 ) ? pszPath + 2 : "" );
        }
        else
        {
            struct passwd *pw;
            char *sop = strchr( pszPath + 1, SLASHC );

            if ( sop )
            {
                *sop = 0;
            }

            if ( ( pw = getpwnam( pszPath + 1 ) ) )
            {
                sprintf( pszNewPath, "%s" SLASHS "%s", pw->pw_dir,
                         sop ? sop + 1 : "" );
            }
        }
    }

    if ( *pszNewPath )
    {
        strcpy( pszPath, pszNewPath );
    }

    free( pszNewPath );
}

/* It's quick, it's nasty, it assumes that the parameter is of at least
 * _POSIX_PATH_MAX bytes in size. Ok, so it needs to be done correctly!
 */

void MakeAbsFilename( char *pszFile )
{
    char *pCWD       = getcwd( NULL, _POSIX_PATH_MAX );
    char *pPath      = egstrdup( pszFile );
    char *pEndOfPath = strrchr( pPath, SLASHC );

    if ( !pEndOfPath )          /* The file is 'here' */
    {
        if ( getcwd( pszFile, _POSIX_PATH_MAX ) != NULL )
        {
            strcat( pszFile, SLASHS );
            strcat( pszFile, pPath );
        }
    }
    else
    {
        char *pFile = pEndOfPath + 1;

        *pEndOfPath = 0;

        if ( chdir( pPath ) != -1 )
        {
            if ( getcwd( pszFile, _POSIX_PATH_MAX ) != NULL )
            {
                strcat( pszFile, SLASHS );
                strcat( pszFile, pFile );
            }
        }
    }

    chdir( pCWD );

    free( pCWD );
    free( pPath );
}
