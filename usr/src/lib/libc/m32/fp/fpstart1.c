/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libc-m32:fp/fpstart1.c	1.4.1.13"


/* establish the default settings for the floating-point state
 * for a C language program:
 *	integer overflow	-- masked off by default by OS.
 *	rounding mode		-- round to nearest default by OS,
 *	exceptions enabled	-- all masked by default by OS
 *	sticky bits		-- all clear by default by OS.
 * set the variable _fp_hw according to what floating-point hardware
 * is available.
 * The value of _fp_hw MUST be established before any of the fpset*()
 * library routines are called.
 */
#include	"synonyms.h"
#include	<sys/types.h>
#include	<sys/sys3b.h>	/* for S3BFPHW definition	*/
#include	<signal.h>	/* for SIGFPE definition	*/

#define	P754_NOFAULT	1	/* avoid forcing FULL fault env.*/
#include	<ieeefp.h>	/* for fp_except defs.		*/

#define		MAUVERS	0xe	/* to get bits 1,2,3 of mau ASR 
				 * determines which version of
				 * MAU is attached
				 */
int      			_fp_hw; /* default: bss: 0 == no hardware  */
int      			_asr;   /* initialized below               */
int				__flt_rounds; /* ANSI rounding mode */

/* allocate space (value immaterial) for FP fault variables	*/

fp_ftype         		_fpftype;
struct fp_fault *        	_fpfault;


 void
_fpstart()
    {
    int tasr;	/* temp ASR. WARNING: used in asm() as 0(%fp)   */

    static void	my_signal(), my_sys3b(); /* avoid external refs */

    __flt_rounds = 1; /*initialize ANSI rounding mode to rnd-to-near */

    /* the following call to my_sys3b() may generate SIGSYS on
     * some 3B2 systems. Change the handling of SIGSYS to SIG_IGN
     * and then change it back to SIG_DFL. Since this code runs
     * before all user code, there cannot be a handler.
     */
    my_signal( SIGSYS, SIG_IGN );
    my_sys3b( S3BFPHW, &_fp_hw ); /* query OS for HW status*/
    my_signal( SIGSYS, SIG_DFL );

    /* At this point the hardware environment (established by UNIX) is:
     *	round to nearest, all sticky bits clear,
     *	all exceptions masked.
     * The software environment (_asr being in .bss, set to 0) is:
     *	round to nearest, all sticky bits clear,
     *	all exceptions masked.
     */

    /* This release of the SGS does not support the generation of
     * diagnostic information in quiet NaNs by software.  Disable
     * the hardware trap also, in case the OS is trying to do
     * quiet NaN generation for us.
     * When writing the ASR, the <CSC> bit must always be forced ON,
     * to tell the kernel that we have used the MAU.
     */
    if ( _fp_hw != 0 ) {
	asm("	mmovfa	0(%fp)	");
	if ((tasr & MAUVERS) != 0)  /* MAU is 32206 	*/
		_fp_hw = 2;
	tasr &= ~(1<<24);	/* turn off <NTNC> bit	*/
	tasr |=  (1<<17);	/* turn on <CSC> bit	*/
	asm("	mmovta	0(%fp)	");
	}
    }

/* the following routines are shrunk-down versions of the libc
 * system call routines for signal() and sys3b().
 * Writing them here does not require every floating-point program
 * to carry the full routines (signal() in particular is LARGE)
 * around with them.
 *
 * NOTE: they are defined here, instead of in-line in _fpstart()
 * because the kernel expects %ap to be pointing at the arguments.
 * This way does that automatically.
 */
/*ARGSUSED*/
 static void
my_signal( signo, handlr )
    int		signo;		/* signal number	*/
    int		(*handlr)();	/* signal handler	*/
    {
    /* define GATE entry vector number in assembler	*/
    asm("	.set	SIGNAL,48*8	");

    asm("	movw	&4,%r0		");
    asm("	movw	&SIGNAL,%r1	");
    asm("	GATE			");
    }

/*ARGSUSED*/
 static void
my_sys3b( cmd,	arg )
    int		cmd;
    int *	arg;
    {
    /* define GATE entry vector number in assembler	*/
    asm("	.set	SYS3B,50*8	");

    asm("	movw	&4,%r0		");
    asm("	movw	&SYS3B,%r1	");
    asm("	GATE			");
    }
