/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)curses:screen/wtimeout.c	1.3"
#include	"curses_inc.h"

void
wtimeout(win,tm)
WINDOW	*win;
int	tm;
{
    win->_delay = tm;
}
