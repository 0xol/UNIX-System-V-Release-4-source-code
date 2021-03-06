/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_CLOCK_H
#define _SYS_CLOCK_H

#ident	"@(#)head.sys:sys/clock.h	11.3"
 
#define	SECHR	(60*60)	/* seconds/hr */
#define	SECDAY	(24*SECHR)	/* seconds/day */
#define	SECYR	(365*SECDAY)	/* seconds/common year */

#if defined(__STDC__)

void delay(long);

#else

void delay();

#endif

#endif	/* _SYS_CLOCK_H */
