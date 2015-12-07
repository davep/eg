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
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>

#include "eg.h"

static int  DirHasSingleEntry( char *, char * );
static int  DirTidy( char *, char * );
static void CompressPath( char * );

/*
 */

int CompleteFilename( char *pszFile )
{
    char szWork[ _POSIX_PATH_MAX ];
    char szDir[ _POSIX_PATH_MAX ];
    char szFile[ _POSIX_PATH_MAX ];
    char szOrig[ _POSIX_PATH_MAX ];
    int  iCompleted = 0;
    char *p;

    strcpy( szWork, pszFile );
    strcpy( szOrig, pszFile );
    ExpandPath( szWork );

    if ( ( p = strrchr( szWork, SLASHC ) ) )
    {
        *p = 0;
        if ( p == szWork )
        {
            strcpy( szDir, SLASHS );
        }
        else
        {
            strcpy( szDir, szWork );
#ifdef DJGPP
            if ( strlen( szDir ) == 2 && szDir[ 1 ] == ':' )
            {
                strcat( szDir, SLASHS );
            }
#endif
        }
        strcpy( szFile, p + 1 );
    }
    else
    {
        strcpy( szDir, "." );
        strcpy( szFile, szWork );
    }

    if ( !( iCompleted = DirHasSingleEntry( szDir, szFile ) ) && *szFile )
    {
        DIR *dir;

        if ( ( dir = opendir( szDir ) ) )
        {
            char          szHit[ _POSIX_PATH_MAX ] = "";
            struct dirent *de;

            while ( ( de = readdir( dir ) ) )
            {
                if (
#ifdef DJGPP
                    strnicmp
#else
                    strncmp
#endif
                    ( szFile, de->d_name, strlen( szFile ) ) == 0 )
                {
                    if ( !*szHit )
                    {
                        strcpy( szHit, de->d_name );
                    }
                    else
                    {
                        int i;

                        for ( i = 0; szHit[ i ] && de->d_name[ i ]; i++ )
                        {
                            if ( szHit[ i ] != de->d_name[ i ] )
                            {
                                szHit[ i ] = 0;
                                break;
                            }
                        }
                    }

                    ++iCompleted;
                }
            }

            if ( *szHit )
            {
                strcpy( szFile, szHit );
            }

            closedir( dir );
        }
    }

    if ( iCompleted )
    {
        if ( iCompleted == 1 )
        {
            DirTidy( szDir, szFile );
        }

        sprintf( pszFile, "%s" SLASHS "%s", szDir, szFile );
    }

    CompressPath( pszFile );

    return( strcmp( szOrig, pszFile ) == 0 &&
            ( iCompleted > 1 || pszFile[ strlen( pszFile ) - 1 ] == SLASHC ) );
}

/*
 */

static int DirHasSingleEntry( char *pszDir, char *pszFile )
{
    int  iSingle = 0;

    if ( !*pszFile )
    {
        DIR *dir = opendir( pszDir );

        if ( dir )
        {
            char          szEntry[ _POSIX_PATH_MAX ];
            struct dirent *de;
            int           iEntries = 0;

            while ( ( de = readdir( dir ) ) && iEntries < 2 )
            {
                if ( strcmp( ".", de->d_name ) != 0 &&
                     strcmp( "..", de->d_name ) != 0 )
                {
                    strcpy( szEntry, de->d_name );
                    ++iEntries;
                }
            }

            if ( iEntries == 1 )
            {
                strcpy( pszFile, szEntry );
                iSingle = 1;
            }

            closedir( dir );
        }
    }

    return( iSingle );
}

/*
 */

int DirTidy( char *szDir, char *szFile )
{
    int iTidy = 0;

    if ( *szFile )
    {
        char        szTest[ _POSIX_PATH_MAX ];
        struct stat s;

        if ( strcmp( szDir, SLASHS ) == 0 )
        {
            sprintf( szTest, "%s%s", szDir, szFile );
        }
        else
        {
            sprintf( szTest, "%s" SLASHS "%s", szDir, szFile );
        }

        if ( stat( szTest, &s ) == 0 )
        {
            if ( S_ISDIR( s.st_mode ) )
            {
                strcpy( szDir, szTest );
                strcpy( szFile, "" );
                iTidy = 1;
            }
        }
    }

    return( iTidy );
}

/*
 */

void CompressPath( char *pszPath )
{
    while ( *pszPath )
    {
        if ( *pszPath == SLASHC && *( pszPath + 1 ) == SLASHC )
        {
            strcpy( pszPath, pszPath + 1 );
        }
        else
        {
            ++pszPath;
        }
    }
}
