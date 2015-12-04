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
#include <string.h>
#include <signal.h>
#include <slang/slang.h>

#include "eg.h"

void SuspendEg( int );
void ResumeEg( int );

/*
 */

void InitSignals( void )
{
#ifndef DJGPP
    struct sigaction act;

    memset( &act, 0, sizeof( act ) );
    
    act.sa_handler = SuspendEg;
    sigaction( SIGTSTP, &act, 0 );

    act.sa_handler = ResumeEg;
    sigaction( SIGCONT, &act, 0 );
#endif
}

/*
 */

#ifndef DJGPP
void SuspendEg( int i )
{
    SLsmg_suspend_smg();
    SLang_reset_tty();
    kill( 0, SIGSTOP );
}
#endif

/*
 */

#ifndef DJGPP
void ResumeEg( int i )
{
    SLsmg_resume_smg();
    SLang_init_tty( 3, 0, 0 );
    SLkp_init();
    SLtty_set_suspend_state( 1 );
    SLtt_get_screen_size();
    SLsmg_Display_Eight_Bit = iEightBitStart;
    SLtt_Ignore_Beep = 1;        /* Audible bell only */
    SLtt_Term_Cannot_Scroll = 1; /* I'm told this gives better IO speed,
                                    I may remove this at some point. */
}
#endif
