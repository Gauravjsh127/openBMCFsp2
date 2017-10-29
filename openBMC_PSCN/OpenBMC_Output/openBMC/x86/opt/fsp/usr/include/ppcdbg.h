/******************************************************************************
 *
 * IBM Confidential
 *
 * Licensed Internal Code Source Materials
 *
 * IBM Flexible Support Processor Licensed Internal Code
 *
 * (c) Copyright IBM Corp. 2007
 *
 * The source code is for this program is not published or otherwise divested
 * of its trade secrets, irrespective of what has been deposited with the
 * U.S. Copyright Office.
 *
 *****************************************************************************/

#ifndef __PPCDBG_H__
#define __PPCDBG_H__

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PPCDBG_VERSION	      6

/*
 * ppcdbg_version - returns version of this library. This should match
 *		    the DBG_VERSION macro, otherwise you have compiled
 *		    your code against a different library. You can try,
 *		    but expect problems.
 */
int ppcdbg_version(void);

/*
 * Register definitions important for autoprobing and initial processor
 * setup independent from using a PPC40x or PPC44x.
 */
#define PPCDBG_SPRN_PVR	      0x11f /* processor version register */

#define PPCDBG_ERR_OK		0 /* No error occured */
#define PPCDBG_ERR_UNKNOWN	101 /* Unknown, generic error condition */
#define PPCDBG_ERR_TIMEOUT	102 /* Timeout error */
#define PPCDBG_ERR_FILEIO	103 /* File I/O error, see errno */
#define PPCDBG_ERR_INVAL	104 /* Invalid parameters */
#define PPCDBG_ERR_GETHOSTBYNAME 105 /* Error using gethostbyname */
#define PPCDBG_ERR_SOCKET	106 /* Error creating a socket */
#define PPCDBG_ERR_CONNECT	107 /* Error on connect */
#define PPCDBG_ERR_READ		108 /* Read error */
#define PPCDBG_ERR_WRITE	109 /* Write error */
#define PPCDBG_ERR_DEBUGGER	110 /* Debugger returned error */
#define PPCDBG_ERR_WSIZE	111 /* Wrong size field returned by dbg */
#define PPCDBG_ERR_WFILLUP	112 /* Wrong fillup field returned by dbg */
#define PPCDBG_ERR_AUTOPROBE	113 /* Can happen with old probes */
#define PPCDBG_ERR_RDATA_AVAIL	114 /* Data for reading avail - err? */
#define PPCDBG_ERR_WTIMEOUT	115 /* Write timeout */
#define PPCDBG_ERR_NODATA	116 /* No data available but expected */
#define PPCDBG_ERR_NOFUNC	117 /* Function not avail for this dbg dev */
#define PPCDBG_ERR_NOTSTOPPED	118 /* Operation needed proc stopped */
#define PPCDBG_ERR_UNSPECIFIED	119 /* Unspecified error condition */
#define PPCDBG_ERR_ALIGN	120 /* Alignement error */
#define PPCDBG_ERR_ALIGN	120 /* Alignment error */
#define PPCDBG_ERR_NOPOWER	121 /* Target has no power */
#define PPCDBG_ERR_NOMEM	122 /* Out of memory */
#define PPCDBG_ERR_NORESPONSE	123 /* Target not responding */
#define PPCDBG_ERR_MAX		124 /* No error number should be larger */

typedef struct ppcdbg_generic *ppcdbg_t;

typedef struct {
	int (* scanir)(void *priv_data, unsigned int num_bits,
		       unsigned int *input, unsigned int *output);
	int (* scandr)(void *priv_data, unsigned int num_bits,
		       unsigned int *input, unsigned int *output);
	int (* halt_set)(void *priv_data, int on);
	int (* halt_get)(void *priv_data, int *on); /* FIXME BUG! */
	int (* reset_set)(void *priv_data, int on);
	int (* power_get)(void *priv_data, int *power);

	/* FIXME are here more functions to be provided?
	 *       Callbacks which are NULL should drop to reasonable defaults.
	 *
	 * Possible candidates:
	 *   reset_set();
	 */
} ppcdbg_callback_t;

/*
 * There are different ways to get the ppcdbg_t handle depending on
 * the method you have choosen to access your PPC device. Currently
 * the library provides to following support:
 *
 *   1. RISCWatch access via network
 *   2. ScanIrDr via network
 *   3. ScanIrDr using the JTAG driver & GPIO functionality
 */
int ppcdbg_open_irdrnet(ppcdbg_t *dbg, const char *ipaddr, unsigned short port);

#if defined(__PPC) /* && defined(CONFIG_NATIVE_JTAG) */

/*
 * Use the JTAG method to debug your PPC.
 */
int ppcdbg_open_jtag(ppcdbg_t *dbg, const ppcdbg_callback_t *callback,
		     void *priv_data);

#endif

int ppcdbg_open_rw(ppcdbg_t *dbg, const char *ipaddr, unsigned short port);

#define PPCDBG_SPEED_AUTOPROBE	0 /* 0 is autoprobe, otherwise MHz */
#define PPCDBG_SPEED_1MHz	1
#define PPCDBG_SPEED_2MHz	2
#define PPCDBG_SPEED_3MHz	3
#define PPCDBG_SPEED_4MHz	4
#define PPCDBG_SPEED_5MHz	5
#define PPCDBG_SPEED_10MHz	10

int ppcdbg_init(ppcdbg_t dbg, int speed);
int ppcdbg_speed(ppcdbg_t dbg, int speed);

/*
 * Procedure to do the autoprobing:
 *   1. check if target has power
 *   2. initialize debugger
 *   3. read PVR
 *   4. run this function
 */
int ppcdbg_speed_autoprobe(ppcdbg_t dbg, unsigned long pvr, int *new_speed);

int ppcdbg_exec_command(ppcdbg_t dbg, const char *cmd, int *dbg_errno);
int ppcdbg_readl(ppcdbg_t dbg, unsigned long addr, unsigned long *val);
int ppcdbg_writel(ppcdbg_t dbg, unsigned long addr, unsigned long val);
int ppcdbg_reads(ppcdbg_t dbg, unsigned long addr, unsigned short *val);
int ppcdbg_writes(ppcdbg_t dbg, unsigned long addr, unsigned short val);
int ppcdbg_readb(ppcdbg_t dbg, unsigned long addr, unsigned char *val);
int ppcdbg_writeb(ppcdbg_t dbg, unsigned long addr, unsigned char val);
ssize_t ppcdbg_read(ppcdbg_t dbg, unsigned long addr, void *buf, size_t count);
ssize_t ppcdbg_write(ppcdbg_t dbg, unsigned long addr, const void *buf,
		     size_t count);
int ppcdbg_stuff(ppcdbg_t dbg, const unsigned long *instr, int count,
		 int *instr_executed);

int ppcdbg_mfdcr(ppcdbg_t dbg, int reg, unsigned long *val);
int ppcdbg_mtdcr(ppcdbg_t dbg, int reg, unsigned long val);
int ppcdbg_mfspr(ppcdbg_t dbg, int reg, unsigned long *val);
int ppcdbg_mtspr(ppcdbg_t dbg, int reg, unsigned long val);
int ppcdbg_mfmsr(ppcdbg_t dbg, unsigned long *val);
int ppcdbg_mtmsr(ppcdbg_t dbg, unsigned long val);
int ppcdbg_mfgpr(ppcdbg_t dbg, int reg, unsigned long *val);
int ppcdbg_mtgpr(ppcdbg_t dbg, int reg, unsigned long val);
int ppcdbg_mfiar(ppcdbg_t dbg, unsigned long *iar);
int ppcdbg_mtiar(ppcdbg_t dbg, unsigned long iar);

int ppcdbg_halt_on(ppcdbg_t dbg);
int ppcdbg_halt_off(ppcdbg_t dbg);
int ppcdbg_reset_on(ppcdbg_t dbg);
int ppcdbg_reset_off(ppcdbg_t dbg);

int ppcdbg_run(ppcdbg_t dbg);
int ppcdbg_stop(ppcdbg_t dbg);
int ppcdbg_step(ppcdbg_t dbg, unsigned long steps);

#define PPCDBG_STATUS_UNDEFINED		-1
#define PPCDBG_STATUS_UNKNOWN		0
#define PPCDBG_STATUS_RUNNING		1
#define PPCDBG_STATUS_STOPPED		2
#define PPCDBG_STATUS_CHECKSTOPPED	3
#define PPCDBG_STATUS_POWEROFF		4
#define PPCDBG_STATUS_HALTED		5

int ppcdbg_status(ppcdbg_t dbg, int *status);

#define PPCDBG_RST_CORE		1
#define PPCDBG_RST_CHIP		2
#define PPCDBG_RST_SYSTEM	3

int ppcdbg_reset(ppcdbg_t dbg, int rst_type);	     /* reset via JTAG */

/* It was discussed to where the abstraction of the differences between
 * the processors should be handled. One solution would have been to
 * make the libppcdbg know what the processor is, so that programs using
 * the library don't have to care about what processor is used.
 * The other, implemented, solution was to make seperate functions for
 * the different processors and let the application do the abstraction.
 * */
int ppcdbg_405_bp_set(ppcdbg_t dbg, unsigned long addr);
int ppcdbg_405_bp_clear(ppcdbg_t dbg, unsigned long addr);
int ppcdbg_405_bp_clearall(ppcdbg_t dbg);
int ppcdbg_440_bp_set(ppcdbg_t dbg, unsigned long addr);
int ppcdbg_440_bp_clear(ppcdbg_t dbg, unsigned long addr);
int ppcdbg_440_bp_clearall(ppcdbg_t dbg);
int ppcdbg_405_freeze_timers(ppcdbg_t dbg);
int ppcdbg_440_freeze_timers(ppcdbg_t dbg);

/*
 * Debug and tweaking flags for performance. May affect reliability.
 */
#define DBG_SLOW_JTAG	 0x00000001	     /* slow JTAG -> reduce polling */

int ppcdbg_set_flags(ppcdbg_t dbg, unsigned long flags);

/*
 * @param tlbs need to be a table of DBG_PPC405_TLBS entries
 *
 * tlbre RT, RA, RW		   TLB Read Entry
 * if WS4 = 1
 *   (RT)  <- TLBLO[(RA26:31)]
 * else
 *   (RT)  <- TLBHI[(RA26:31)]
 *   (PID) <- TID from TLB[(RA26:31)]
 */
typedef struct {
	unsigned long entry[2];
	unsigned long pid;
} ppcdbg_405tlb_t;

#define PPCDBG_405_TLBS 64

int ppcdbg_read405tlbs(ppcdbg_t dbg, int start, int nr,
		       ppcdbg_405tlb_t *tlbs);

/*
 * @param tlbs need to be a table of DBG_PPC440_TLBS entries
 * NOTE The 440GX has already parity bits which can be read
 * out as well if CCR0[CRPE] is set.
 *
 * tlbre RT, RA, WS		   TLB Read Entry
 * tlbentry	 <-  TLB[(RA)26:31]
 * if WS =0
 *   (RT)0:27	 <- tlbentry[EPN,V,TS,SIZE]
 *   if CCR0[CRPE] = 0
 *     (RT)28:31 <- 40
 *   else
 *     (RT)28:31 <- TPAR
 *   MMUCR[STID] <- tlbentry[TID]
 * else if WS = 1
 *   (RT)0:21	 <- tlbentry[RPN]
 *   if CCR0[CRPE] = 0
 *     (RT)22:23 <- 20
 *   else
 *     (RT)22:23 <- PAR1
 *   (RT)24:27	 <- 40
 *   (RT)28:31	 <- tlbentry[ERPN]
 * else if WS = 2
 *   if CCR0[CRPE] = 0
 *     (RT)0:1	 <- 20
 *   else
 *     (RT)0:1	 <- PAR2
 *   (RT)2:15	 <- 140
 *   (RT)16:24	 <- tlbentry[U0,U1,U2,U3,W,I,M,G,E]
 *   (RT)25	 <- 0
 *   (RT)26:31	 <- tlbentry[UX,UW,UR,SX,SW,SR]
 * else
 *   (RT), MMUCR[STID] undefined
 *
 * MSB				       LSB
 *	       11.1111.1111.2222.2222.2233 IBM	 (MSB bit 0)
 * 0123.4567.8901.2345.6789.0123.4567.8901
 */
#define DBG_E0_PARITY_MSK 0x0000000f
#define DBG_E1_PARITY_MSK 0x00000300
#define DBG_E2_PARITY_MSK 0xc0000000

typedef struct {
	unsigned long entry[3];
	unsigned long mmucr;
} ppcdbg_440tlb_t;

#define PPCDBG_440_TLBS 64

int ppcdbg_read440tlbs(ppcdbg_t dbg, int start, int nr,
			   ppcdbg_440tlb_t *tlbs);
int ppcdbg_write440tlbs(ppcdbg_t dbg, int start, int nr,
			const ppcdbg_440tlb_t *tlbs);

int ppcdbg_close(ppcdbg_t dbg);

void ppcdbg_verbosity(int verbosity);

/*
 * Specific funtionality only available for FSPs. How it is
 * implemented is rather an agreement than something PowerPC embedded
 * specific.
 *
 * FIXME Consider to move it to a destinct library e.g. libbuddyjtag.
 */
int ppcdbg_buddy_read_disr(ppcdbg_t dbg, unsigned long *disr);
int ppcdbg_buddy_panic(ppcdbg_t dbg);

#ifdef	__cplusplus
}
#endif
#endif /* __PPCDBG_H__ */
