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
#include <stdlib.h>
#include <string.h>

#include "eg.h"

static void    ReadHeader( PNG );
static void    ReadMenus( PNG );
static PNGMENU ReadMenu( PNG );
static void    SkipSection( PNG );
static void    ReadEntryText( PNG, int );
static void    KillEntry( PNG );
static void    ReadShortEntry( PNG );
static void    ReadLongEntry( PNG );

/*
 */

PNG OpenGuide( void )
{
    PNG ng = NULL;

    if ( CurrentGuide( NULL ) )
    {
        FILE *f = fopen( CurrentGuide( NULL ), "rb" );

        if ( f != NULL )
        {
            ng = (PNG) egmalloc( sizeof( NG ) );

            memset( ng, 0, sizeof( NG ) );
            
            ng->file   = f;
            ng->header = (PNGHEADER) egmalloc( sizeof( NGHEADER ) );

            memset( ng->header, 0, sizeof( NGHEADER ) );

            ReadHeader( ng );

            if ( ng->header->iMagic == 0x474E ||  /* 'NG' */
                ng->header->iMagic == 0x4845 )    /* 'EH' */
            {
                ng->header->iMenuCount++;

                ng->lGuideStart = ftell( ng->file );
                
                ReadMenus( ng );
            }
            else
            {
                CloseGuide( ng );
                ng = (PNG) NULL;
                egerror( "%s doesn't look like a Norton Guide\n",
                         CurrentGuide( NULL ) );
            }
        }
        else
        {
            egperror( "Can't open '%s'", CurrentGuide( NULL ) );
        }
    }
    
    return( ng );
}

/*
 */

void CloseGuide( PNG ng )
{
    if ( ng )
    {
        int i;

        if ( ng->file )
        {
            fclose( ng->file );

            if ( ng->header )
            {
                if ( ng->menus )
                {
                    for ( i = 0; i < ng->header->iMenuCount; i++ )
                    {
                        if ( ng->menus[ i ]->iEntries )
                        {
                            free( ng->menus[ i ]->entries );
                        }
                        free( ng->menus[ i ] );
                    }
                    
                    free( ng->menus );
                }
            
                free( ng->header );
            }
        }

        KillEntry( ng );
        
        free( ng );
    }
}

/*
 */

void ReadMenuEntry( PNG ng, int iMenu, int iItem )
{
    ReadEntry( ng, ng->menus[ iMenu ]->entries[ iItem ].lOffset );
}

/*
 */

void ReadEntry( PNG ng, long lOffset )
{
    int iID;

    KillEntry( ng );

    ng->entry = (PNGENTRY) egmalloc( sizeof( NGENTRY ) );

    memset( ng->entry, 0, sizeof( NGENTRY ) );
    
    ng->entry->lAddress = lOffset;
    
    fseek( ng->file, lOffset, SEEK_SET );

    iID = ReadWord( ng, 1 );

    switch ( iID )
    {
        case 0 :
            ReadShortEntry( ng );
            break;
        case 1 :
            ReadLongEntry( ng );
            break;
        default :
            egerror( "ID #%d is not a long or short entry.\n", iID );
            break;
    }
}

/*
 */

void ReadFirstEntry( PNG ng )
{
    ReadEntry( ng, FindFirstEntry( ng ) );
}

/*
 */

long FindFirstEntry( PNG ng )
{
    long lOffset = 0L;
    int  iID;
    
    fseek( ng->file, ng->lGuideStart, SEEK_SET );

    do
    {
        iID = ReadWord( ng, 1 );

        switch ( iID )
        {
            case 0 :
            case 1 :
                fseek( ng->file, (long) ftell( ng->file ) - 2L, SEEK_SET );
                lOffset = ftell( ng->file );
                iID = 5;
                break;
                
            case 2 :
                SkipSection( ng );
                break;
                
            default :
                iID = 5;
        }
            
    } while ( iID != 5 );

    return( lOffset );
}

/*
 */

int IsNgEntry( PNG ng, long lOffset )
{
    long lSav = ftell( ng->file );
    int  iID  = -1;

    if ( lOffset > -1 )
    {
        fseek( ng->file, lOffset, SEEK_SET );

        iID = ReadWord( ng, 1 );

        fseek( ng->file, lSav, SEEK_SET );
    }

    return( iID == 0 || iID == 1 );
}

/*
 */

static void ReadShortEntry( PNG ng )
{
    int iItems;
    int i;
    
    ng->entry->lOffset = ftell( ng->file ) - 2L;

    (void) ReadWord( ng, 1 );   /* Don't know what this is. */

    iItems = ReadWord( ng, 1 );

    (void) ReadWord( ng, 1 );   /* Don't know what this is. */

    ng->entry->iParentLine = ReadWord( ng, 1 );
    ng->entry->lParent     = ReadLong( ng, 1 );

    fseek( ng->file, 12L, SEEK_CUR );

    ng->entry->offsets = (long *) egcalloc( iItems, sizeof( long ) );

    for ( i = 0; i < iItems; i++ )
    {
        ReadWord( ng, 1 );      /* Don't know what this is. */
        ng->entry->offsets[ i ] = ReadLong( ng, 1 );
    }

    ReadEntryText( ng, iItems );

    ng->entry->iVisTop  = 0;
    ng->entry->iVisBtm  = iItems >= DisplayRows() ? DisplayRows() - 1 : iItems - 1;
    ng->entry->iLong    = 0;
    ng->entry->iSeeAlso = 0;
    ng->entry->iCurrent = 0;
    ng->entry->iLast    = 0;
}

/*
 */

static void ReadLongEntry( PNG ng )
{
    int iItems;
    int iSeeAlsos;
    
    ng->entry->lOffset = ftell( ng->file ) - 2L;

    (void) ReadWord( ng, 1 );

    iItems    = ReadWord( ng, 1 );
    iSeeAlsos = ReadWord( ng, 1 );

    ng->entry->iParentLine = ReadWord( ng, 1 );
    ng->entry->lParent     = ReadLong( ng, 1 );

    fseek( ng->file, 4L, SEEK_CUR );

    ng->entry->lPrevious = ReadLong( ng, 1 );
    ng->entry->lNext     = ReadLong( ng, 1 );

    if ( ng->entry->lPrevious == -1L ) ng->entry->lPrevious = 0L;
    if ( ng->entry->lNext     == -1L ) ng->entry->lNext     = 0L;

    ReadEntryText( ng, iItems );

    if ( iSeeAlsos )
    {
        int iCount = ReadWord( ng, 1 );

        if ( iCount )
        {
            int i;

            ng->entry->pSeeAlso = (PNGMENU) egmalloc( sizeof( NGMENU ) );
            
            memset( ng->entry->pSeeAlso, 0, sizeof( NGMENU ) );
            
            ng->entry->pSeeAlso->iEntries = iCount + 1;
            
            ng->entry->pSeeAlso->entries = (PNGMENUENTRY)
                egcalloc( iCount + 1, sizeof( NGMENUENTRY ) );
            
            for ( i = 0; i < iCount; i++ )
            {
                ng->entry->pSeeAlso->entries[ i ].lOffset = ReadLong( ng, 1 );
            }
            
            for ( i = 0; i < iCount; i++ )
            {
                GetStrZ( ng, ng->entry->pSeeAlso->entries[ i ].szName,
                         sizeof( ng->entry->pSeeAlso->entries[ i ]. szName ) );
            }
        }
        else
        {
            iSeeAlsos = 0;
        }
    }
    
    ng->entry->iVisTop  = 0;
    ng->entry->iVisBtm  = iItems >= DisplayRows() ? DisplayRows() - 1 :
        iItems - 1;
    ng->entry->iLong    = 1;
    ng->entry->iSeeAlso = iSeeAlsos;
    ng->entry->iCurrent = 0;
    ng->entry->iLast    = 0;
}

/*
 */

static void ReadEntryText( PNG ng, int iLines )
{
#define _RET_BUFF 1024
    
    char *pszBuffer = (char *) egmalloc( _RET_BUFF );
    int  i;
    
    ng->entry->lines     = (char **) egcalloc( iLines, sizeof( char * ) );
    ng->entry->formatted = (char **) egcalloc( iLines, sizeof( char * ) );
    ng->entry->iLines    = iLines;

    memset( ng->entry->formatted, 0, sizeof( char * ) * iLines );

    for ( i = 0; i < iLines; i++ )
    {
        GetStrZ( ng, pszBuffer, _RET_BUFF );
        ng->entry->lines[ i ] = (char *) egmalloc( strlen( pszBuffer ) + 1 );
        strcpy( ng->entry->lines[ i ], pszBuffer );
    }

    free( pszBuffer );

#undef _RET_BUFF
}

/*
 */

static void KillEntry( PNG ng )
{
    if ( ng->entry )
    {
        int i;

        for ( i = 0; i < ng->entry->iLines; i++ )
        {
            free( ng->entry->lines[ i ] );
            if ( ng->entry->formatted[ i ] )
            {
                free( ng->entry->formatted[ i ] );
            }
        }

        if ( ng->entry->pSeeAlso )
        {
            if ( ng->entry->pSeeAlso->entries )
            {
                free( ng->entry->pSeeAlso->entries );
            }
            free( ng->entry->pSeeAlso );
        }
        
        free( ng->entry->lines );
        free( ng->entry );

        ng->entry = (PNGENTRY) NULL;
    }
}
/*
 */

static void ReadHeader( PNG ng )
{
    int i;

    /* Read the NG magic number. */
    
    ng->header->iMagic = ReadWord( ng, 0 );

    /* Skip two shorts, I don't know what they are for. */

    (void) ReadWord( ng, 0 );
    (void) ReadWord( ng, 0 );
    
    /* Read in the count of menus. */

    ng->header->iMenuCount = ReadWord( ng, 0 );

    /* Read in the title. */

    fread( ng->header->szTitle, 40, 1, ng->file );

    /* Read in the guide comments. */

    for ( i = 0; i < 5; i++ )
    {
        fread( ng->header->pszCredits[ i ], 66, 1, ng->file );
    }
}

/*
 */

static void ReadMenus( PNG ng )
{
    int iID;
    int i = 0;
    
    ng->menus = (PNGMENU *) egcalloc( ng->header->iMenuCount, sizeof( PNGMENU ) );

    ng->menus[ i ] = (PNGMENU) egmalloc( sizeof( NGMENU ) );
    ng->menus[ i ]->iEntries = 0;
    
    strcpy( ng->menus[ i ]->szName, "Expand" );

    ++i;
    
    do
    {
        iID = ReadWord( ng, 1 );

        switch ( iID )
        {
            case 0 :
            case 1 :
                SkipSection( ng );
                break;
            case 2 :
                ng->menus[ i++ ] = ReadMenu( ng );
                break;
            default :
                iID = 5;
        }
    } while ( iID != 5 && i < ng->header->iMenuCount );
}

/*
 */

static PNGMENU ReadMenu( PNG ng )
{
    int     iItems;
    PNGMENU menu = (PNGMENU) egmalloc( sizeof( NGMENU ) );
    int     i;
    long    l;
    
    (void) ReadWord( ng, 1 );

    iItems = ReadWord( ng, 1 );

    menu->entries  = (PNGMENUENTRY) egcalloc( iItems, sizeof( NGMENUENTRY ) );
    menu->iEntries = iItems;
    
    fseek( ng->file, 20L, SEEK_CUR );

    for ( i = 1; i < iItems; i++ )
    {
        menu->entries[ i - 1 ].lOffset = ReadLong( ng, 1 );
    }

    l = ftell( ng->file );

    l += ( iItems * 8 );

    fseek( ng->file, l, SEEK_SET );

    GetStrZ( ng, menu->szName, 40 );

    for ( i = 1; i < iItems; i++ )
    {
        GetStrZ( ng, menu->entries[ i - 1 ].szName, 50 );
    }

    getc( ng->file );
    
    return( (PNGMENU) menu );
}

/*
 */

static void SkipSection( PNG ng )
{
    int iLen = ReadWord( ng, 1 );

    fseek( ng->file, (long) 22L + iLen, SEEK_CUR );
}

