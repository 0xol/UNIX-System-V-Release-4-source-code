/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef _SYS_DSD_H
#define _SYS_DSD_H

#ident	"@(#)head.sys:sys/dsd.h	11.2"
/********************************************************/
/*	dsd.h - header file for Com I/O DSD command	*/
/*		and fl_cons floating console structure  */
/********************************************************/


/********************************************************/
/*	structure used for DSD command			*/
/********************************************************/

#define CODE_MAX 16	/* maximum subdevices per device */


typedef struct {
	unsigned short num_sbdev;		/* number of subdevices */
	unsigned short sbdev_code[CODE_MAX];	/* subdevice code array
						 * for peripheral devices */
	}DSD_POLL;


/********************************************************/
/* 	fl_cons structure for floating console data     */
/********************************************************/

struct fl_cons {
	unsigned short cflags;		/* baud rate, parity settings	*/
	unsigned char device;		/* slot # for console device	*/
	unsigned char port;		/* port (minor device) # for console */
	unsigned char cons_found;	/* zero = console terminal not found */
					/* one = console terminal found */
	};

#endif	/* _SYS_DSD_H */
