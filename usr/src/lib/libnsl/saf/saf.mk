#	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#ident	"@(#)libnet:saf/saf.mk	1.15"

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	PROPRIETARY NOTICE (Combined)
#
# This source code is unpublished proprietary information
# constituting, or derived under license from AT&T's UNIX(r) System V.
# In addition, portions of such source code were derived from Berkeley
# 4.3 BSD under license from the Regents of the University of
# California.
#
#
#
#	Copyright Notice 
#
# Notice of copyright on this source code product does not indicate 
#  publication.
#
#	(c) 1986,1987,1988.1989  Sun Microsystems, Inc
#	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
#          All rights reserved.
# 

ROOT =
INC = $(ROOT)/usr/include
CFLAGS = -O  -Kpic -I$(INC)
STRIP = strip
SIZE = size
LINT = lint
OBJECTS = ut.o utx.o doconfig.o checkver.o
LIBOBJECTS = ../ut.o ../utx.o ../doconfig.o ../checkver.o
SRCS = $(OBJS:%.o=%.c)

all:	$(OBJECTS)
	cp $(OBJECTS) ../

ut.o:	ut.c \
	$(INC)/sys/types.h \
	$(INC)/utmp.h \
	$(INC)/utmpx.h \
	$(INC)/ctype.h \
	$(INC)/unistd.h \
	$(INC)/sys/fcntl.h \
	$(INC)/sys/signal.h \
	$(INC)/sac.h \
	$(INC)/sys/param.h \
	$(INC)/sys/stat.h \
	$(INC)/errno.h \
	$(INC)/stdio.h

utx.o:	utx.c \
	$(INC)/sys/types.h \
	$(INC)/utmpx.h \
	$(INC)/ctype.h \
	$(INC)/unistd.h \
	$(INC)/sys/fcntl.h \
	$(INC)/sys/signal.h \
	$(INC)/sac.h \
	$(INC)/sys/param.h \
	$(INC)/sys/stat.h \
	$(INC)/errno.h \
	$(INC)/stdio.h

doconfig.o:	doconfig.c \
		$(INC)/stdio.h \
		$(INC)/fcntl.h \
		$(INC)/ctype.h \
		$(INC)/sys/types.h \
		$(INC)/stropts.h \
		$(INC)/sys/conf.h \
		$(INC)/sys/errno.h \
		$(INC)/signal.h \
		$(INC)/sac.h \
		$(INC)/signal.h

checkver.o:	checkver.c \
		$(INC)/stdio.h


lint:
	$(LINT) $(CFLAGS) $(SRCS)

clean:
	rm -f $(OBJECTS)

clobber: clean
	rm -f $(LIBOBJECTS)

size: all
	$(SIZE) $(LIBOBJECTS)

strip: all
	$(STRIP) $(LIBOBJECTS)
