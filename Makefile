#    Expert Guide - A Text Mode Norton Guide Reader
#    Copyright (C) 1997 David A Pearson
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the license, or 
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

######################################################################
# You might want to fiddle with the following.                       #
######################################################################
CC=gcc
#CFLAGS=-Wall -g -O2 -DDEBUG
CFLAGS=-Wall -O2 -I/usr/local/include
DLIBS= -L/usr/local/lib -lslang
BINDIR=/usr/bin
MANDIR=/usr/man
NGDIR=/usr/share/norton-guides
INSTALL=/usr/bin/install
######################################################################

OBJS=	eg.o		\
	egscreen.o	\
	eglib.o		\
	ng.o		\
	egnavgte.o	\
	cfgfile.o	\
	egmenu.o	\
	eggetfld.o	\
	egpipe.o	\
	egformat.o	\
	egtxsrch.o	\
	egsave.o	\
	egregex.o	\
	eghelp.o	\
	egdraw.o	\
	egcmplte.o	\
	egdir.o		\
	egcfg.o		\
	egsigs.o

all: eg

eg: $(OBJS)
	$(CC) $(CFLAGS) -o eg $(OBJS) $(DLIBS)

cleanish:
	rm -f *~ core

clean: cleanish
	rm -f *.o

veryclean: clean
	rm -f eg eg.exe TAGS

tags:
	etags *.c *.h

tarfile: veryclean
	./mktarfile

install: eg
	$(INSTALL) -d $(BINDIR)
	$(INSTALL) eg $(BINDIR)
	$(INSTALL) -d $(MANDIR)/man1
	$(INSTALL) eg.1 $(MANDIR)/man1
	$(INSTALL) -d $(NGDIR)
	$(INSTALL) default-guide/eg.ng $(NGDIR)

eg.o: 		eg.c 		eg.h
egscreen.o: 	egscreen.c 	eg.h 		egcolour.h
eglib.o: 	eglib.c		eg.h		cfgfile.h
ng.o: 		ng.c 		eg.h
egnavgte.o: 	egnavgte.c 	eg.h
cfgfile.o:	cfgfile.c	cfgfile.h
egmenu.o:	egmenu.c	eg.h
eggetfld.o:	eggetfld.c	eg.h
egpipe.o:	egpipe.c	eg.h
egformat.o:	egformat.c	eg.h
egtxsrch.o:	egtxsrch.c	eg.h
egsave.o:	egsave.c	eg.h
egregex.o:	egregex.c	eg.h
eghelp.o:	eghelp.c	eg.h
egdraw.o:	egdraw.c	eg.h
egomplte.o:	egcmplte.c	eg.h
egdir.o:	egdir.c		eg.h
egcfg.o:	egcfg.c		eg.h		cfgfile.h
egsigs.o:	egsigs.c	eg.h
