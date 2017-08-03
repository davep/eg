/*

     Expert Guide - A Text Mode Norton Guide Reader
     Copyright (C) 1997-2017 David A Pearson

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

#include <slang.h>
#include <stdio.h>
#include <stdlib.h>

#include "eg.h"

/*
 */

void DrawEntry( PNG ng, int iRefresh )
{
    int i;
    int iRow;

    for ( i = ng->entry->iVisTop, iRow = 0; i <= ng->entry->iVisBtm;
          i++, iRow++ )
    {
        ShowNormal( ng, i );
    }

    if ( !ng->entry->iLong )
    {
        ShowHigh( ng, ng->entry->iCurrent );
    }

    if ( ( DisplayTop() + iRow ) < DisplayBtm() )
    {
        SLsmg_set_color( iColourNorm );
        SLsmg_fill_region( DisplayTop() + iRow, DisplayLeft(), DisplayBtm() -
                           ( DisplayTop() + iRow ), DisplayWidth(), ' ' );
    }

    if ( iRefresh )
    {
        SLsmg_refresh();
    }
}

/*
 */

void ShowNormal( PNG ng, int iLine )
{
    char *pszLine;
    int  iAttr = iColourNorm;
    int  iWidth;
    int  iRow;
    int  iCol;
    char *p;

    if ( ng->entry->formatted[ iLine ] )
    {
        pszLine = ng->entry->formatted[ iLine ] ;
    }
    else
    {
        pszLine = FormatLine( ng->entry->lines[ iLine ], 1 );
        ng->entry->formatted[ iLine ] = pszLine;
    }

    SLsmg_gotorc( ( iRow = DisplayTop() + ( iLine - ng->entry->iVisTop ) ),
                  ( iCol = DisplayLeft() ) );
    SLsmg_set_color( iAttr );

    for ( iWidth = 0, p = pszLine; *p && iWidth < DisplayWidth();
          iWidth++, p += 2 )
    {
        if ( *( p + 1 ) != iAttr )
        {
            iAttr = *( p + 1 );
            SLsmg_set_color( iAttr );
        }

        SaneDisplay( iRow, iCol++, *p );
    }
}

/*
 */

void ShowHigh( PNG ng, int iLine )
{
    char *pszLine = FormatLine( ng->entry->lines[ iLine ], 0 );
    char *p;
    int  iWidth;
    int  iRow;
    int  iCol;

    SLsmg_gotorc( ( iRow = DisplayTop() + ( iLine - ng->entry->iVisTop ) ),
                  ( iCol = DisplayLeft() ) );
    SLsmg_set_color( iColourSele );

    for ( iWidth = 0, p = pszLine; *p && iWidth < DisplayWidth();
          iWidth++, p++ )
    {
        SaneDisplay( iRow, iCol++, *p );
    }

    free( pszLine );
}
