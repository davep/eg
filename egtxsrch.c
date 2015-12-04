/*

     Expert Guide - A Text Mode Norton Guide Reader
     Copyright (C) 1997,1998,1999,2000 David A Pearson
   
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
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "eg.h"

static void UpperCase( char * );

/*
 */

int SimpleTextSearch( PNG ng, char *pszTarget, int iStartLine )
{
    char *pszNeedle = egmalloc( strlen( pszTarget ) + 1 );
    int iFound      = -1;
    int iLine;

    strcpy( pszNeedle, pszTarget );
    UpperCase( pszNeedle );
    
    for ( iLine = iStartLine; iLine < ng->entry->iLines && iFound == -1;
          iLine++ )
    {
        char *p = FormatLine( ng->entry->lines[ iLine ], 0 );

        UpperCase( p );

        if ( strstr( p, pszNeedle ) != NULL )
        {
            DisplayMessage( "Found", 0 );
            iFound = iLine;
        }

        free( p );
    }

    free( pszNeedle );

    return( iFound );
}

/*
 */

static void UpperCase( char *p )
{
    while ( *p )
    {
        *p = toupper( *p );
        ++p;
    }
}
