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

#include <stdio.h>
#include <slang.h>

#include "eg.h"

void DisplayHelp( PNG ng )
{
    int iWidth   = 67;
    int iHeight  = 20;
    int iTop     = DisplayTop() + ( ( DisplayBtm() - DisplayTop() ) / 2 )
        - ( iHeight / 2 );
    int iLeft    = ( SLtt_Screen_Cols / 2 ) - ( iWidth  / 2 );
    int iGHlpTop = iTop + iHeight - 7;
    int i;

    SLsmg_set_color( iColourHelp );
    SLsmg_fill_region( iTop, iLeft, iHeight, iWidth, ' ' );
    SLsmg_draw_box( iTop, iLeft, iHeight, iWidth );
    SLsmg_gotorc( iGHlpTop, iLeft + 1 );
    SLsmg_draw_hline( iWidth - 2 );
    SLsmg_draw_object( iGHlpTop, iLeft, SLSMG_LTEE_CHAR );
    SLsmg_draw_object( iGHlpTop, iLeft + iWidth - 1, SLSMG_RTEE_CHAR );

    SLsmg_gotorc( iTop + 1, iLeft + 1 );
    SLsmg_write_string( "Expert Guide v" EG_VERSION );
    SLsmg_gotorc( iTop + 2, iLeft + 1 );
    SLsmg_write_string( "Copyright (C) 1997-2015 David A Pearson <davep@davep.org>" );

    SLsmg_gotorc( iTop + 3, iLeft + 1 );
    SLsmg_draw_hline( iWidth - 2 );
    SLsmg_draw_object( iTop + 3, iLeft, SLSMG_LTEE_CHAR );
    SLsmg_draw_object( iTop + 3, iLeft + iWidth - 1, SLSMG_RTEE_CHAR );

    SLsmg_gotorc( iTop + 4, iLeft + 1 );
    SLsmg_write_string( "<Space>   Case insensitive text search" );
    SLsmg_gotorc( iTop + 5, iLeft + 1 );
    SLsmg_write_string( "      /   Case insensitive regular expression "
                        "search" );
    SLsmg_gotorc( iTop + 6, iLeft + 1 );
    SLsmg_write_string( "      \\   Case sensitive regular expression "
                        "search" );
    SLsmg_gotorc( iTop + 7, iLeft + 1 );
    SLsmg_write_string( "      |   Pipe page via external utility" );
    SLsmg_gotorc( iTop + 8, iLeft + 1 );
    SLsmg_write_string( "    d D   Guide directory (d = display, D = "
                        "rescan & display)" );
    SLsmg_gotorc( iTop + 9, iLeft + 1 );
    SLsmg_write_string( "      r   Read in a new guide" );
    SLsmg_gotorc( iTop + 10, iLeft + 1 );
    SLsmg_write_string( "  w s S   Save page to file (w = source, s = ASCII,"
                        " S = raw)" );
    SLsmg_gotorc( iTop + 11, iLeft + 1 );
    SLsmg_write_string( "      v   Show eg version and guide filename" );
    SLsmg_gotorc( iTop + 12, iLeft + 1 );
    SLsmg_write_string( "  F10 0   Quick exit" );

    SLsmg_gotorc( iTop + 4, iLeft + 9 );
    SLsmg_draw_vline( iGHlpTop - ( iTop + 4 ) );
    SLsmg_draw_object( iTop + 3, iLeft + 9, SLSMG_UTEE_CHAR );
    SLsmg_draw_object( iGHlpTop, iLeft + 9, SLSMG_DTEE_CHAR );

    for ( i = 0; i < 5; i++ )
    {
        int  iCol = iLeft + 1;
        char *p   = ng->header->pszCredits[ i ];

        while ( *p )
        {
            SaneDisplay( iGHlpTop + 1 + i, iCol++, *p++ );
        }
    }

    SLsmg_gotorc( SLtt_Screen_Rows - 1, SLtt_Screen_Cols - 1 );

    SLsmg_refresh();

    SLkp_getkey();

    DrawBackground( ng, 0 );
    DrawEntry( ng, 0 );
}
