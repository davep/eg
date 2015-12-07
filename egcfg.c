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
#include <limits.h>
#include <stdlib.h>
#ifdef DJGPP
#include <fcntl.h>
#endif

#include "eg.h"
#include "cfgfile.h"

#ifndef DJGPP
int iEightBitStart = 160;
#endif

void ConfigureEG( void )
{
    char szRcFile[ _POSIX_PATH_MAX ];

    if ( MakeHomeDotFile( "egrc", szRcFile ) )
    {
        if ( cfgReadFile( szRcFile ) )
        {
#ifndef DJGPP
            iEightBitStart = atoi( cfgGetSetting( "EightBit" ) );
#endif
            cfgReset();
        }
    }
}
