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
#include <string.h>

#include "eg.h"

static void CalcWindowSize( PNG, int, PEGMENU );
static void CalcSAWindowSize( PNG, PEGMENU );
static int  CalcMenuLeftSide( PNG, int );
static void RefreshMenu( PEGMENU );
    
/*
 */

PEGMENU NewEgMenu( PNG ng, int iMenu )
{
    PEGMENU pegMenu = (PEGMENU) egcalloc( 1, sizeof( EGMENU ) );

    pegMenu->pngMenu = ng->menus[ iMenu ];

    CalcWindowSize( ng, iMenu, pegMenu );

    pegMenu->iCurrent = pegMenu->iPrevious = 0;
    
    return( pegMenu );
}

/*
 */

PEGMENU NewEgMenuSeeAlso( PNG ng )
{
    PEGMENU pegMenu = (PEGMENU) egcalloc( 1, sizeof( EGMENU ) );

    pegMenu->pngMenu = ng->entry->pSeeAlso;

    CalcSAWindowSize( ng, pegMenu );

    pegMenu->iCurrent = pegMenu->iPrevious = 0;
    
    return( pegMenu );
}

/*
 */

void KillEgMenu( PEGMENU pegMenu )
{
    free( pegMenu );
}

/*
 */

void DropMenu( PEGMENU pegMenu )
{
    int iOption;
    
    SLsmg_set_color( iColourMenu );
    SLsmg_draw_box( pegMenu->iTop, pegMenu->iLeft,
                    pegMenu->iHeight, pegMenu->iWidth );
    SLsmg_fill_region( pegMenu->iTop + 1, pegMenu->iLeft + 1,
                       pegMenu->iHeight - 2, pegMenu->iWidth - 2, ' ' );

    for ( iOption = 0; iOption < pegMenu->pngMenu->iEntries - 1; iOption++ )
    {
        SLsmg_gotorc( pegMenu->iTop + 1 + iOption, pegMenu->iLeft + 2 );
        WriteStringSane( pegMenu->pngMenu->entries[ iOption ].szName );
    }

    SLsmg_set_color( iColourBar );
    SLsmg_fill_region( pegMenu->iTop + pegMenu->iCurrent + 1,
                       pegMenu->iLeft + 1, 1, pegMenu->iWidth - 2, ' ' );
                       
    SLsmg_gotorc( pegMenu->iTop + pegMenu->iCurrent + 1, pegMenu->iLeft + 2 );
    WriteStringSane( pegMenu->pngMenu->entries[ pegMenu->iCurrent ].szName );
}

/*
 */

void MenuDown( PEGMENU pegMenu )
{
    pegMenu->iPrevious = pegMenu->iCurrent;

    if ( ++pegMenu->iCurrent == pegMenu->pngMenu->iEntries - 1 )
    {
        pegMenu->iCurrent = 0;
    }

    RefreshMenu( pegMenu );
}

/*
 */

void MenuUp( PEGMENU pegMenu )
{
    pegMenu->iPrevious = pegMenu->iCurrent;

    if ( --pegMenu->iCurrent < 0 )
    {
        pegMenu->iCurrent = pegMenu->pngMenu->iEntries - 2;
    }

    RefreshMenu( pegMenu );
}

/*
 */

void MenuHome( PEGMENU pegMenu )
{
    pegMenu->iPrevious = pegMenu->iCurrent;
    pegMenu->iCurrent  = 0;
    
    RefreshMenu( pegMenu );
}

/*
 */

void MenuEnd( PEGMENU pegMenu )
{
    pegMenu->iPrevious = pegMenu->iCurrent;
    pegMenu->iCurrent  = pegMenu->pngMenu->iEntries - 2;

    RefreshMenu( pegMenu );
}

/*
 */

static void CalcWindowSize( PNG ng, int iMenu, PEGMENU pegMenu )
{
    int iWidth = 0;
    int iThisWidth;
    int iItem;
    
    pegMenu->iTop    = 2;
    pegMenu->iLeft   = CalcMenuLeftSide( ng, iMenu );
    pegMenu->iHeight = ng->menus[ iMenu ]->iEntries + 1;

    for ( iItem = 0; iItem < ng->menus[ iMenu ]->iEntries; iItem++ )
    {
        iThisWidth = strlen( ng->menus[ iMenu ]->entries[ iItem ].szName );

        if ( iThisWidth > iWidth )
        {
            iWidth = iThisWidth;
        }
    }

    pegMenu->iWidth = iWidth + 4;
    
    if ( ( pegMenu->iLeft + pegMenu->iWidth ) > SLtt_Screen_Cols )
    {
        pegMenu->iLeft -= ( ( ( pegMenu->iLeft + pegMenu->iWidth ) + 1 ) -
                            SLtt_Screen_Cols );
    }
}

/*
 */

static void CalcSAWindowSize( PNG ng, PEGMENU pegMenu )
{
    int iWidth = 0;
    int iThisWidth;
    int iItem;
    
    pegMenu->iTop    = 2;
    pegMenu->iLeft   = 1;
    pegMenu->iHeight = ng->entry->pSeeAlso->iEntries + 1;

    for ( iItem = 0; iItem < ng->entry->pSeeAlso->iEntries; iItem++ )
    {
        iThisWidth = strlen( ng->entry->pSeeAlso->entries[ iItem ].szName );

        if ( iThisWidth > iWidth )
        {
            iWidth = iThisWidth;
        }
    }

    pegMenu->iWidth = iWidth + 4;
}

/*
 */

int CalcMenuLeftSide( PNG ng, int iMenu )
{
    int i;
    int iLeft = 1;

    for ( i = 0; i < iMenu; i++ )
    {
        iLeft += strlen( ng->menus[ i ]->szName ) + 2;
    }

    return( iLeft );
}

/*
 */

static void RefreshMenu( PEGMENU pegMenu )
{
    SLsmg_set_color( iColourMenu );
    SLsmg_fill_region( pegMenu->iTop + pegMenu->iPrevious + 1,
                       pegMenu->iLeft + 1, 1, pegMenu->iWidth - 2, ' ' );
    
    SLsmg_gotorc( pegMenu->iTop + pegMenu->iPrevious + 1, pegMenu->iLeft + 2 );
    WriteStringSane( pegMenu->pngMenu->entries[ pegMenu->iPrevious ].szName );

    SLsmg_set_color( iColourBar );
    SLsmg_fill_region( pegMenu->iTop + pegMenu->iCurrent + 1,
                       pegMenu->iLeft + 1, 1, pegMenu->iWidth - 2, ' ' );
    
    SLsmg_gotorc( pegMenu->iTop + pegMenu->iCurrent + 1, pegMenu->iLeft + 2 );
    WriteStringSane( pegMenu->pngMenu->entries[ pegMenu->iCurrent ].szName );
}


