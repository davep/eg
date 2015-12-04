/*

     CFGFILE - CONFIG FILE READING LIBRARY
     Copyright (C) 1996 David A Pearson
   
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

/*
 * Modification history:
 *
 * $Log: cfgfile.c,v $
 * Revision 1.3  1997/04/13 18:13:25  davep
 * cfgReset() wasn't setting the config list point to NULL. This
 * caused problems when another config file was read in.
 *
 * Revision 1.2  1996/07/05 13:02:05  davep
 * Minor changes to allow a clean compile when using -Wall.
 *
 * Revision 1.1  1996/04/02 16:16:35  davep
 * Initial revision
 *
 *
 */

/* Standard header files. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Local header file. */

#include "cfgfile.h"

/* Handle those compilers that don't do strcasecmp(). */

#if defined( __MSDOS__ ) || defined( __EMX__ )
#define strcasecmp( x, y ) stricmp( x, y )
#endif

/* Config entry structure. */

typedef struct _cfgOPTIONS
{
    char               *id;
    char               *value;
    struct _cfgOPTIONS *next;
} cfgOPTIONS;

/* Prorotype internal functions */

static cfgOPTIONS *cfgLastOption( cfgOPTIONS * );

/* This holds the list of config options. */

static cfgOPTIONS *cfgOptions = NULL;

/*
 */

void cfgAddOption( char *pszId, char *pszValue )
{
    cfgOPTIONS *p;

    if ( !cfgOptions )
    {
	cfgOptions = (cfgOPTIONS *) calloc( sizeof( cfgOPTIONS ), 1 );
	cfgOptions->next = NULL;
	p = cfgOptions;
    }
    else
    {
	p       = cfgLastOption( cfgOptions );
	p->next = (cfgOPTIONS *) calloc( sizeof( cfgOPTIONS ), 1 );
	p       = p->next;
	p->next = NULL;
    }

    p->id    = (char *) malloc( strlen( pszId ) + 1 );
    p->value = (char *) malloc( strlen( pszValue ) + 1 );
    strcpy( p->id, pszId );
    strcpy( p->value, pszValue );
}

/*
 */

int cfgReadFile( char *pszFile )
{
    int  i  = 0;
    FILE *f = fopen( pszFile, "r" );
    char szBuffer[ 512 ];

    cfgReset();

    if ( f != NULL )
    {
	while ( fgets( szBuffer, sizeof( szBuffer ), f ) )
	{
	    szBuffer[ strlen( szBuffer ) - 1 ] = 0;
	    if ( *szBuffer && *szBuffer != ';' && *szBuffer != '#' )
	    {
		char *pszSplit = szBuffer;

		while ( *pszSplit && *pszSplit != '=' )
		{
		    ++pszSplit;
		}

		if ( *pszSplit )
		{
		    *pszSplit = 0;
		    ++pszSplit;
		    
		    if ( *pszSplit )
		    {
			cfgAddOption( szBuffer, pszSplit );
			++i;
		    }
		}
	    }
	}
	fclose( f );
    }

    return( i );
}

/*
 */

char *cfgGetSetting( char *pszId )
{
    char       *p   = NULL;
    cfgOPTIONS *cfg = cfgOptions;

    while ( cfg && !p )
    {
	if ( strcasecmp( pszId, cfg->id ) == 0 )
	{
	    p = cfg->value;
	}
	else
	{
	    cfg = (cfgOPTIONS *) cfg->next;
	}
    }

    return( p );
}

/*
 */

void cfgReset( void )
{
    cfgOPTIONS *cfg = cfgOptions;
    cfgOPTIONS *last;

    while ( cfg )
    {
	if ( cfg->id )    free( cfg->id );
	if ( cfg->value ) free( cfg->value );
	last = cfg;
	cfg  = (cfgOPTIONS *) cfg->next;
	free( last );
    }

    cfgOptions = (cfgOPTIONS *) NULL;
}

/*
 */

static cfgOPTIONS *cfgLastOption( cfgOPTIONS *p )
{
    while ( p->next )
    {
	p = p->next;
    }

    return( p );
}
