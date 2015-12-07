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
#include <limits.h>

#include "eg.h"

static char *UnCompressSrc( char * );

/*
 */

void SaveToFile( PNG ng, char *pszFile, int iLevel )
{
    FILE *f = fopen( pszFile, "w" );

    if ( f != NULL )
    {
        int iLine;

        DisplayMessage( "Saving....", 1 );

        for ( iLine = 0; iLine < ng->entry->iLines; iLine++ )
        {
            if ( iLevel == EG_SAVE_RAW || iLevel == EG_SAVE_SANE )
            {
                char *pszLine = FormatLine( ng->entry->lines[ iLine ], 0 );

                if ( iLevel == EG_SAVE_SANE )
                {
                    MakeBufferSane( pszLine );
                }

                RightCompress( pszLine );

                fprintf( f, "%s\n", pszLine );

                free( pszLine );
            }
            else
            {
                char *tmp = UnCompressSrc( ng->entry->lines[ iLine ] );

                RightCompress( tmp );

                fprintf( f, "%s\n", tmp );

                free( tmp );
            }
        }

        DisplayMessage( "Saved", 0 );

        fclose( f );
    }
    else
    {
        ShowFileError( pszFile, NULL );
    }
}

/*
 */

static char *UnCompressSrc( char *pszSrc )
{
    char *p1  = pszSrc;
    int  iLen = 0;
    char *p2;
    char *pszExpanded;

    /* Work out how big the resulting string will be. */
    while ( *p1 )
    {
        if ( *p1 == (char) 0xFF )
        {
            iLen += *( ++p1 );
        }
        else
        {
            ++iLen;
        }
        ++p1;
    }

    /* Allocate space for it (the caller must free it). */
    pszExpanded = (char *) egmalloc( iLen + 1 );
    p2          = pszExpanded;
    p1          = pszSrc;

    /* Expand it. */
    while ( *p1 )
    {
        /* 0xFF is the RLE marker. */
        if ( *p1 == (char) 0xFF )
        {
            /* The byte following it is the number of spaces to expand to. */
            int iSpaces = (unsigned) *( ++p1 );
            int i;

            /* Roll out the spaces. */
            for ( i = 0; i < iSpaces; i++ )
            {
                *p2++ = ' ';
            }
        }
        else
        {
            *p2++ = *p1;
        }

        ++p1;
    }

    *p2 = 0;

    return( pszExpanded );
}
