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
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "eg.h"

/* Structure for holding entries in the history buffer. */
typedef struct structGFHISTORY
{
    char                   *text;
    struct structGFHISTORY *next;
    struct structGFHISTORY *prev;
} GFHISTORY, *PGFHISTORY;

static PGFHISTORY gfHist   = NULL; /* Pointer to the head of the buffer. */
static PGFHISTORY gfEOL    = NULL; /* Pointer to the tail of the buffer. */
static int        iHistLen = 0;    /* Count of entries in the buffer.    */

#define EG_GH_MAX 100              /* Max entries in the buffer. */

#define EG_GF_SCROLL 10            /* Number of chatacters to scroll. */
#define EG_GF_EOB    10            /* End of buffer from edge of screen. */

static void HistAdd( char * );

/*
 */

int GetField( char *pszPrompt, char *pszBuffer, int iLength, int iFlags )
{
    char       *pszRevert = (char *) egmalloc( strlen( pszBuffer ) + 1 );
    char       *p         = pszBuffer;
    int        iEOS       = strlen( pszBuffer );
    int        iOk        = 1;
    int        iOff       = 0;
    int        iInputting = 1;
    int        iMoved     = 0;
    int        iInsert    = 1;
    int        iBegin     = 0;
    int        iStartCol  = strlen( pszPrompt ) + 2;
    int        iCol       = iStartCol;
    PGFHISTORY gfhCurrent = NULL;
    int        iStuffKey  = 0;
    int        iKey;

    --iLength;

    strcpy( pszRevert, pszBuffer );

    SLsmg_set_color( iColourNorm );

    /* Dirty the message area so that the clear at the end will clear. */
    DisplayMessage( " ", 0 );

    SLsmg_gotorc( SLtt_Screen_Rows - 1, 0 );
    SLsmg_write_string( pszPrompt );
    SLsmg_write_string( ": " );
    SLsmg_refresh();

    while ( iInputting )
    {
        if ( iCol > ( SLtt_Screen_Cols - EG_GF_EOB ) )
        {
            iBegin += EG_GF_SCROLL;
            iCol   -= EG_GF_SCROLL;
        }
        else if ( iCol < iStartCol )
        {
            iBegin -= EG_GF_SCROLL;
            iCol   += EG_GF_SCROLL;
        }

        if ( iBegin < 0 )
        {
            iBegin = 0;
            iCol   = iStartCol;
        }

        if ( !iStuffKey )
        {
            SLsmg_fill_region( SLtt_Screen_Rows - 1, iStartCol, 1,
                               SLtt_Screen_Cols, ' ' );
            SLsmg_gotorc( SLtt_Screen_Rows - 1, iStartCol );
            SLsmg_write_string( pszBuffer + iBegin );

            SLsmg_gotorc( SLtt_Screen_Rows - 1, iCol );
            SLsmg_refresh();

            iKey = SLkp_getkey();
        }
        else
        {
            iKey      = iStuffKey;
            iStuffKey = 0;
        }

        switch ( iKey )
        {

            case '\r' :         /* Return */

                RightCompress( pszBuffer );

                if ( *pszBuffer )
                {
                    HistAdd( pszBuffer );
                }

                iInputting = 0;

                break;

            case 7 :            /* Ctrl-G or ESC(DJGPP) */
#ifdef DJGPP
            case 27 :
#endif
                iOk        = 0;
                iInputting = 0;
                strcpy( pszBuffer, pszRevert );
                break;

            case SL_KEY_IC :    /* Toggle insert status */

                iInsert = !iInsert;
                break;

            case SL_KEY_LEFT :  /* Move left */

                if ( iOff )
                {
                    --iOff;
                    --iCol;
                    --p;
                }

                iMoved = 1;

                break;

            case SL_KEY_RIGHT : /* Move right */

                if ( iOff < iEOS )
                {
                    ++iOff;
                    ++iCol;
                    ++p;
                }

                iMoved = 1;

                break;

            case SL_KEY_UP :    /* Recall history backwards */

                if ( gfEOL )
                {
                    int iWentBack = 0;

                    if ( !gfhCurrent )
                    {
                        gfhCurrent = gfEOL;
                        iWentBack  = 1;
                        strncpy( pszBuffer, gfhCurrent->text, iLength );
                    }
                    else if ( gfhCurrent->prev )
                    {
                        strncpy( pszBuffer, gfhCurrent->prev->text, iLength );
                        gfhCurrent = gfhCurrent->prev;
                        iWentBack  = 1;
                    }

                    if ( iWentBack )
                    {
                        iEOS      = strlen( pszBuffer );
                        iStuffKey = SL_KEY_HOME;
                    }
                    else
                    {
                        SLtt_beep();
                    }
                }
                else
                {
                    SLtt_beep();
                }

                break;

            case SL_KEY_DOWN :  /* Recall history forwards */

                if ( gfEOL && gfhCurrent )
                {
                    if ( gfhCurrent->next )
                    {
                        strncpy( pszBuffer, gfhCurrent->next->text, iLength );
                        gfhCurrent = gfhCurrent->next;
                    }
                    else
                    {
                        strcpy( pszBuffer, "" );
                        gfhCurrent = (PGFHISTORY) NULL;
                    }

                    iEOS      = strlen( pszBuffer );
                    iStuffKey = SL_KEY_HOME;
                }
                else
                {
                    SLtt_beep();
                }

                break;

            case SL_KEY_HOME :  /* Start of buffer */
            case 0x01 :         /* including Ctrl-A */

                iCol   = iStartCol;
                p     -= iOff;
                iOff   = 0;
                iBegin = 0;
                iMoved = 1;
                break;

            case SL_KEY_END :   /* End of buffer */
            case 0x05 :         /* including Ctrl-E */

                iCol  += ( iEOS - iOff );
                p     += ( iEOS - iOff );
                iOff   = iEOS;
                iMoved = 1;

                if ( iCol > ( SLtt_Screen_Cols - EG_GF_EOB ) )
                {
                    iBegin = iEOS - ( SLtt_Screen_Cols - EG_GF_EOB - iStartCol );
                    iCol   = SLtt_Screen_Cols - EG_GF_EOB;
                }
                break;

            case SL_KEY_BACKSPACE : /* Backspace */
            case 127 :

                if ( iOff )
                {
                    --iCol;
                    --iOff;
                    --p;
                    strcpy( p, p + 1 );
                    pszBuffer[ iEOS - 1 ] = 0;
                    --iEOS;
                }

                iMoved = 1;

                break;

            case SL_KEY_DELETE : /* Delete */

                if ( *p )
                {
                    strcpy( p, p + 1 );
                    --iEOS;
                }

                iMoved = 1;

                break;

            case 11 :           /* Ctrl-K, kill to end of line */

                iEOS  -= strlen( p );
                *p     = 0;
                iMoved = 1;
                break;

            case '\t' :         /* Filename completion */

                if ( iFlags & EG_GF_FILENAME )
                {
                    if ( CompleteFilename( pszBuffer ) )
                    {
                        SLtt_beep();
                    }

                    iEOS      = strlen( pszBuffer );
                    iStuffKey = SL_KEY_END;
                }
                break;

            default :

                if ( isprint( iKey ) && iOff < iLength )
                {
                    if ( !iMoved )
                    {
                        iEOS = 0;
                        *p   = 0;
                    }

                    if ( iInsert && iOff < iEOS )
                    {
                        char *temp = (char *) egmalloc( ( iLength - iOff ) + 1 );

                        strncpy( temp, p,  iLength - iOff );
                        strcpy( p + 1, temp );

                        if ( iEOS < iLength )
                        {
                            ++iEOS;
                        }

                        pszBuffer[ iEOS ] = 0;

                        free( temp );
                    }

                    *p++ = (char) iKey;
                    ++iCol;
                    ++iOff;

                    iMoved = 1;

                    if ( iOff > iEOS )
                    {
                        *p = 0;
                        ++iEOS;
                    }
                }
                break;
        }
    }

    free( pszRevert );

    DisplayMessage( "", 0 );

    return( iOk );
}

/*
 */

static void HistAdd( char *psz )
{
    PGFHISTORY gfNew = egmalloc( sizeof( GFHISTORY ) );

    if ( iHistLen == EG_GH_MAX )
    {
        PGFHISTORY tmp = gfHist->next;

        free( gfHist->text );
        free( gfHist );

        gfHist       = tmp;
        gfHist->prev = (PGFHISTORY) NULL;

        --iHistLen;
    }

    gfNew->next = (PGFHISTORY) NULL;
    gfNew->prev = gfEOL;

    if ( gfEOL )
    {
        gfEOL->next = gfNew;
    }

    gfEOL       = gfNew;
    gfEOL->text = egmalloc( strlen( psz ) + 1 );

    if ( !gfHist )
    {
        gfHist = gfEOL;
    }

    strcpy( gfEOL->text, psz );

    ++iHistLen;
}
