/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_TIMES_H
#define _SYS_TIMES_H

#ident	"@(#)head.sys:sys/times.h	11.6"
/*
 * Structure returned by times()
 */
struct tms {
	clock_t	tms_utime;		/* user time */
	clock_t	tms_stime;		/* system time */
	clock_t	tms_cutime;		/* user time, children */
	clock_t	tms_cstime;		/* system time, children */
};

#if defined(__STDC__) && !defined(_KERNEL)
clock_t times(struct tms *);
#endif

#endif	/* _SYS_TIMES_H */
