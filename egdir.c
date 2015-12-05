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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>

#include "eg.h"

/* Structure to hold an entry in the directory list. */
typedef struct structEGDIR
{
    char szTitle[ 50 ];
    char szPath[ _POSIX_PATH_MAX ];
} EGDIR, *PEGDIR;

/* Directory list. */
static PEGDIR *egDir   = NULL;

/* Number of entries in the list. */
static int    iEntries = 0;

/* Local support functions. */
static void MakeEgDir( void );
static void KillDir( void );
static int  CountGuidesInPath( void );
static int  CountGuidesInDir( char * );
static void LoadInGuideDir( void );
static int  LoadInGuidesFromDir( char *, int );
static void LoadInGuideDetails( char *, char *, int );
static int  DirMenu( void );
static int  DirSorter( const void *, const void * );
static void SaveEgDir( void );
static void RestoreEgDir( void );

/*
 */

int EGDir( int iRescan )
{
    int iNewGuide = 0;
    
    if ( iRescan || !iEntries )
    {
        if ( iRescan )
        {
            MakeEgDir();
        }
        else
        {
            RestoreEgDir();
            
            if ( !iEntries )
            {
                MakeEgDir();
            }
        }
        
        if ( !iEntries )
        {
            ShowError( "Can't locate any Norton Guides" );
        }
    }

    if ( iEntries )
    {
        int iGuide;

        if ( ( iGuide = DirMenu() ) > -1 )
        {
            if ( IsAnNG( egDir[ iGuide ]->szPath, 1 ) )
            {
                CurrentGuide( egDir[ iGuide ]->szPath );
                iNewGuide = 1;
            }
        }
    }
    
    return( iNewGuide );
}

/*
 */

static void MakeEgDir( void )
{
    if ( egDir )
    {
        KillDir();
    }

    DisplayMessage( "Scanning for Norton Guides...", 1 );
    
    if ( ( iEntries = CountGuidesInPath() ) )
    {
        LoadInGuideDir();
        SaveEgDir();
    }

    DisplayMessage( "", 0 );
}

/*
 */

static void KillDir( void )
{
    int i;

    for ( i = 0; i < iEntries; i++ )
    {
        free( egDir[ i ] );
    }

    free( egDir );
    
    egDir    = (PEGDIR *) NULL;
    iEntries = 0;
}

/*
 */

static int CountGuidesInPath( void )
{
    int iGuides = 0;

    if ( GuidePath( NULL ) )
    {
        char *pszPath = egmalloc( strlen( GuidePath( NULL ) + 1 ) );
        char *pszDir;

        strcpy( pszPath, GuidePath( NULL ) );

        pszDir = strtok( pszPath, PATHSEP );

        while ( pszDir )
        {
            iGuides += CountGuidesInDir( pszDir );
            pszDir = strtok( NULL, PATHSEP );
        }

        free( pszPath );
    }

    return( iGuides );
}

/*
 */

static int CountGuidesInDir( char *pszDir )
{
    DIR *dir    = opendir( pszDir );
    int iGuides = 0;

    if ( dir )
    {
        char          szFile[ _POSIX_PATH_MAX ];
        struct dirent *dire;

        while ( ( dire = readdir( dir ) ) )
        {
            sprintf( szFile, "%s" SLASHS "%s", pszDir, dire->d_name );

            if ( IsAnNG( szFile, 0 ) )
            {
                ++iGuides;
            }
        }
        
        closedir( dir );
    }

    return( iGuides );
}

/*
 */

static void LoadInGuideDir( void )
{
    egDir = egcalloc( iEntries, sizeof( PEGDIR ) );
    
    if ( GuidePath( NULL ) )
    {
        char *pszPath = egmalloc( strlen( GuidePath( NULL ) + 1 ) );
        char *pszDir;
        int  iGuide = 0;

        strcpy( pszPath, GuidePath( NULL ) );

        pszDir = strtok( pszPath, PATHSEP );

        while ( pszDir )
        {
            iGuide = LoadInGuidesFromDir( pszDir, iGuide );
            pszDir = strtok( NULL, PATHSEP );
        }

        free( pszPath );

        qsort( egDir, iEntries, sizeof( PEGDIR ), DirSorter );
    }
}

/*
 */

static int LoadInGuidesFromDir( char *pszDir, int iGuide )
{
    DIR *dir = opendir( pszDir );

    if ( dir )
    {
        char          szFile[ _POSIX_PATH_MAX ];
        struct dirent *dire;

        while ( ( dire = readdir( dir ) ) )
        {
            sprintf( szFile, "%s" SLASHS "%s", pszDir, dire->d_name );
            
            if ( IsAnNG( szFile, 0 ) )
            {
                LoadInGuideDetails( pszDir, dire->d_name, iGuide );
                ++iGuide;
            }
        }
        
        closedir( dir );
    }

    return( iGuide );
}

/*
 */

static void LoadInGuideDetails( char *pszPath, char *pszFile, int iGuide )
{
    FILE *f;
    
    egDir[ iGuide ] = (PEGDIR) egmalloc( sizeof( EGDIR ) );

    memset( egDir[ iGuide ], 0, sizeof( EGDIR ) );

    sprintf( egDir[ iGuide ]->szPath, "%s" SLASHS "%s", pszPath, pszFile );

    if ( ( f = fopen( egDir[ iGuide ]->szPath, "rb" ) ) != NULL )
    {
        fseek( f, 8, SEEK_SET );
        fread( egDir[ iGuide ]->szTitle, 40, 1, f );
        fclose( f );
    }
    else
    {
        strcpy( egDir[ iGuide ]->szTitle, "**Error** Can't get title" );
    }
}

/*
 */

static int DirSorter( const void *x, const void *y )
{
    PEGDIR a = (PEGDIR) *( (PEGDIR *) x );
    PEGDIR b = (PEGDIR) *( (PEGDIR *) y );
    
    return( strcasecmp( a->szTitle, b->szTitle ) );
}

/*
 */

static int DirMenu( void )
{
    int iTop       = DisplayTop() + 1;
    int iLeft      = ( DisplayWidth() / 2 ) - 21;
    int iHeight    = ( DisplayBtm() - 2 ) - ( DisplayTop() + 1 );
    int iWidth     = 42;
    int iChosen    = -1;
    int iSelecting = 1;
    int iTopVis    = 0;
    int iBtmVis    = ( iEntries > ( iHeight - 2 ) ? iHeight - 3 : iEntries - 1);
    int iCurrent   = 0;
    int iLast      = 0;
    int iDirty     = 1;
    int iKey;
    int i;

    SLsmg_set_color( iColourMenu );
    SLsmg_draw_box( iTop, iLeft, iHeight, iWidth );

    while ( iSelecting )
    {
        if ( iDirty )
        {
            SLsmg_set_color( iColourMenu );
            SLsmg_fill_region( iTop + 1, iLeft + 1, iHeight - 2, iWidth - 2, ' ' );
            
            for ( i = iTopVis; i <= iBtmVis; i++ )
            {
                SLsmg_gotorc( iTop + ( i - iTopVis ) + 1, iLeft + 1 );
                SLsmg_write_string( egDir[ i ]->szTitle );
            }

            iDirty = 0;
        }

        SLsmg_set_color( iColourMenu );
        SLsmg_fill_region( iTop + ( iLast - iTopVis ) + 1, iLeft + 1,
                           1, iWidth - 2, ' ' );
        SLsmg_gotorc( iTop + ( iLast - iTopVis ) + 1, iLeft + 1 );
        SLsmg_write_string( egDir[ iLast ]->szTitle );
        SLsmg_set_color( iColourBar );
        SLsmg_fill_region( iTop + ( iCurrent - iTopVis ) + 1, iLeft + 1, 1, iWidth - 2, ' ' );
        SLsmg_gotorc( iTop + ( iCurrent - iTopVis ) + 1, iLeft + 1 );
        SLsmg_write_string( egDir[ iCurrent ]->szTitle );

        SLsmg_gotorc( SLtt_Screen_Rows - 1, SLtt_Screen_Cols - 1 );
        
        SLsmg_refresh();

        iLast = iCurrent;
        iKey  = SLkp_getkey();

        switch ( iKey )
        {
            case 'q' :
#ifdef DJGPP
            case 27 :
#endif
                iChosen    = -1;
                iSelecting = 0;
                break;
                
            case '\r' :
                
                iChosen    = iCurrent;
                iSelecting = 0;
                break;
                
            case SL_KEY_UP :        /* Up one line */
            case 'K' :
            case 'k' :
            case SL_KEY_LEFT :
            case 'H' :
            case 'h' :
                
                if ( iCurrent )
                {
                    if ( ( --iCurrent ) < iTopVis )
                    {
                        --iTopVis;
                        --iBtmVis;
                        iDirty = 1;
                    }
                }

                break;

            case SL_KEY_DOWN :      /* Down one line */
            case 'J' :
            case 'j' :
            case SL_KEY_RIGHT :  
            case 'L' :
            case 'l' :
                
                if ( iCurrent < ( iEntries - 1 ) )
                {
                    if ( ( ++iCurrent ) > iBtmVis )
                    {
                        ++iTopVis;
                        ++iBtmVis;
                        iDirty = 1;
                    }
                }
                
                break;
                
            case SL_KEY_HOME :      /* Top of menu */
            case '<' :
                
                iCurrent = iLast = 0;
                iTopVis  = 0;
                iBtmVis  = ( iEntries > ( iHeight - 2 ) ? iHeight - 3 : iEntries - 1);
                iDirty   = 1;
                break;
                
            case SL_KEY_END :       /* Bottom of menu */
            case '>' :
                
                iCurrent = iLast = ( iEntries - 1 );
                iBtmVis  = iEntries - 1;
                iTopVis  = ( iEntries > ( iHeight - 2 ) ? iBtmVis - ( iHeight - 3 ) : 0 );
                iDirty   = 1;
                break;
                
            case SL_KEY_PPAGE :     /* Page up */
            case 0x02 :
            case 'B' :
            case 'b' :
                
                if ( iCurrent )
                {
                    if ( iEntries > ( iHeight - 2 ) )
                    {
                        iCurrent -= ( iHeight - 3 );
                        
                        if ( iCurrent < 0 )
                        {
                            iTopVis = iCurrent = 0;
                            iBtmVis = iHeight - 3;
                        }
                        else
                        {
                            iTopVis = iCurrent;
                            iBtmVis = iTopVis + ( iHeight - 3 );
                        }
                    }
                    else
                    {
                        iCurrent = iLast = 0;
                    }
                    
                    iDirty = 1;
                }
                
                break;
                
            case SL_KEY_NPAGE :     /* Page down (bottom of menu) */
            case 0x06 :
            case 'F' :
            case 'f' :
                
                if ( iCurrent < ( iEntries - 1 ) )
                {
                    if ( iEntries > ( iHeight - 2 ) )
                    {
                        iCurrent += ( iHeight - 3 );
                        
                        if ( iCurrent > ( iEntries - 1 ) )
                        {
                            iBtmVis  = iCurrent = ( iEntries - 1 );
                            iTopVis  = iBtmVis - ( iHeight - 3 );
                        }
                        else
                        {
                            iBtmVis = iCurrent;
                            iTopVis = iBtmVis - ( iHeight - 3 );
                        }
                    }
                    else
                    {
                        iCurrent = iLast = ( iEntries - 1 );
                    }
                    
                    iDirty = 1;
                }
                
                break;
        }

    }

    return( iChosen );
}

/*
 */

static void SaveEgDir( void )
{
    char szDirFile[ _POSIX_PATH_MAX ];

    if ( MakeHomeDotFile( "egdir", szDirFile ) )
    {
        FILE *f = fopen( szDirFile, "w+" );

        if ( f != NULL )
        {
            int i;

            fprintf( f, "%d\n", iEntries );
            
            for ( i = 0; i < iEntries; i++ )
            {
                fprintf( f, "%s\n%s\n", egDir[ i ]->szPath, egDir[ i ]->szTitle );
            }
            
            fclose( f );
        }
    }
}

/*
 */

static void RestoreEgDir( void )
{
    char szDirFile[ _POSIX_PATH_MAX ];
    
    if ( egDir )
    {
        KillDir();
    }

    if ( MakeHomeDotFile( "egdir", szDirFile ) )
    {
        FILE *f;
        
        if ( ( f = fopen( szDirFile, "r" ) ) != NULL )
        {
            char szInFile[ 10 ];

            if ( fgets( szInFile, sizeof( szInFile ), f ) )
            {
                int iInFile = atoi( szInFile );
                
                if ( iInFile > 0 )
                {
                    int i;
                    
                    iEntries = iInFile;
                    
                    egDir = egcalloc( iEntries, sizeof( PEGDIR ) );
                    
                    for ( i = 0; i < iEntries; i++ )
                    {
                        egDir[ i ] = (PEGDIR) egmalloc( sizeof( EGDIR ) );
                        
                        fgets( egDir[ i ]->szPath, _POSIX_PATH_MAX, f );
                        fgets( egDir[ i ]->szTitle, 50, f );
                        
                        egDir[ i ]->szPath[ strlen( egDir[ i ]->szPath ) - 1 ]   = 0;
                        egDir[ i ]->szTitle[ strlen( egDir[ i ]->szTitle ) - 1 ] = 0;
                    }
                }
            }
            
            fclose( f );
        }
    }
}
