/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)librpc:svc_run.c	1.2"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*	PROPRIETARY NOTICE (Combined)
*
* This source code is unpublished proprietary information
* constituting, or derived under license from AT&T's UNIX(r) System V.
* In addition, portions of such source code were derived from Berkeley
* 4.3 BSD under license from the Regents of the University of
* California.
*
*
*
*	Copyright Notice 
*
* Notice of copyright on this source code product does not indicate 
*  publication.
*
*	(c) 1986,1987,1988.1989  Sun Microsystems, Inc
*	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
*          All rights reserved.
*/ 
#if !defined(lint) && defined(SCCSIDS)
static char sccsid[] = "@(#)svc_run.c 1.9 89/02/28 Copyr 1984 Sun Micro";
#endif

/*
 * This is the rpc server side idle loop
 * Wait for input, call server program.
 */
#include <rpc/rpc.h>
#include <sys/errno.h>
#ifdef SYSLOG
#include <sys/syslog.h>
#else
#define LOG_ERR 3
#endif /* SYSLOG */

void
svc_run()
{
	fd_set readfds;
	extern int errno;

	for (;;) {
		readfds = svc_fdset;
		switch (select(_rpc_dtbsize(), &readfds, (fd_set *)0, (fd_set *)0,
			       (struct timeval *)0)) {
		case -1:
			if (errno == EINTR) {
				continue;
			}
			(void) syslog(LOG_ERR, "svc_run: select failed: %m");
			return;
		case 0:
			continue;
		default:
			svc_getreqset(&readfds);
		}
	}
}
