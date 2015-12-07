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

#ifndef __EG_H

#define __EG_H

#include <assert.h>  /* For the moment anyway */

#define EG_VERSION   "1.01"     /* Version number. */
#define EG_ENV_PATH  "EG_PATH"  /* Environment variable that contains the
                                   path to the NG files. */

/* Decrypt the NGs "encryption". */

#define NG_DECRYPT( x ) ( (unsigned char) ( x ^ 0x1A ) )

/* Type for holding the deails about a guide. */

typedef struct structNGHEADER
{
    int    iMagic;
    int    iMenuCount;
    char   szTitle[ 41 ];
    char   pszCredits[ 5 ][ 67 ];
} NGHEADER, *PNGHEADER;

/* Structure to hold menu entries. */

typedef struct structNGMENUENTRY
{
    long lOffset;
    char szName[ 70 ];
} NGMENUENTRY, *PNGMENUENTRY;

/* Structure to hold a menu. */

typedef struct structNGMENU
{
    char         szName[ 50 ];
    PNGMENUENTRY entries;
    int          iEntries;
} NGMENU, *PNGMENU;

/* Structure to hold the current entry */

typedef struct structNGENTRY
{
    char    **lines;
    char    **formatted;
    long    *offsets;
    int     iLines;
    long    lOffset;
    int     iParentLine;
    long    lParent;
    int     iVisTop;
    int     iVisBtm;
    int     iSeeAlso;
    int     iLong;
    long    lPrevious;
    long    lNext;
    int     iCurrent;
    int     iLast;
    long    lAddress;
    PNGMENU pSeeAlso;
} NGENTRY, *PNGENTRY;

/* Structure for tracking a guide. */

typedef struct structNG
{
    PNGHEADER header;
    FILE      *file;
    PNGMENU   *menus;
    PNGENTRY  entry;
    long      lGuideStart;
} NG, *PNG;

/* Structure for holding drawable menu details. */

typedef struct structEGMENU
{
    PNGMENU pngMenu;
    int     iCurrent;
    int     iPrevious;
    int     iTop;
    int     iLeft;
    int     iHeight;
    int     iWidth;
} EGMENU, *PEGMENU;

#ifdef DEBUG
extern int egDebug;
#define EGDEBUG( x ) if ( egDebug ) x
#else
#define EGDEBUG( x )
#endif

/* Function prototypes. */
char    *GuidePath( char * );
char    *CurrentGuide( char * );
void    *egmalloc( size_t );
void    *egcalloc( size_t, size_t );
char    *egstrdup( const char * );
void    egerror( char *, ... );
void    egperror( char *, ... );
PNG     OpenGuide( void );
void    CloseGuide( PNG );
void    InitScreen( int );
void    KillScreen( void );
void    DrawBackground( PNG, int );
void    DrawMenu( PNG, int, int );
void    GetStrZ( PNG, char *, int );
void    ReadMenuEntry( PNG, int, int );
void    ReadEntry( PNG, long );
void    ReadFirstEntry( PNG );
int     Navigate( PNG, int );
int     ReadWord( PNG, int );
long    ReadLong( PNG, int );
int     DisplayTop( void );
int     DisplayBtm( void );
int     DisplayRows( void );
int     DisplayLeft( void );
int     DisplayRight( void );
int     DisplayWidth( void );
void    LoadLastGuide( void );
long    LastGuidePosition( long );
void    RememberLastGuide( PNG );
PEGMENU NewEgMenu( PNG, int );
PEGMENU NewEgMenuSeeAlso( PNG );
void    KillEgMenu( PEGMENU );
void    DropMenu( PEGMENU );
void    MenuDown( PEGMENU );
void    MenuUp( PEGMENU );
void    MenuHome( PEGMENU );
void    MenuEnd( PEGMENU );
int     IsNgEntry( PNG, long );
void    DisplayMessage( char *, int );
void    ShowStdMsg( int );
int     GetField( char *, char *, int, int );
void    PipeText( PNG, char * );
char    *FormatLine( char *, int );
char    *MakeBufferSane( char * );
void    ShowError( char * );
void    ShowFileError( char *, char * );
int     SimpleTextSearch( PNG, char *, int );
int     RegExSearch( PNG, char *, int, int );
void    SaveToFile( PNG, char *, int );
void    DisplayHelp( PNG );
void    SaneDisplay( int, int, unsigned char );
void    DrawEntry( PNG, int );
void    ShowNormal( PNG, int );
void    ShowHigh( PNG, int );
void    RightCompress( char * );
int     CompleteFilename( char * );
int     EGDir( int );
void    WriteStringSane( char * );
void    ConfigureEG( void );
int     MakeHomeDotFile( char *, char * );
int     IsAnNG( char *, int );
long    FindFirstEntry( PNG );
char    *HomeDir( void );
void    ExpandPath( char * );
void    MakeAbsFilename( char * );
void    InitSignals( void );

/* Flags to say how we want to save an entry. */
#define EG_SAVE_SANE      1
#define EG_SAVE_RAW       2
#define EG_SAVE_SOURCE    3

/* Flags to tell GetField() how to behave. */
#define EG_GF_DEFA        0
#define EG_GF_FILENAME    1

/* Filesystem characters. */
#ifndef DJGPP
#define SLASHC  '/'
#define SLASHS  "/"
#define PATHSEP ":"
#else
#define SLASHC  '\\'
#define SLASHS  "\\"
#define PATHSEP ";"
#endif

#ifndef DJGPP
extern int iForceSane;
#endif

/* Somewhere to hold the colour pointers. */
extern int iColourNorm;
extern int iColourSele;
extern int iColourBold;
extern int iColourUndl;
extern int iColourRev;
extern int iColourTitle;
extern int iColourMenu;
extern int iColourBar;
extern int iColourMsg;
extern int iColourError;
extern int iColourHelp;

/* Configuration variables. */

#ifndef DJGPP
extern int iEightBitStart;
#endif

#endif
