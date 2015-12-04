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

#include <slang.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

#include "eg.h"

static void JumpToLine( PNG, int );
static void PageUp( PNG );
static void PageDown( PNG );
static void GoHome( PNG );
static void GoEnd( PNG );

char szSearch[ 60 ]   = "";
char szRxSearch[ 60 ] = "";

/*
 */

int Navigate( PNG ng, int iFirstTime )
{
    PEGMENU pegMenu     = (PEGMENU) NULL;
    PEGMENU pSeeAlso    = (PEGMENU) NULL;
    int     iExit       = 0;
    int     iMenu       = 0;
    long    lPrevious   = -1L;
    int     iSearch     = 0;
    int     iExitEG     = 0;
    int     iStuffKey   = 0;
    int     iLastSearch = (int) ' ';
    int     iReSearch   = 0;
    int     iKey;

    /* Do we wan't to restore the last location we were at? */
    if ( LastGuidePosition( 0L ) && iFirstTime && IsNgEntry( ng, LastGuidePosition( 0L ) ) )
    {
        lPrevious = FindFirstEntry( ng );
        ReadEntry( ng, LastGuidePosition( 0L ) );
    }
    /* nope, does the guide have any menus? */
    else if ( ng->header->iMenuCount > 1 )
    {
        /* Yes, go to the entry on the first menu option. */
        ReadMenuEntry( ng, 1, 0 );
    }
    else
    {
        /* Nope, find the first short or long entry and read that. */
        ReadFirstEntry( ng );
    }
    
    DrawEntry( ng, 0 );
    
    while ( !iExit )
    {
        if ( !pSeeAlso )
        {
            DrawMenu( ng, iMenu, 0 );
        }
        
        if ( !ng->entry->iLong && !iMenu )
        {
            ShowNormal( ng, ng->entry->iLast );
            ShowHigh( ng, ng->entry->iCurrent );
        }

        SLsmg_gotorc( SLtt_Screen_Rows - 1, SLtt_Screen_Cols - 1 );
        
        SLsmg_refresh();

        if ( iStuffKey )
        {
            iKey      = iStuffKey;
            iStuffKey = 0;
        }
        else
        {
            iKey = SLkp_getkey();
        }

        DisplayMessage( "", 0 );

        switch ( iKey )
        {

            case SL_KEY_F( 1 ) : /* Help */
            case '1' :
            case '?' :

                if ( !iMenu )
                {
                    DisplayHelp( ng );
                }
                break;
            
            case SL_KEY_F( 10 ) : /* Quick exit */
            case '0' :
            case 'x' :
            
                if ( !iMenu )
                {
                    iExit   = 1;
                    iExitEG = 1;
                }
                break;

            case SL_KEY_UP :    /* Up one line */
            case 'K' :
            case 'k' :
            
                if ( ng->entry->iLong && !iMenu )
                {
                    if ( ng->entry->iVisTop )
                    {
                        --ng->entry->iVisTop;
                        --ng->entry->iVisBtm;
                        /* How the hell do you scroll with slang? */
                        DrawEntry( ng, 0 );
                        iSearch = ng->entry->iVisTop;
                    }
                }
                else if ( iMenu )
                {
                    MenuUp( pSeeAlso ? pSeeAlso : pegMenu );
                }
                else
                {
                    if ( ng->entry->iCurrent )
                    {
                        ng->entry->iLast = ng->entry->iCurrent;
                        --ng->entry->iCurrent;

                        if ( ng->entry->iCurrent < ng->entry->iVisTop )
                        {
                            --ng->entry->iVisTop;
                            --ng->entry->iVisBtm;
                            /* How the hell do you scroll with slang? */
                            DrawEntry( ng, 0 );
                        }
                    }

                    iSearch = ng->entry->iCurrent;
                }
                break;

            case SL_KEY_DOWN :  /* Down one line */
            case 'J' :
            case 'j' :
            
                if ( ng->entry->iLong && !iMenu )
                {
                    if ( ng->entry->iVisBtm <  ( ng->entry->iLines - 1 ) )
                    {
                        ++ng->entry->iVisTop;
                        ++ng->entry->iVisBtm;
                        /* How the hell do you scroll with slang? */
                        DrawEntry( ng, 0 );
                        iSearch = ng->entry->iVisTop;
                    }
                }
                else if ( iMenu )
                {
                    MenuDown( pSeeAlso ? pSeeAlso : pegMenu );
                }
                else
                {
                    if ( ng->entry->iCurrent < ( ng->entry->iLines - 1 ) )
                    {
                        ng->entry->iLast = ng->entry->iCurrent;
                        ++ng->entry->iCurrent;

                        if ( ng->entry->iCurrent > ng->entry->iVisBtm )
                        {
                            ++ng->entry->iVisTop;
                            ++ng->entry->iVisBtm;
                            /* How the hell do you scroll with slang? */
                            DrawEntry( ng, 0 );
                        }
                    }

                    iSearch = ng->entry->iVisTop;
                }
                break;

            case SL_KEY_LEFT :  /* Menu left */
            case 'H' :
            case 'h' :

                if ( !ng->entry->iLong )
                {
                    if ( --iMenu == -1 )
                    {
                        iMenu = ng->header->iMenuCount - 1;
                    }
                
                    if ( pegMenu )
                    {
                        KillEgMenu( pegMenu );
                        pegMenu = (PEGMENU) NULL;
                        DrawEntry( ng, 0 );
                    }
                
                    if ( iMenu )
                    {
                        pegMenu = NewEgMenu( ng, iMenu );
                        DropMenu( pegMenu );
                    }
                }
            
                break;

            case SL_KEY_RIGHT :     /* Menu right */
            case 'L' :
            case 'l' :

                if ( !ng->entry->iLong )
                {
                    if ( ++iMenu == ng->header->iMenuCount )
                    {
                        iMenu = 0;
                    }

                    if ( pegMenu )
                    {
                        KillEgMenu( pegMenu );
                        pegMenu = (PEGMENU) NULL;
                        DrawEntry( ng, 0 );
                    }

                    if ( iMenu )
                    {
                        pegMenu = NewEgMenu( ng, iMenu );
                        DrawEntry( ng, 0 );
                        DropMenu( pegMenu );
                    }
                }
            
                break;
            
            case SL_KEY_HOME :  /* Top of page/menu */
            case '<' :

                if ( iMenu )
                {
                    MenuHome( pSeeAlso ? pSeeAlso : pegMenu );
                }
                else
                {
                    ng->entry->iLast    = ng->entry->iCurrent;
                    ng->entry->iCurrent = 0;
            
                    if ( ng->entry->iCurrent < ng->entry->iVisTop )
                    {
                        GoHome( ng );
                    }
            
                    DrawEntry( ng, 0 );

                    iSearch = ng->entry->iVisTop;
                }
                break;

            case SL_KEY_END :   /* Bottom of page/menu */
            case '>' :

                if ( iMenu )
                {
                    MenuEnd( pSeeAlso ? pSeeAlso : pegMenu );
                }
                else
                {
                    ng->entry->iLast    = ng->entry->iCurrent;
                    ng->entry->iCurrent = ( ng->entry->iLines - 1 );

                    if ( ng->entry->iCurrent > ng->entry->iVisBtm )
                    {
                        GoEnd( ng );
                    }
            
                    DrawEntry( ng, 0 );

                    iSearch = ng->entry->iLong ? ng->entry->iVisTop : ng->entry->iCurrent;
                }
                break;

            case SL_KEY_PPAGE : /* Page up (top of menu) */
            case 0x02 :
            case 'B' :
            case 'b' :
            
                if ( iMenu )
                {
                    MenuHome( pSeeAlso ? pSeeAlso : pegMenu );
                }
                else
                {
                    PageUp( ng );
                    DrawEntry( ng, 0 );
                
                    iSearch = ng->entry->iLong ? ng->entry->iVisTop : ng->entry->iCurrent;
                }
                break;
            
            case SL_KEY_NPAGE : /* Page down (bottom of menu) */
            case 0x06 :
            case 'F' :
            case 'f' :
            
                if ( iMenu )
                {
                    MenuEnd( pSeeAlso ? pSeeAlso : pegMenu );
                }
                else
                {
                    PageDown( ng );
                    DrawEntry( ng, 0 );
                
                    iSearch = ng->entry->iLong ? ng->entry->iVisTop : ng->entry->iCurrent;
                }
                break;

            case '-' :          /* Previous long entry */
            
                if ( ng->entry->lPrevious && !iMenu )
                {
                    lPrevious = ng->entry->lAddress;
                
                    ReadEntry( ng, ng->entry->lPrevious );
                    DrawEntry( ng, 0 );

                    iSearch = 0;
                }
                break;

            case '+' :          /* Next long entry */
            
                if ( ng->entry->lNext && !iMenu )
                {
                    lPrevious = ng->entry->lAddress;
                
                    ReadEntry( ng, ng->entry->lNext );
                    DrawEntry( ng, 0 );

                    iSearch = 0;
                }
                break;

            case 'Q' :          /* Backup/exit menu/exit */
            case 'q' :
#ifdef DJGPP
            case 27 :
#endif

                if ( iMenu )
                {
                    KillEgMenu( pSeeAlso ? pSeeAlso : pegMenu );
                    pSeeAlso = (PEGMENU) NULL;
                    pegMenu  = (PEGMENU) NULL;
                    iMenu    = 0;
                    DrawEntry( ng, 0 );
                }
                else if ( ng->entry->lParent > 0 )
                {
                    int iJumpLine = ng->entry->iParentLine;
                
                    ReadEntry( ng, ng->entry->lParent );

                    if ( iJumpLine > -1 && iJumpLine < 0xFFFF )
                    {
                        JumpToLine( ng, iJumpLine );
                    }
                
                    DrawEntry( ng, 0 );

                    iSearch = iJumpLine;
                }
                else if ( lPrevious > -1L && ng->entry->iLong )
                {
                    ReadEntry( ng, lPrevious );
                    DrawEntry( ng, 0 );

                    iSearch = 0;
                }
                else 
                {
                    SLsmg_cls();
                    SLsmg_refresh();
                    iExit   = 1;
                    iExitEG = 1;
                }
            
                break;

            case '\r' :         /* Select short entry/menu item */

                if ( iMenu )
                {
                    if ( IsNgEntry( ng, pSeeAlso ?
                                    ng->entry->pSeeAlso->entries[ pSeeAlso->iCurrent ].lOffset :
                                    ng->menus[ iMenu ]->entries[ pegMenu->iCurrent ].lOffset ) )
                    {
                        lPrevious = ng->entry->lAddress;
            
                        if ( pSeeAlso )
                        {
                            ReadEntry( ng, ng->entry->pSeeAlso->entries[ pSeeAlso->iCurrent ].lOffset );
                        }
                        else
                        {
                            ReadMenuEntry( ng, iMenu, pegMenu->iCurrent );
                        }
                    }
                    else
                    {
                        ShowError( "Menu item does not point to a valid Guide entry!" );
                    }
                
                    KillEgMenu( pSeeAlso ? pSeeAlso : pegMenu );
                    iMenu    = 0;
                    pegMenu  = (PEGMENU) NULL;
                    pSeeAlso = (PEGMENU) NULL;
                
                    DrawEntry( ng, 0 );
                }
                else if ( !ng->entry->iLong )
                {
                    if ( IsNgEntry( ng, ng->entry->offsets[ ng->entry->iCurrent ] ) )
                    {
                        lPrevious = ng->entry->lAddress;
                    
                        ReadEntry( ng, ng->entry->offsets[ ng->entry->iCurrent ] );
                        DrawEntry( ng, 0 );
                    }
                }
                else if ( ng->entry->iSeeAlso )
                {
                    lPrevious = ng->entry->lAddress;
                
                    pSeeAlso = NewEgMenuSeeAlso( ng );
                    iMenu    = 1;

                    DropMenu( pSeeAlso );
                }

                iSearch = 0;
            
                break;

            case '|' :          /* Pipe page via... */
            
                if ( !iMenu )
                {
                    char szPipeCmd[ _POSIX_PATH_MAX ] = "";
                
                    if ( GetField( "Pipe", szPipeCmd, sizeof( szPipeCmd ), EG_GF_DEFA ) )
                    {
                        if ( *szPipeCmd )
                        {
                            KillScreen();
                            PipeText( ng, szPipeCmd );
                            InitScreen( 0 );
                            DrawBackground( ng, 0 );
                            DrawMenu( ng, iMenu, 0 );
                            ShowStdMsg( 0 );
                            DrawEntry( ng, 0 );
                        }
                    }
                }
                break;

            case ' ' :          /* Simple text search */
            
                if ( !iMenu )
                {
                    if ( iReSearch ? 1 : GetField( "Search", szSearch, sizeof( szSearch ), EG_GF_DEFA ) )
                    {
                        iReSearch = 0;
                        
                        if ( *szSearch )
                        {
                            int iLine = SimpleTextSearch( ng, szSearch, iSearch );

                            iLastSearch = iKey;
                            
                            if ( iLine > -1 )
                            {
                                JumpToLine( ng, iLine );
                                DrawEntry( ng, 0 );

                                if ( ng->entry->iLong )
                                {
                                    ShowHigh( ng, iLine );
                                }
                            
                                if ( iLine < ng->entry->iLines )
                                {
                                    iSearch = iLine + 1;
                                }
                                else
                                {
                                    iSearch = 0;
                                }
                            }
                            else
                            {
                                if ( ng->entry->iLong && iSearch )
                                {
                                    ShowNormal( ng, iSearch - 1 );
                                }
                            
                                iSearch = 0;
                            
                                ShowError( "Not found" );
                            }
                        }
                    }
                }
            
                break;

            case '\\' :         /* Regular expression search */
            case '/' :
            
                if ( !iMenu )
                {
                    if ( iReSearch ? 1 : GetField( iKey == '/' ? "RegExp(ci)" : "RegExp",
                                                   szRxSearch, sizeof( szRxSearch ),
                                                   EG_GF_DEFA ) )
                    {
                        iReSearch = 0;
                            
                        if ( *szRxSearch )
                        {
                            int iLine = RegExSearch( ng, szRxSearch, iSearch, iKey == '/' );

                            iLastSearch = iKey;
                            
                            if ( iLine > -1 )
                            {
                                JumpToLine( ng, iLine );
                                DrawEntry( ng, 0 );

                                if ( ng->entry->iLong )
                                {
                                    ShowHigh( ng, iLine );
                                }
                            
                                if ( iLine < ng->entry->iLines )
                                {
                                    iSearch = iLine + 1;
                                }
                                else
                                {
                                    iSearch = 0;
                                }
                            }
                            else
                            {
                                if ( ng->entry->iLong && iSearch )
                                {
                                    ShowNormal( ng, iSearch - 1 );
                                }
                            
                                iSearch = 0;

                                if ( iLine == -1 )
                                {
                                    ShowError( "Not found" );
                                }
                            }
                        }
                    }
                }

                break;

            case 'n' :          /* Redo last search */

                iStuffKey = iLastSearch;
                iReSearch = 1;
                break;
                
            case 'S' :          /* Save page to file */
            case 's' :
            case 'w' :

                if ( !iMenu )
                {
                    char szSaveTo[ _POSIX_PATH_MAX ] = "";
                
                    if ( GetField( iKey == 'S' ? "Save Raw To" :
                                   iKey == 's' ? "Save Text To" :
                                   "Save Source To", szSaveTo, sizeof( szSaveTo ),
                                   EG_GF_FILENAME ) )
                    {
                        if ( *szSaveTo )
                        {
                            ExpandPath( szSaveTo );
                            SaveToFile( ng, szSaveTo,
                                        iKey == 'S' ? EG_SAVE_RAW
                                        : iKey == 's' ? EG_SAVE_SANE
                                        : EG_SAVE_SOURCE );
                        }
                    }
                }

                break;

            case 'r' :          /* Read in a new guide */

                if ( !iMenu )
                {
                    char szFile[ _POSIX_PATH_MAX ] = "";

                    if ( GetField( "Guide", szFile, sizeof( szFile ), EG_GF_FILENAME ) )
                    {
                        if ( *szFile && IsAnNG( szFile, 1 ) )
                        {
                            ExpandPath( szFile );
                            CurrentGuide( szFile );
                            iExit = 1;
                        }
                    }
                }

                break;

            case 'v' :          /* Display version details */
                ShowStdMsg( 0 );
                break;

            case 'D' :
            case 'd' :

                if ( !iMenu )
                {
                    iExit = EGDir( iKey == 'D' );
                    DrawEntry( ng, 0 );
                }
                break;

            case 12 :

                KillScreen();
                InitScreen( 0 );
                DrawBackground( ng, 0 );
                DrawMenu( ng, iMenu, 0 );
                DrawEntry( ng, 0 );
                if ( !ng->entry->iLong )
                {
                    ShowNormal( ng, ng->entry->iLast );
                    ShowHigh( ng, ng->entry->iCurrent );
                }
                DrawMenu( ng, iMenu, 0 );
                if ( pSeeAlso || pegMenu )
                {
                    DropMenu( pSeeAlso ? pSeeAlso : pegMenu );
                }
                break;
        }
    }

    return( iExitEG );
}

/*
 */

static void JumpToLine( PNG ng, int iLine )
{
    int iPage = DisplayRows() - 1;
    
    ng->entry->iCurrent = iLine;
    ng->entry->iLast    = iLine;

    if ( ng->entry->iLines > iPage )
    {
        int ihPage = iPage / 2;
        
        ng->entry->iVisTop = iLine - ihPage;
        ng->entry->iVisBtm = ng->entry->iVisTop + iPage;
    }
    
    if ( ng->entry->iVisTop < 0 )
    {
        ng->entry->iVisTop = 0;
        ng->entry->iVisBtm = iPage;
    }
    else if ( ng->entry->iVisBtm > ( ng->entry->iLines - 1 ) )
    {
        ng->entry->iVisBtm = ng->entry->iLines - 1;
        ng->entry->iVisTop = ng->entry->iVisBtm - iPage;
    }
}

/*
 */

static void PageUp( PNG ng )
{
    int iPage = DisplayRows() - 1;

    if ( ng->entry->iLines > iPage )
    {
        if ( ng->entry->iLong )
        {
            ng->entry->iCurrent = ng->entry->iVisTop - iPage;
        }
        else
        {
            ng->entry->iCurrent -= iPage;
        }

        if ( ng->entry->iCurrent < 0 )
        {
            ng->entry->iCurrent = ng->entry->iVisTop = 0;
            ng->entry->iVisBtm  = iPage;
        }
        else
        {
            ng->entry->iVisTop = ng->entry->iCurrent;
            ng->entry->iVisBtm = ng->entry->iVisTop + iPage;
        }
    }
    else
    {
        ng->entry->iCurrent = ng->entry->iLast = 0;
    }
}

/*
 */

static void PageDown( PNG ng )
{
    int iPage = DisplayRows() - 1;

    if ( ng->entry->iLines > iPage )
    {
        if ( ng->entry->iLong )
        {
            ng->entry->iCurrent = ng->entry->iVisBtm + iPage;
        }
        else
        {
            ng->entry->iCurrent += iPage;
        }

        if ( ng->entry->iCurrent > ( ng->entry->iLines - 1 ) )
        {
            ng->entry->iCurrent = ng->entry->iLines - 1;
            ng->entry->iVisTop  = ng->entry->iCurrent - iPage;
            ng->entry->iVisBtm  = ng->entry->iCurrent;
        }
        else
        {
            ng->entry->iVisTop = ng->entry->iCurrent - iPage;
            ng->entry->iVisBtm = ng->entry->iCurrent;
        }

        ng->entry->iLast = ng->entry->iCurrent;
    }
    else
    {
        ng->entry->iCurrent = ng->entry->iLines - 1;
        ng->entry->iLast    = ng->entry->iCurrent;
    }
}

/*
 */

static void GoHome( PNG ng )
{
    ng->entry->iLast    = ng->entry->iCurrent;
    ng->entry->iVisTop  = ng->entry->iCurrent;
    ng->entry->iVisBtm  = DisplayRows() - 1;
}

/*
 */

static void GoEnd( PNG ng )
{
    ng->entry->iLast    = ng->entry->iCurrent;
    ng->entry->iVisBtm  = ng->entry->iCurrent;
    ng->entry->iVisTop  = ng->entry->iVisBtm - ( DisplayRows() - 1 );
}
