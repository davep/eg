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
 * $Log: cfgfile.h,v $
 * Revision 1.1  1996/04/02 16:16:53  davep
 * Initial revision
 *
 *
 */

/* Ensure we only get included once. */

#ifndef __CFGFILE_H
#define __CFGFILE_H

/* Prototype functions found in the "library". */

void cfgAddOption( char *, char * );
int  cfgReadFile( char * );
char *cfgGetSetting( char * );
void cfgReset( void );

#endif
