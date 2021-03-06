/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)curses:screen/V3.waddch.c	1.1"

#include	"curses_inc.h"
extern	int	_outchar();

#ifdef	_VR3_COMPAT_CODE
#undef	waddch
waddch(win, c)
WINDOW		*win;
_ochtype	c;
{
    return (w32addch(win, _FROM_OCHTYPE(c)));
}
#endif	/* _VR3_COMPAT_CODE */
