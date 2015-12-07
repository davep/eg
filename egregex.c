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
#include <ctype.h>
#ifdef DJGPP
#include <sys/types.h>
#endif
#include <regex.h>
#include <slang.h>
#include <stdlib.h>

#include "eg.h"

int RegExSearch( PNG ng, char *pszRegEx, int iStartLine, int iIgnoreCase )
{
    int        iFound = -1;
    int        iLine;
    int        iCompCode;
    regex_t    re;

    if ( ( iCompCode = regcomp( &re, pszRegEx, REG_EXTENDED |
                                ( iIgnoreCase ? REG_ICASE : 0 ) ) ) == 0 )
    {
        for ( iLine = iStartLine; iLine < ng->entry->iLines && iFound == -1;
              iLine++ )
        {
            char *p = FormatLine( ng->entry->lines[ iLine ], 0 );

            /* We right trim whitespace from the string so that any
               regular expression using '$' will work as the user
               probably expects it to work. */
            RightCompress( p );

            if ( regexec( &re, p, 0, NULL, 0 ) == 0 )
            {
                DisplayMessage( "Found", 0 );
                iFound = iLine;
            }

            free( p );
        }

        regfree( &re );
    }
    else
    {
        char *pszError = (char *) egmalloc( SLtt_Screen_Cols );

        regerror( iCompCode, &re, pszError, SLtt_Screen_Cols - 1 );

        ShowError( pszError );

        free( pszError );

        iFound = -2;
    }

    return( iFound );
}
