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
#include <stdlib.h>

#include "eg.h"

void PipeText( PNG ng, char *pszCmd )
{
    FILE *f = popen( pszCmd, "w" );

    if ( f != NULL )
    {
        int iLine;
        char *pszLine;

        for ( iLine = 0; iLine < ng->entry->iLines; iLine++ )
        {
            pszLine = FormatLine( ng->entry->lines[ iLine ], 0 );

#ifndef DJGPP
            MakeBufferSane( pszLine );
#endif
            fprintf( f, "%s\n", pszLine );

            free( pszLine );
        }

        pclose( f );
    }

    /* This bit of code needs improving, unless you can't work out why,
       in which case it doesn't. :-) */
    printf( "Press return to continue..." );
    fflush( stdout );
    fgetc( stdin );
    fflush( stdin );
}
