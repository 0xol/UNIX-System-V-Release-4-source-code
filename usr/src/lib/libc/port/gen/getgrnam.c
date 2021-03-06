/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libc-port:gen/getgrnam.c	1.13"
/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#ifdef __STDC__
	#pragma weak getgrnam = _getgrnam
#endif
#include "synonyms.h"
#include <grp.h>
#include <stddef.h>

extern struct group *getgrent();
extern void setgrent(), endgrent();
extern int strcmp();

struct group *
getgrnam(name)
register const char *name;
{
	register struct group *p;

	setgrent();
	while((p = getgrent()) != NULL && strcmp(p->gr_name, name))
		;
	endgrent();
	return(p);
}
