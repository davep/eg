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
#include <slang/slang.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "eg.h"
#include "egcolour.h"

/* Local private functions. */
static void InitColours( void );
static void WriteMessage( char *, int );

/* System colours */
int iColourNorm  = HI_WHITE_BLUE;
int iColourSele  = HI_WHITE_RED;
int iColourBold  = HI_YELLOW_BLUE;
int iColourUndl  = HI_MAGENTA_BLUE;
int iColourRev   = HI_WHITE_RED;
int iColourTitle = BLACK_WHITE;
int iColourMenu  = BLACK_WHITE;
int iColourBar   = HI_WHITE_RED;
int iColourMsg   = BLACK_WHITE;
int iColourError = RED_WHITE;
int iColourHelp  = HI_WHITE_CYAN;

/*
 */

void InitScreen( int iFirstTime )
{
    /* Get slang up and going */
    
    SLtt_get_terminfo();
    SLang_init_tty( 3, 0, 0 );
    SLsmg_init_smg();
    SLkp_init();
#ifndef DJGPP
    SLtty_set_suspend_state( 1 );
    SLtt_get_screen_size();
    SLsmg_Display_Eight_Bit = iEightBitStart;
#endif
    SLtt_Ignore_Beep = 1;        /* Audible bell only */
    SLtt_Term_Cannot_Scroll = 1; /* I'm told this gives better IO speed,
                                    I may remove this at some point. */

    if ( iFirstTime )
    {
        InitColours();
    }
}

/*
 */

void KillScreen( void )
{
    /* Shutdown slang */
    SLsmg_reset_smg();
    SLang_reset_tty();
}

/*
 */

void DrawBackground( PNG ng, int iRefresh )
{
    if ( ng )
    {
        SLsmg_set_color( iColourTitle );
        SLsmg_fill_region( 0, 0, 1, SLtt_Screen_Cols, ' ' );
        SLsmg_gotorc( 0, ( SLtt_Screen_Cols / 2 ) -
                      ( strlen( ng->header->szTitle ) / 2 ) );
        SLsmg_write_string( ng->header->szTitle );
    }
    
    SLsmg_set_color( iColourMenu );
    SLsmg_fill_region( SLtt_Screen_Rows - 1, 0, 1, SLtt_Screen_Cols, ' ' );

    SLsmg_set_color( iColourNorm );
    SLsmg_fill_region( 1, 1, SLtt_Screen_Rows - 3, SLtt_Screen_Cols - 2, ' ' );
    
    DrawMenu( ng, 0, 0 );

    if ( iRefresh )
    {
        SLsmg_refresh();
    }
}

/*
 */

void DrawMenu( PNG ng, int iSelected, int iRefresh )
{
    if ( ng && ng->entry )
    {
        SLsmg_set_color( iColourMenu );
        SLsmg_fill_region( 1, 0, 1, SLtt_Screen_Cols, ' ' );

        if ( !ng->entry->iLong )
        {
            int iCol = 1;
            int i;

            for ( i = 0; i < ng->header->iMenuCount; i++ )
            {
                SLsmg_set_color( iSelected == i ? iColourBar : iColourMenu );
                SLsmg_gotorc( 1, iCol );
                SLsmg_write_string( ng->menus[ i ]->szName );
            
                iCol += strlen( ng->menus[ i ]->szName ) + 2;
            }
        }
        else if ( ng->entry->iSeeAlso )
        {
            SLsmg_gotorc( 1, 1 );
            SLsmg_set_color( iColourBar );
            SLsmg_write_string( "See also" );
        }
    }
    
    if ( iRefresh )
    {
        SLsmg_refresh();
    }
}

/*
 */

int DisplayTop( void )
{
    return( 2 );
}

/*
 */

int DisplayBtm( void )
{
    return( SLtt_Screen_Rows - 1 );
}

/*
 */

int DisplayRows( void )
{
    return( DisplayBtm() - DisplayTop() );
}

/*
 */

int DisplayLeft( void )
{
    return( 0 );
}

/*
 */

int DisplayRight( void )
{
    return( SLtt_Screen_Cols );
}

/*
 */

int DisplayWidth( void )
{
    return( DisplayRight() - DisplayLeft() );
}

/*
 */

void DisplayMessage( char *pszMsg, int iRefresh )
{
    WriteMessage( pszMsg, iColourMsg );

    if ( iRefresh )
    {
        SLsmg_refresh();
    }
}

/*
 */

void ShowStdMsg( iRefresh )
{
    char *pszMsg = (char *) egmalloc( strlen( CurrentGuide( NULL ) ) +
                                      strlen( EG_VERSION ) + 7 );

    sprintf( pszMsg, "eg v%s: %s", EG_VERSION,
             CurrentGuide( NULL ) );
    
    DisplayMessage( pszMsg, iRefresh );

    free( pszMsg );
}

/*
 */

void ShowError( char *pszErr )
{
    WriteMessage( pszErr, iColourError );
    SLtt_beep();
}

/*
 */

void ShowFileError( char *pszFile, char *pszError )
{
    char *err = egmalloc( strlen( pszFile ) + strlen(
        pszError ? pszError : strerror( errno ) ) + 3 );

    sprintf( err, "%s: %s", pszFile, pszError ? pszError : strerror( errno ) );

    ShowError( err );

    free( err );
}

/*
 */

static void WriteMessage( char *pszMsg, int iColour )
{
    static int iMustClear = 1;
    char       *psz       = (char *) NULL;

    if ( *pszMsg )
    {
        psz = (char *) egmalloc( SLtt_Screen_Cols + 1 );

        if ( strlen( pszMsg ) > SLtt_Screen_Cols )
        {
            strncpy( psz, pszMsg, SLtt_Screen_Cols );
        }
        else
        {
            strcpy( psz, pszMsg );
        }
    }

    if ( iMustClear )
    {
        SLsmg_set_color( iColour );
        SLsmg_fill_region( SLtt_Screen_Rows - 1, 0, 1, SLtt_Screen_Cols, ' ' );
    }

    if ( *pszMsg )
    {
        SLsmg_set_color( iColour );
        SLsmg_gotorc( SLtt_Screen_Rows - 1, 0 );
        SLsmg_write_string( pszMsg );
        
        free( psz );
    }

    iMustClear = *pszMsg;
}

/*
 */

static void InitColours( void )
{
    #define EG_MAX_COLOURS 16

    if ( SLtt_Use_Ansi_Colors )
    {
        char *names[] = { "black", "blue", "green", "cyan", "red", "magenta",
                          "brown", "lightgray", "gray", "brightblue",
                          "brightgreen", "brightcyan", "brightred",
                          "brightmagenta", "yellow", "white" };
        int fg;
        int bg;

        for ( fg = 0; fg < EG_MAX_COLOURS; fg++ )
        {
            for ( bg = 0; bg < EG_MAX_COLOURS; bg++ )
            {
                SLtt_set_color( ( 16 * bg ) + fg, "", names[ fg ],
                                names[ bg ] );
            }
        }
    }
    else
    {
        iColourNorm  = 1;
        iColourSele  = 2;
        iColourBold  = 3;
        iColourUndl  = 4;
        iColourRev   = 5;
        iColourTitle = 6;
        iColourMenu  = 7;
        iColourBar   = 8;
        iColourMsg   = 9;
        iColourError = 10;
        iColourHelp  = 11;
        
        SLtt_set_mono( iColourNorm,  "", 0 );
        SLtt_set_mono( iColourSele,  "", SLTT_REV_MASK );
        SLtt_set_mono( iColourBold,  "", SLTT_BOLD_MASK );
        SLtt_set_mono( iColourUndl,  "", SLTT_ULINE_MASK );
        SLtt_set_mono( iColourRev,   "", SLTT_REV_MASK );
        SLtt_set_mono( iColourTitle, "", SLTT_REV_MASK );
        SLtt_set_mono( iColourMenu,  "", SLTT_REV_MASK );
        SLtt_set_mono( iColourBar,   "", 0 );
        SLtt_set_mono( iColourMsg,   "", SLTT_REV_MASK );
        SLtt_set_mono( iColourError, "", SLTT_REV_MASK );
        SLtt_set_mono( iColourHelp,  "", SLTT_REV_MASK );
    }
}
