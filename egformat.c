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

#include "eg.h"

static int Hex2Byte( char * );

/*
 */

char *FormatLine( char *pszBuffer, int iColour )
{
    char *psz  = pszBuffer;
    int  iLen  = 0;
    int  iChar = 0;
    int  iAttr = iColourNorm;
    char *pszClean;
    char *pszTgt;

    /* Work out how long the expanded string will be including the rolled
       out RLE spaces. Note that the resulting length may be longer that
       the resulting string because we also count all the tokens. However,
       a buffer that is slightly too big is much better than one that is
       too small. <g> */
    while ( *psz )
    {
        if ( *psz == (char) 0xFF )
        {
            iLen += *( ++psz );
        }
        else
        {
            ++iLen;
        }
        ++psz;
    }

    /* Limit it to the width of the display area. */
    if ( iLen < DisplayWidth() )
    {
        iLen = DisplayWidth();
    }

    iLen     = ( iLen << 1 ) + 2;
    pszClean = (char *) egmalloc( iLen );
    psz      = pszBuffer;
    pszTgt   = pszClean;

    while ( *psz )
    {
        /* Are we looking at a token? */
        if ( *psz == '^' )
        {
            ++psz;

            switch ( *psz )
            {
                case 'a' :          /* Attribute */
                case 'A' :
                    ++psz;
                    iAttr = Hex2Byte( psz ); 
                    ++psz;
                    break;
                    
                case 'b' :          /* Bold */
                case 'B' :
                    iAttr = ( iAttr == iColourBold ?
                              iColourNorm : iColourBold );
                    break;
                    
                case 'c' :          /* Character */
                case 'C' :
                    ++psz;
                    if ( !( *pszTgt = Hex2Byte( psz ) ) )
                    {
                        *pszTgt = ' ';
                    }
                    ++pszTgt;
                    ++psz;
                    if ( iColour )
                    {
                        *pszTgt++ = (unsigned char) iAttr;
                    }
                    ++iChar; 
                    break;
                    
                case 'n' :          /* Normal */
                case 'N' :
                    iAttr = iColourNorm;
                    break;
                    
                case 'r' :          /* Reverse */
                case 'R' :
                    iAttr = ( iAttr == iColourRev ?
                              iColourNorm : iColourRev );
                    break;
                    
                case 'u' :          /* Underlined */
                case 'U' :
                    iAttr = ( iAttr == iColourUndl ?
                              iColourNorm : iColourUndl );
                    break;
                    
                case '^' :          /* A '^' character. */
                    *pszTgt++ = '^';
                    if ( iColour )
                    {
                        *pszTgt++ = (unsigned char) iAttr;
                    }
                    ++iChar;
                    break;
                    
                default :           /* Bogus '^' */
                    --psz;
            }
        }
        else if ( *psz == (char) 0xFF ) /* RLE spaces */
        {
            /* RLE spaces are in the format 0xFF<Count>. */
            
            int iSpaces;
            int i;
            
            ++psz;
            
            iSpaces = *psz;
            
            for ( i = 0; i < iSpaces; i++ )
            {
                *pszTgt++ = ' ';
                if ( iColour )
                {
                    *pszTgt++ = (unsigned char) iAttr;
                }
                ++iChar;
            }
        }
        else
        {
            /* Turn any character 0s to spaces. */
            if ( !( *pszTgt = *psz ) )
            {
                *pszTgt = ' ';
            }
            
            ++pszTgt;
            
            if ( iColour )
            {
                *pszTgt++ = (unsigned char) iAttr;
            }
            
            ++iChar;
        }

        ++psz;
    }

    /* If we are short of the display width, padd the string out. */
    while ( iChar++ < DisplayWidth() )
    {
        *pszTgt++ = ' ';
        if ( iColour )
        {
            *pszTgt++ = (unsigned char) iColourNorm;
        }
    }

    *pszTgt++ = 0;
    *pszTgt   = 0;

    return( pszClean );
}


/*
 */

static int Hex2Byte( char *psz )
{
    int iByte;
    int i;
    unsigned char bByte = 0;
    
    for ( i = 0; i < 2; i++, psz++ )
    {
        *psz = (char) toupper( *psz );
        
        if ( *psz > '/' && *psz < ':' )
        {
            iByte = ( ( (int) *psz ) - '0' );
        }
        else if ( *psz > '@' && *psz < 'G' )
        {
            iByte = ( ( (int) *psz ) - '7' );
        }
        else
        {
            iByte = 0;
        }
        
        bByte += ( iByte * ( !i ? 16 : 1 ) );
    }
    
    return( (int) bByte );
}

