/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libsocket:accept.c	1.9"

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 		PROPRIETARY NOTICE (Combined)
 * 
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 * 
 * 
 * 
 * 		Copyright Notice 
 * 
 * Notice of copyright on this source code product does not indicate 
 * publication.
 * 
 * 	(c) 1986,1987,1988.1989  Sun Microsystems, Inc
 * 	(c) 1983,1984,1985,1986,1987,1988,1989  AT&T.
 * 	          All rights reserved.
 *  
 */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/mkdev.h>
#include <sys/errno.h>
#include <sys/stream.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/stropts.h>
#include <sys/tihdr.h>
#include <sys/sockmod.h>
#include <sys/socketvar.h>
#include <sys/socket.h>
#include <sys/tiuser.h>
#include <sys/signal.h>
#include <sys/stat.h>
#ifdef DEBUG
#include <stdio.h>
#endif


extern int	errno;

int
accept(s, addr, addrlen)
	register int			s;
	register struct sockaddr	*addr;
	register int			*addrlen;

{
	register struct _si_user	*siptr;

	if ((siptr = _s_checkfd(s)) == NULL)
		return -1;

	if (siptr->udata.servtype == T_CLTS) {
		errno = EOPNOTSUPP;
		return -1;
	}

	/* make sure a listen() has been done
	 * actually if the accept() has not been done, then the
	 * effect will be that the user blocks forever.
	 */
	if ( !(siptr->udata.so_options & SO_ACCEPTCONN)) {
		errno = EINVAL;
		return -1;
	}

	return _accept(siptr, addr, addrlen);
}

int
_accept(siptr, addr, addrlen)
	register struct _si_user	*siptr;
	register struct sockaddr	*addr;
	register int			*addrlen;
{

	register struct T_conn_res	*cres;
	register struct _si_user	*nsiptr;
	register int			s;
	register int			s2;
	int				retval;
	register void			(*sigsave)();
	register struct netconfig	*net;
	register dev_t			needev;
	register union T_primitives	*pptr;
	struct strfdinsert		strfdinsert;
	int				flg;
	struct stat			statd;
	struct strbuf			ctlbuf;
	void				*nethandle;

	flg = 0;
	s = siptr->fd;

	/* get/wait for the T_CONN_IND.
	 */
	ctlbuf.maxlen = siptr->ctlsize;
        ctlbuf.len = 0;
        ctlbuf.buf = siptr->ctlbuf;
 
	/* we don't expect any data, so no data
	 * buffer is needed.
	 */
        if ((retval = getmsg(s, &ctlbuf, NULL, &flg)) < 0) {
                if (errno == EAGAIN)
                        errno = EWOULDBLOCK;
                return -1;
        }
        /*
         * did I get entire message?
         */
        if (retval) {
                errno = EIO;
                return -1;
        }
 
        /*
         * is ctl part large enough to determine type
         */
        if (ctlbuf.len < sizeof(long)) {
                errno = EPROTO;
                return -1;
        }
 
        pptr = (union T_primitives *)ctlbuf.buf;
        switch(pptr->type) {
                case T_CONN_IND:
                        if (ctlbuf.len < (sizeof(struct T_conn_ind)+
				pptr->conn_ind.SRC_length)) {
                                errno = EPROTO;
                                return -1;
                        }
			if (addr && addrlen) {
				if (*addrlen > pptr->conn_ind.SRC_length)
					*addrlen = pptr->conn_ind.SRC_length;
				(void)memcpy(addr, ctlbuf.buf+pptr->conn_ind.SRC_offset, *addrlen);
			}
			break;
 
                default:         
			errno = EPROTO;
			return -1;
	}

	/* get the device file to open.
	 */
	if (fstat(s, &statd) < 0)
		return -1;
	needev = major(statd.st_rdev);

	/* loop through each entry in netconfig
	 * until one matches.
	 */
	if ((nethandle = setnetconfig()) == NULL) {
#ifdef DEBUG
		nc_perror("_accept: setnetconfig failed");
#endif
		return -1;
	}
	while ((net = getnetconfig(nethandle)) != NULL) {
		if (net->nc_semantics != NC_TPI_COTS &&
			net->nc_semantics != NC_TPI_COTS_ORD)
			continue;
		if (stat(net->nc_device, &statd) < 0)
			continue;
		if (minor(statd.st_rdev) == needev)
			break;
	}
	if (net == NULL) {
		endnetconfig(nethandle);
		errno = ENODEV;
		return -1;
	}

	/* open a new instance to do the accept on
	 *
	 * Note that we have lost the protocol number(if one was
	 * specified) with the listening endpoint, so we assume the
	 * transport provider makes a good enough copy of all the
	 * endpoints details, when the new one is created.
	 */
	if ((nsiptr = _s_open(net->nc_device, 0)) == NULL) {
		endnetconfig(nethandle);
		return -1;
	}
	endnetconfig(nethandle);

	s2 = nsiptr->fd;

	/* must be bound for TLI.
	 */
	if (_bind(nsiptr, NULL, 0, NULL, NULL) < 0)
		return -1;

	cres = (struct T_conn_res *)siptr->ctlbuf;
	cres->PRIM_type = T_CONN_RES;
	cres->OPT_length = 0;
	cres->OPT_offset = 0;
	cres->SEQ_number = pptr->conn_ind.SEQ_number;

	strfdinsert.ctlbuf.maxlen = siptr->ctlsize;
	strfdinsert.ctlbuf.len = sizeof(*cres);
	strfdinsert.ctlbuf.buf = (caddr_t)cres;

	strfdinsert.databuf.maxlen = 0;
	strfdinsert.databuf.len = -1;
	strfdinsert.databuf.buf = NULL;

	strfdinsert.fildes = s2;
	strfdinsert.offset = sizeof(long);
	strfdinsert.flags = 0;

	sigsave = sigset(SIGPOLL, SIG_HOLD);
	if (ioctl(s, I_FDINSERT, &strfdinsert) < 0) {
		sigset(SIGPOLL, sigsave);
		return -1;
	}

	if (!_s_is_ok(siptr, T_CONN_RES)) {
		sigset(SIGPOLL, sigsave);
		return -1;
	}

	sigset(SIGPOLL, sigsave);

	nsiptr->udata.so_state |= SS_ISCONNECTED;

	return s2;
}



