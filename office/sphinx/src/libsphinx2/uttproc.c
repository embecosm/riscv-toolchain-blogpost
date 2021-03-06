/* ====================================================================
 * Copyright (c) 1995-2000 Carnegie Mellon University.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The names "Sphinx" and "Carnegie Mellon" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. To obtain permission, contact 
 *    sphinx@cs.cmu.edu.
 *
 * 4. Products derived from this software may not be called "Sphinx"
 *    nor may "Sphinx" appear in their names without prior written
 *    permission of Carnegie Mellon University. To obtain permission,
 *    contact sphinx@cs.cmu.edu.
 *
 * 5. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Carnegie
 *    Mellon University (http://www.speech.cs.cmu.edu/)."
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */

/*
 * uttproc.c -- Process utterance.
 * 
 * HISTORY
 * 
 * $Log: uttproc.c,v $
 * Revision 1.7  2001/02/13 19:51:38  lenzo
 * *** empty log message ***
 *
 * Revision 1.6  2001/01/25 19:36:28  lenzo
 * Fixing some memory leaks
 *
 * Revision 1.5  2000/12/21 18:04:51  lenzo
 * Fixed a nasty (but small) FRAME_RATE error.  This will need cleanup later.
 *
 * Revision 1.4  2000/12/12 23:01:42  lenzo
 * Rationalizing libs and names some more.  Split a/d and fe libs out.
 *
 * Revision 1.3  2000/12/05 01:45:12  lenzo
 * Restructuring, hear rationalization, warning removal, ANSIfy
 *
 * Revision 1.2  2000/02/08 20:44:32  lenzo
 * Changed uttproc_allphone_cepfile() to uttproc_allphone_file.
 *
 * Revision 1.1.1.1  2000/01/28 22:08:58  lenzo
 * Initial import of sphinx2
 *
 * 09-Jan-00    Kevin Lenzo <lenzo@cs.cmu.edu> at Carnegie Mellon
 *              Altered to accomodate new fe lib.
 * 
 * 30-Oct-98	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Changed rawlogfile mode to READONLY (WIN32).
 * 
 * 10-Sep-98	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Reset uttno to 0 whenever uttproc_set_auto_uttid_prefix() is called.
 * 
 * 10-Sep-98	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added uttproc_allphone_cepfile(), and minor modifications to support it.
 * 
 * 20-Aug-98	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Bugfix: 
 * 		Added call to agc_emax_update() inside uttproc_end_utt().  Added call
 * 		to initialize AGC with a reasonable value.
 * 
 * 20-Apr-98	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added uttproc_set_auto_uttid_prefix().
 * 
 * 11-Apr-98	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added AGC_NONE test to determining livemode in uttproc_begin_utt().
 * 		Added memcpy to mfc2feat_live if AGC_NONE (bugfix).
 * 
 * 22-Jul-97	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added sampling rate spec in call to fe_init.
 * 
 * 27-May-97	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added uttprocSetcomp2rawfr() and uttprocGetcomp2rawfr() functions
 * 		implemented by Bob Brennan for maintaining multiple lattices.
 * 
 * 04-Apr-97	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added dictwd_in_lm() check in uttproc_set_context.
 * 
 * 30-Oct-96	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Commented out call to search_dump_lattice_ascii.
 * 		Added feature vector padding in mfc2feat_batch ().
 * 
 * 17-Jun-96	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added uttproc_set_context().
 * 
 * 04-Jun-96	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added BLOCKING option to uttproc_rawdata, uttproc_cepdata, uttproc_result.
 * 
 * 24-May-96	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Substantially modified to be driven with externally provided data, rather
 * 			than explicitly reading an A/D source.
 * 		Added uttproc_abort_utt() and uttproc_partial_result().
 * 		Added raw and mfc logging function.
 * 
 * 17-Nov-95	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added function uttproc_lmupdate().
 * 
 * 17-Nov-95	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Fixed bug in uttproc_feat2rawfr that could return feat2rawfr[-1].
 * 
 * 29-Sep-95	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added -matchsegfn argument and processing.
 * 
 * 17-Sep-95	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added autonumbering of utterances (typically used in live mode).
 * 
 * 02-Jul-95	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Added allphone handling.
 * 
 * 13-Jun-95	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 * 		Simplified the uttproc interface by combining functions and redefining
 * 		others.
 * 
 * 01-Jun-95	M K Ravishankar (rkm@cs) at Carnegie Mellon University
 *		Added uttproc_set_lm() and uttproc_set_startword().
 */


/*
 * BUGS:
 *   - Instead of using query_fwdtree_flag() to determine which first pass to run
 *     (tree or flag), there should be an explicit uttproc_set_firstpass() call.
 */


#include <stdio.h>
#include <stdlib.h>
#ifdef SUN4
#include <unistd.h>
#endif
#include <time.h>

#ifdef WIN32
#include <io.h>
#include <windows.h>
#include <time.h>
#include <sys/stat.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include <assert.h>

#include "s2types.h"
#include "CM_macros.h"
#include "basic_types.h"
#include "err.h"
#include "scvq.h"
#include "search_const.h"
#include "msd.h"
#include "strfuncs.h"
#include "linklist.h"
#include "list.h"
#include "hash.h"
#include "dict.h"
#include "lmclass.h"
#include "lm_3g.h"
#include "kb.h"
#include "cdcn.h"
#include "fe.h"
#include "fbs.h"
#include "search.h"
#include "logmsg.h"

#define MAX_UTT_LEN	6000	/* #frames */
#define MAX_CEP_LEN	(MAX_UTT_LEN*CEP_SIZE)
#define MAX_POW_LEN	(MAX_UTT_LEN*POW_SIZE)

static int32 frame_spacing;

typedef enum {UTTSTATE_UNDEF=-1,
	      UTTSTATE_IDLE=0,
	      UTTSTATE_BEGUN=1,
	      UTTSTATE_ENDED=2,
	      UTTSTATE_STOPPED=3
} uttstate_t;
static uttstate_t uttstate = UTTSTATE_UNDEF;


static int32 inputtype;
#define INPUT_UNKNOWN	0
#define INPUT_RAW	1
#define INPUT_MFC	2

static int32 livemode;		/* Iff TRUE, search while input being supplied.  In this
				   case, CMN, AGC and silence compression cannot be
				   utterance based */
static int32 utt_ofl;		/* TRUE iff buffer limits overflowed in current utt */
static int32 nosearch = 0;

/* MFC vectors for entire utt */
static float **mfcbuf;
static int32 n_rawfr;		/* #raw frames before compression or feature computation */

/* Feature vectors for entire utt */
static float *cep_buf = NULL;
static float *dcep_buf;
static float *dcep_80ms_buf;
static float *pcep_buf;
static float *ddcep_buf;
static int32 n_featfr;		/* #features frames */
static int32 n_compfr;		/* #compressed frames */
static int32 n_searchfr;
static int16 *comp2rawfr;	/* Compressed frame no. to original raw frame no. */

static int32 pow_i, cep_i;	/* #feature frames total in current utt so far */
static int32 search_cep_i, search_pow_i;	/* #frames already searched */

/* CMN, AGC, silence compression; default values */
static scvq_norm_t cmn = NORM_UTT;
static scvq_agc_t agc = AGC_MAX;
static scvq_compress_t silcomp = COMPRESS_NONE;

static FILE *matchfp = NULL;
static FILE *matchsegfp = NULL;

static char *rawlogdir = NULL;
static char *mfclogdir = NULL;
static FILE *rawfp = NULL;
static FILE *mfcfp = NULL;
static char rawfilename[4096];

static int32 samp_hist[5];	/* #Samples in 0-4K, 4K-8K, 8K-16K, 16K-24K, 24K-32K */
static int32 max_samp;

static char *uttid;
static char *uttid_prefix = NULL;
#define UTTIDSIZE	4096
static int32 uttno;	/* A running sequence number assigned to every utterance.  Used as
			   an id for an utterance if uttid is undefined. */

static search_hyp_t *utt_seghyp = NULL;

static CDCN_type cdcn;

static float TotalCPUTime, TotalElapsedTime, TotalSpeechTime;

#ifdef WIN32
static float e_start, e_stop;
static HANDLE pid;
static FILETIME t_create, t_exit, kst, ket, ust, uet;
static double lowscale, highscale;
extern double win32_cputime();
#else
static struct rusage start, stop;
static struct timeval e_start, e_stop;
#endif

/* FIXME: These are all internal to this module, but still should go
   into internal header files... */

/* live_norm.c */
extern void mean_norm_init(int32 vlen);
extern void mean_norm_update(void);
extern void mean_norm_acc_sub(float *vec);
extern int32 cepmean_set (float *vec);
extern int32 cepmean_get (float *vec);

/* agc_emax.c */
void agc_emax_update ( void );
extern int32 agcemax_set (double m);
extern double agcemax_get ( void );
extern int agc_emax_proc (float *ocep, float const *icep, int veclen);

/* norm.c */
void norm_mean (float *vec, int32 nvec, int32 veclen);

/* r_agc_noise.c */
extern int32 delete_background (float *cep, int32 fcnt,
				int32 cf_cnt, double thresh);
extern float histo_noise_level (float *cep, int32 fcnt, int32 cf_cnt);
extern int32 histo_add_c0 (float c0);
void compute_noise_level (void);
void real_agc_noise(float *cep,
		    register int32 fcnt,
		    register int32 cf_cnt);
void agc_max(float *cep,
	     register int32 fcnt,
	     register int32 cf_cnt);

/* searchlat.c */
void searchlat_set_rescore_lm (char const *lmname);

#ifdef WIN32

double win32_cputime (FILETIME *st, FILETIME *et)
{
    double dt;
    
    dt = (et->dwLowDateTime - st->dwLowDateTime) * lowscale;
    dt += (et->dwHighDateTime - st->dwHighDateTime) * highscale;

    return (dt);
}

#else

double MakeSeconds (struct timeval const *s, struct timeval const *e)
/*------------------------------------------------------------------------*
 * Compute an elapsed time from two timeval structs
 */
{
    return ((e->tv_sec - s->tv_sec) + ((e->tv_usec - s->tv_usec) / 1000000.0));
}

#endif


/*
 * One time initialization
 */
static void timing_init ( void )
{
#ifdef WIN32
    lowscale = 1e-7;
    highscale = 65536.0 * 65536.0 * lowscale;

    pid = GetCurrentProcess();
#endif

    TotalCPUTime = TotalElapsedTime = TotalSpeechTime = 0.0;
}


/*
 * Start of each utterance
 */
static void timing_start ( void )
{
#ifndef WIN32
#ifndef _HPUX_SOURCE
    getrusage (RUSAGE_SELF, &start);
#endif
    gettimeofday (&e_start, 0);
#else /* WIN32 */
    e_start = (float)clock()/CLOCKS_PER_SEC;
    GetProcessTimes (pid, &t_create, &t_exit, &kst, &ust);
#endif /* WIN32 */
}


/*
 * End of each utterance
 */
static void timing_stop ( void )
{
    if (searchFrame() == 0)
	return;
    
    log_info(" %5.2f SoS", searchFrame()*0.01);
    TotalSpeechTime += searchFrame()*0.01;
    
#ifdef WIN32
    /* ---------------- WIN32 ---------------- */
    e_stop = (float)clock()/CLOCKS_PER_SEC;
    GetProcessTimes (pid, &t_create, &t_exit, &ket, &uet);
    
    log_info(", %6.2f sec elapsed", (e_stop - e_start));
    log_info(", %5.2f xRT", (e_stop - e_start)/(searchFrame()*0.01));
    log_info(", %6.2f sec CPU", win32_cputime(&ust, &uet));
    log_info(", %5.2f xRT", win32_cputime(&ust, &uet)/(searchFrame()*0.01));
    
    TotalCPUTime += win32_cputime(&ust, &uet);
    TotalElapsedTime += (e_stop - e_start);
#else
    /* ---------------- Unix ---------------- */
#ifndef _HPUX_SOURCE
    getrusage (RUSAGE_SELF, &stop);
#endif
    gettimeofday (&e_stop, 0);
    
    log_info(", %6.2f sec elapsed", MakeSeconds (&e_start, &e_stop));
    log_info(", %5.2f xRT", MakeSeconds (&e_start, &e_stop)/(searchFrame()*0.01));
    
#ifndef _HPUX_SOURCE
    log_info(", %6.2f sec CPU", MakeSeconds (&start.ru_utime, &stop.ru_utime));
    log_info(", %5.2f xRT",
	    MakeSeconds (&start.ru_utime, &stop.ru_utime)/(searchFrame()*0.01));
#endif
    
    TotalCPUTime += MakeSeconds (&start.ru_utime, &stop.ru_utime);
    TotalElapsedTime += MakeSeconds (&e_start, &e_stop);
#endif
    
    log_info("\n");
}


/*
 * One time cleanup before exiting program
 */
static void timing_end ( void )
{
    log_info("\n");

    log_info("TOTAL Elapsed time %.2f seconds\n",TotalElapsedTime);
#ifndef _HPUX_SOURCE
    log_info("TOTAL CPU time %.2f seconds\n", TotalCPUTime);
#endif
    log_info("TOTAL Speech %.2f seconds\n", TotalSpeechTime);

    if (TotalSpeechTime > 0.0) {
	log_info("AVERAGE %.2f xRT(Elapsed)", TotalElapsedTime/TotalSpeechTime);
#ifndef _HPUX_SOURCE
	log_info(", %.2f xRT(CPU)\n", TotalCPUTime/TotalSpeechTime);
#endif
	log_info("\n");
    }
}


static void feat_alloc ( void )
{
    int32 k;
    
    if (! cep_buf) {
	cep_buf       = (float *) CM_calloc (MAX_CEP_LEN, sizeof(float));
	dcep_buf      = (float *) CM_calloc (MAX_CEP_LEN, sizeof(float));
	dcep_80ms_buf = (float *) CM_calloc (MAX_CEP_LEN, sizeof(float));
	pcep_buf      = (float *) CM_calloc (MAX_POW_LEN, sizeof(float));
	ddcep_buf     = (float *) CM_calloc (MAX_CEP_LEN, sizeof(float));

	mfcbuf = (float **) CM_calloc (MAX_UTT_LEN+10, sizeof(float *));
	mfcbuf[0] = (float *) CM_calloc ((MAX_UTT_LEN+10)*CEP_SIZE, sizeof(float));
	for (k = 1; k < MAX_UTT_LEN+10; k++)
	    mfcbuf[k] = mfcbuf[k-1] + CEP_SIZE;
    }
}


int32 uttproc_get_featbuf (float **cep, float **dcep, float **dcep_80ms, float **pcep,
float **ddcep)
{
    *cep	= cep_buf;
    *dcep	= dcep_buf;
    *dcep_80ms	= dcep_80ms_buf;
    *pcep	= pcep_buf;
    *ddcep	= ddcep_buf;

    return n_featfr;
}


/*
 * Compute sphinx-II feature vectors (cep, dcep, ddcep, pow) from input melcep vector.
 * The input melcep vector has had mean normalization, agc, and silence compression
 * already applied to it.  Since dcep and ddcep look at cepstra from several adjacent
 * frames, no valid feature vectors are present for several frames (4) at either end
 * of each utterance.
 * Return 1 if a valid feature is computed for this input frame, 0 otherwise.
 */
static int32 compute_features(float *cep_o,
			      float *dcep_o,
			      float *dcep_80ms_o,
			      float *pcep_o,
			      float *ddcep_o,
			      float *mfcc)
{
    float *cep_in;
    float *dcep_in;
    float *dcep_80ms_in;
    float *pcep_in;
    float *ddcep_in;

    if (SCVQComputeFeatures(&cep_in, &dcep_in, &dcep_80ms_in, &pcep_in, &ddcep_in, mfcc)) {
        memcpy(cep_o,       cep_in,       sizeof(float) * CEP_SIZE);
        memcpy(dcep_o,      dcep_in,      sizeof(float) * CEP_SIZE);
        memcpy(dcep_80ms_o, dcep_80ms_in, sizeof(float) * CEP_SIZE);
        memcpy(pcep_o,      pcep_in,      sizeof(float) * POW_SIZE);
        memcpy(ddcep_o,     ddcep_in,     sizeof(float) * CEP_SIZE);

        return 1;
    } else
	return 0;
}


static void warn_notidle (char const *func)
{
    if (uttstate != UTTSTATE_IDLE)
	E_WARN("%s called when not in IDLE state\n", func);
}


static void mfc2feat_live_frame (float *incep, int32 rawfr)
{
    float cep[CEP_SIZE];
    
    if (cmn == NORM_PRIOR)
	mean_norm_acc_sub (incep);

    if (agc == AGC_EMAX)
	agc_emax_proc(cep, incep, CEP_SIZE);
    else {
        memcpy (cep, incep, CEP_SIZE*sizeof(float));
    }

    if ((! silcomp) || histo_add_c0 (cep[0])) {
	comp2rawfr[n_compfr++] = rawfr;
	
	if (compute_features(cep_buf + cep_i,
			     dcep_buf + cep_i,
			     dcep_80ms_buf + cep_i,
			     pcep_buf + pow_i,
			     ddcep_buf + cep_i,
			     cep)) {
	    cep_i += CEP_SIZE;
	    pow_i += POW_SIZE;

	    n_featfr++;
	}
    }
}


/* Convert all given mfc vectors to feature vectors */
static int32 mfc2feat_live (float **mfc, int32 nfr)
{
    int32 i;
    
    for (i = 0; i < nfr; i++, n_rawfr++)
	mfc2feat_live_frame (mfc[i], n_rawfr);

    return 0;
}


static int32 cmn_batch (float **mfc, int32 nfr)
{
    int32 i;
    
    if (cmn == NORM_UTT)
	norm_mean (mfc[0], nfr, CEP_SIZE);
    else if (cmn == NORM_PRIOR) {
	for (i = 0; i < nfr; i++)
	    mean_norm_acc_sub (mfc[i]);
    }

    return 0;
}


static int32 agc_batch (float **mfc, int32 nfr)
{
    int32 i;
    float agc_out[CEP_SIZE];
    
    if (agc == AGC_NOISE) {
	real_agc_noise (mfc[0], nfr, CEP_SIZE);
    } else if (agc == AGC_MAX) {
	agc_max (mfc[0], nfr, CEP_SIZE);
    } else if (agc == AGC_EMAX) {
	for (i = 0; i < nfr; i++) {
	    agc_emax_proc (agc_out, mfc[i], CEP_SIZE);
	    memcpy (mfc[i], agc_out, CEP_SIZE * sizeof(float));
	}
    } else
	E_WARN("NO AGC\n");

    return 0;
}


static int32 silcomp_batch (float **mfc, int32 nfr)
{
    int32 i, j;
    float noiselevel;

    if (silcomp == COMPRESS_PRIOR) {
	j = 0;
	for (i = 0; i < nfr; i++) {
	    if (histo_add_c0 (mfc[i][0])) {
		if (i != j)
		    memcpy (mfc[j], mfc[i], sizeof(float)*CEP_SIZE);

		comp2rawfr[j++] = i;
	    }
	    /* Else skip the frame, don't copy across */
	}
	nfr = j;
    } else {
	for (i = 0; i < nfr; i++)
	    comp2rawfr[i] = i;		/* HACK!! */

	if (silcomp == COMPRESS_UTT) {
	    noiselevel = histo_noise_level (mfc[0], nfr, CEP_SIZE);
	    nfr = delete_background (mfc[0], nfr, CEP_SIZE, noiselevel);
	}
    }
    
    return nfr;
}


static int32 mfc2feat_batch (float **mfc, int32 nfr)
{
    int32 i, j, k;
    
    cmn_batch (mfc, nfr);
    agc_batch (mfc, nfr);
    nfr = silcomp_batch (mfc, nfr);
    
    assert (cep_i == 0);
    
    /*
     * HACK!! Hardwired knowledge that first and last 4 frames don't have features.
     * Simply copy frame[4] into frame[0..3], and frame[n-4] into the last four.
     */
    cep_i = (CEP_SIZE << 2);
    pow_i = (POW_SIZE << 2);
    for (i = 0; i < nfr; i++) {
	if (compute_features(cep_buf + cep_i,
			     dcep_buf + cep_i,
			     dcep_80ms_buf + cep_i,
			     pcep_buf + pow_i,
			     ddcep_buf + cep_i,
			     mfc[i])) {
	    cep_i += CEP_SIZE;
	    pow_i += POW_SIZE;

	    n_featfr++;
	}
    }

    /* Copy frame[4] into frame[0]..[3] */
    for (i = 0, j = 0, k = 0; i < 4; i++, j += CEP_SIZE, k += POW_SIZE) {
	memcpy (cep_buf+j, cep_buf + (CEP_SIZE<<2), CEP_SIZE * sizeof(float));
	memcpy (dcep_buf+j, dcep_buf + (CEP_SIZE<<2), CEP_SIZE * sizeof(float));
	memcpy (dcep_80ms_buf+j, dcep_80ms_buf + (CEP_SIZE<<2), CEP_SIZE * sizeof(float));
	memcpy (ddcep_buf+j, ddcep_buf + (CEP_SIZE<<2), CEP_SIZE * sizeof(float));
	memcpy (pcep_buf+k, pcep_buf + (POW_SIZE<<2), POW_SIZE * sizeof(float));
	
	n_featfr++;
    }
    /* Similarly fill in the last 4 frames */
    for (i = 0; i < 4; i++) {
	memcpy (cep_buf + cep_i, cep_buf + (cep_i - CEP_SIZE), CEP_SIZE*sizeof(float));
	memcpy (dcep_buf + cep_i, dcep_buf + (cep_i - CEP_SIZE), CEP_SIZE*sizeof(float));
	memcpy (dcep_80ms_buf + cep_i, dcep_80ms_buf + (cep_i - CEP_SIZE),
		CEP_SIZE*sizeof(float));
	memcpy (ddcep_buf + cep_i, ddcep_buf + (cep_i - CEP_SIZE), CEP_SIZE*sizeof(float));
	memcpy (pcep_buf + pow_i, pcep_buf + (pow_i - POW_SIZE), POW_SIZE*sizeof(float));
	
	cep_i += CEP_SIZE;
	pow_i += POW_SIZE;
	
	n_featfr++;
    }
    
    return 0;
}


/* Convert all given mfc vectors to feature vectors, and search one frame */
static int32 uttproc_frame ( void )
{
    /* Search one frame */
    if (query_fwdtree_flag())
	search_fwd (cep_buf + search_cep_i,
		    dcep_buf + search_cep_i,
		    dcep_80ms_buf + search_cep_i,
		    pcep_buf + search_pow_i,
		    ddcep_buf + search_cep_i);
    else
	search_fwdflat_frame (cep_buf + search_cep_i,
			      dcep_buf + search_cep_i,
			      dcep_80ms_buf + search_cep_i,
			      pcep_buf + search_pow_i,
			      ddcep_buf + search_cep_i);
    search_cep_i += CEP_SIZE;
    search_pow_i += POW_SIZE;
    n_searchfr++;
    
    return 0;
}


static void fwdflat_search (int32 n_frames)
{
    int32 i, j, k;
    
    search_fwdflat_start ();

    for (i = 0, j = 0, k = 0; i < n_frames; i++, j += CEP_SIZE, k += POW_SIZE)
	search_fwdflat_frame (cep_buf+j, dcep_buf+j, dcep_80ms_buf+j, pcep_buf+k, ddcep_buf+j);

    search_fwdflat_finish ();
}


static void write_results (char const *hyp, int32 aborted)
{
    search_hyp_t *seghyp;	/* Hyp with word segmentation information */
    int32 i;
    
    /* Check if need to autonumber utterances */
    if (matchfp) {
	fprintf (matchfp, "%s (%s %s %d)\n",
		 hyp, uttid, aborted ? "[ABORTED]" : "", search_get_score());
	fflush (matchfp);
    }
    
    if (matchsegfp) {
	fprintf (matchsegfp, "%s ", uttid);
	seghyp = search_get_hyp ();
	for (i = 0; seghyp[i].wid >= 0; i++) {
	    fprintf (matchsegfp, " %d %d %s",
		     seghyp[i].sf,
		     (seghyp[i].ef-seghyp[i].sf+1),
		     kb_get_word_str(seghyp[i].wid));
	}
	fprintf (matchsegfp, "\n");
	fflush (matchsegfp);
    }
    
#if 0
    {
	char const *dumplatdir;
	if ((dumplatdir = query_dumplat_dir()) != NULL) {
	    char fplatfile[1024];
	
	    sprintf (fplatfile, "%s/%s.fplat", dumplatdir, uttid);
	    search_dump_lattice_ascii (fplatfile);
    }
#endif /* 0 */
}


static void uttproc_windup (int32 *fr, char **hyp)
{
    /* Wind up first pass and run next pass, if necessary */
    if (query_fwdtree_flag()) {
	search_finish_fwd ();
	
	if (query_fwdflat_flag() && (searchFrame() > 0))
	    fwdflat_search (n_featfr);
    } else
	search_fwdflat_finish ();

    /* Run bestpath pass if specified */
    if ((searchFrame() > 0) && query_bestpath_flag())
	bestpath_search ();
    
    timing_stop ();
    
    search_result (fr, hyp);

    write_results (*hyp, 0);
    
    uttstate = UTTSTATE_IDLE;
}


/*
 * One time initialization
 */

fe_t    *fe;

int32 uttproc_init ( void )
{
    char const *fn;
    int32 sps;

    param_t *fe_param;
    fe_param = CM_calloc(1, sizeof(param_t));

    if (uttstate != UTTSTATE_UNDEF) {
	E_ERROR("uttproc_init called when not in UNDEF state\n");
	return -1;
    }
    
    sps = query_sampling_rate ();


    if ((sps != 16000) && (sps != 8000))
	E_FATAL("Sampling rate must be 8000 or 16000, is %d\n", sps);
    

    frame_spacing = sps/100;

    fe_param->SAMPLING_RATE = sps;
  /*    fe_param->FRAME_RATE    = frame_spacing; */  /* removed; KAL */
    fe_param->FRAME_RATE    = 100;
    fe_param->PRE_EMPHASIS_ALPHA = 0.97;
    
    fe = fe_init(fe_param);

    if (!fe) 
      return -1;

    mean_norm_init (CEP_SIZE);
    
    feat_alloc ();

    comp2rawfr = (int16 *) CM_calloc (MAX_UTT_LEN, sizeof(int16));
    uttid = (char *) CM_calloc (UTTIDSIZE, 1);

    if ((fn = query_match_file_name()) != NULL) {
	if ((matchfp = fopen (fn, "w")) == NULL)
	    E_ERROR("fopen(%s,w) failed\n", fn);
    }
    if ((fn = query_matchseg_file_name()) != NULL) {
	if ((matchsegfp = fopen (fn, "w")) == NULL)
	    E_ERROR("fopen(%s,w) failed\n", fn);
    }
    
    if ((fn = query_cdcn_file()) != NULL) {
	E_INFO("Initializing CDCN module from %s\n", fn);
	cdcn_init (fn, &cdcn);
    }
    
    timing_init ();

    uttstate = UTTSTATE_IDLE;
    utt_ofl = 0;
    uttno = 0;

    free(fe_param);

    return 0;
}


CDCN_type *uttproc_get_cdcn_ptr ( void )
{
    return &cdcn;
}


/*
 * One time cleanup
 */
int32 uttproc_end ( void )
{
    if (uttstate != UTTSTATE_IDLE) {
	E_ERROR("uttproc_end called when not in IDLE state\n");
	return -1;
    }
    
    if (matchfp)
	fclose (matchfp);
    if (matchsegfp)
	fclose (matchsegfp);
    
    timing_end ();

    return 0;
}


int32 uttproc_begin_utt (char const *id)
{
    char filename[1024];
    int32 i;
    
    for (i = 0; i < 5; i++)
	samp_hist[i] = 0;
    max_samp = 0;
    
    if (uttstate != UTTSTATE_IDLE) {
	E_ERROR("uttproc_begin_utt called when not in IDLE state\n");
	return -1;
    }

    if (fe_start_utt(fe) < 0)
	return -1;
    
    inputtype = INPUT_UNKNOWN;

    livemode = (nosearch ||
		(cmn == NORM_UTT) ||
		((agc != AGC_EMAX) && (agc != AGC_NONE)) ||
		(silcomp == COMPRESS_UTT)) ? 0 : 1;
    E_INFO("%s\n", livemode ? "Livemode" : "Batchmode");
    
    /*
     * One-time initialization of AGC as necessary. Done here rather than in
     * uttproc_init because type of cmn/agc not known until now.
     */
    if ((uttno == 0) && (agc == AGC_EMAX)) {
	if (cmn == NORM_PRIOR)
	    uttproc_agcemax_set (5.0);	/* Hack!! Hardwired max(C0) of 5.0 with CMN */
	else
	    uttproc_agcemax_set (10.0);	/* Hack!! Hardwired max(C0) of 10.0 without CMN */
    }
    
    pow_i = cep_i = 0;
    search_pow_i = search_cep_i = 0;
    n_rawfr = n_featfr = n_searchfr = n_compfr = 0;
    utt_ofl = 0;
    
    uttno++;
    if (! id)
	sprintf (uttid, "%s%08d", uttid_prefix ? uttid_prefix : "", uttno);
    else
	strcpy (uttid, id);
    
    if (rawlogdir) {
	sprintf (filename, "%s/%s.raw", rawlogdir, uttid);
	if ((rawfp = fopen(filename, "wb")) == NULL)
	    E_ERROR("fopen(%s,wb) failed\n", filename);
	else {
	    strcpy (rawfilename, filename);
	    E_INFO("Rawfile: %s\n", filename);
	}
    }
    if (mfclogdir) {
	int32 k = 0;

	sprintf (filename, "%s/%s.mfc", mfclogdir, uttid);
	if ((mfcfp = fopen(filename, "wb")) == NULL)
	    E_ERROR("fopen(%s,wb) failed\n", filename);
	else
	    fwrite (&k, sizeof(int32), 1, mfcfp);
    }
    
    timing_start ();
    
    SCVQNewUtt ();
    
    if (! nosearch) {
	if (query_fwdtree_flag())
	    search_start_fwd ();
	else
	    search_fwdflat_start ();
    }
    
    uttstate = UTTSTATE_BEGUN;
    
    return 0;
}


int32 uttproc_rawdata (int16 *raw, int32 len, int32 block)
{
    int32 i, k, v;
    
    for (i = 0; i < len; i++) {
	v = raw[i];
	if (v < 0)
	    v = -v;
	if (v > max_samp)
	    max_samp = v;
	
	if (v < 4096)
	    samp_hist[0]++;
	else if (v < 8192)
	    samp_hist[1]++;
	else if (v < 16384)
	    samp_hist[2]++;
	else if (v < 30720)
	    samp_hist[3]++;
	else
	    samp_hist[4]++;
    }
    
    if (uttstate != UTTSTATE_BEGUN) {
	E_ERROR("uttproc_rawdata called when utterance not begun\n");
	return -1;
    }
    if (inputtype == INPUT_MFC) {
	E_ERROR("uttproc_rawdata mixed with uttproc_cepdata in same utterance??\n");
	return -1;
    }
    inputtype = INPUT_RAW;
    
    if (utt_ofl)
	return -1;
    
    k = (MAX_UTT_LEN - n_rawfr) * frame_spacing;
    if (len > k) {
	len = k;
	utt_ofl = 1;
	E_ERROR("Utterance too long; truncating to about %d frames\n", MAX_UTT_LEN);
    }

    if (rawfp && (len > 0))
	fwrite (raw, sizeof(int16), len, rawfp);
    
    /*    
	  if ((k = fe_raw2cep (raw, len, mfcbuf + n_rawfr)) < 0)
	  return -1;
    */
    if ((k = fe_process_utt (fe, raw, len, mfcbuf + n_rawfr)) < 0)
	return -1;

    if (mfcfp && (k > 0))
	fwrite (mfcbuf[n_rawfr], sizeof(float), k * CEP_SIZE, mfcfp);

    if (livemode) {
	mfc2feat_live (mfcbuf+n_rawfr, k);

	if (search_cep_i < cep_i)
	    uttproc_frame ();

	if (block) {
	    while (search_cep_i < cep_i)
		uttproc_frame ();
	}
    } else
	n_rawfr += k;

    return (n_featfr - n_searchfr);
}


int32 uttproc_cepdata (float **cep, int32 nfr, int32 block)
{
    int32 i, k;
    
    if (uttstate != UTTSTATE_BEGUN) {
	E_ERROR("uttproc_cepdata called when utterance not begun\n");
	return -1;
    }
    if (inputtype == INPUT_RAW) {
	E_ERROR("uttproc_cepdata mixed with uttproc_rawdata in same utterance??\n");
	return -1;
    }
    inputtype = INPUT_MFC;
    
    if (utt_ofl)
	return -1;
    
    k = MAX_UTT_LEN - n_rawfr;
    if (nfr > k) {
	nfr = k;
	utt_ofl = 1;
	E_ERROR("Utterance too long; truncating to about %d frames\n", MAX_UTT_LEN);
    }
    
    for (i = 0; i < nfr; i++)
	memcpy (mfcbuf[i+n_rawfr], cep[i], CEP_SIZE*sizeof(float));

    if (mfcfp && (nfr > 0))
	fwrite (mfcbuf[n_rawfr], sizeof(float), nfr * CEP_SIZE, mfcfp);

    if (livemode) {
	mfc2feat_live (mfcbuf+n_rawfr, nfr);

	if (search_cep_i < cep_i)
	    uttproc_frame ();

	if (block) {
	    while (search_cep_i < cep_i)
		uttproc_frame ();
	}
    } else
	n_rawfr += nfr;

    return (n_featfr - n_searchfr);
}


int32 uttproc_end_utt ( void )
{
    int32 i, k;
    float cep[13], c0;
    float *leftover_cep;

    /* kal */
    leftover_cep       = (float *) CM_calloc (MAX_CEP_LEN, sizeof(float));

    /* Dump samples histogram */
    k = 0;
    for (i = 0; i < 5; i++)
	k += samp_hist[i];
    if (k > 0) {
	E_INFO("Samples histogram (%s) (4/8/16/30/32K):", uttproc_get_uttid());
	for (i = 0; i < 5; i++)
	    log_info(" %.1f%%(%d)", samp_hist[i]*100.0/k, samp_hist[i]);
	log_info("; max: %d\n", max_samp);
    }
    
    if (uttstate != UTTSTATE_BEGUN) {
	E_ERROR("uttproc_end_utt called when utterance not begun\n");
	return -1;
    }

    if (! livemode)
	mfc2feat_batch (mfcbuf, n_rawfr);

    uttstate = nosearch ? UTTSTATE_IDLE : UTTSTATE_ENDED;

    fe_end_utt(fe, leftover_cep);

    SCVQEndUtt();
    
    /* Update estimated CMN vector */ 
    if (cmn == NORM_PRIOR) {
	uttproc_cepmean_get (cep);
	c0 = cep[0];
	mean_norm_update ();
	uttproc_cepmean_get (cep);

	/* Update estimated AGC Max (C0) */ 
	if (agc == AGC_EMAX) {
	    agc_emax_update ();
	}
    } else {
	/* Update estimated AGC Max (C0) */ 
	if (agc == AGC_EMAX) {
	    agc_emax_update ();
	}
    }
    
    if (silcomp == COMPRESS_PRIOR)
	compute_noise_level ();
    
    if (rawfp) {
	fclose (rawfp);
	rawfp = NULL;
#ifdef WIN32
	if (_chmod(rawfilename, _S_IREAD ) < 0)
	    E_ERROR("chmod(%s,READONLY) failed\n", rawfilename);
#endif
    }
    if (mfcfp) {
	int32 k;
	
	fflush (mfcfp);
	fseek (mfcfp, 0, SEEK_SET);
	k = n_rawfr * CEP_SIZE;
	fwrite (&k, sizeof(int32), 1, mfcfp);

	fclose (mfcfp);
	mfcfp = NULL;
    }

    free(leftover_cep);

    return 0;
}


int32 uttproc_abort_utt ( void )
{
    int32 fr;
    char *hyp;
    
    if (uttproc_end_utt () < 0)
	return -1;

    /* Truncate utterance to the portion already processed */
    cep_i = search_cep_i;
    pow_i = search_pow_i;
    
    uttstate = UTTSTATE_IDLE;

    if (! nosearch) {
	if (query_fwdtree_flag())
	    search_finish_fwd ();
	else
	    search_fwdflat_finish ();
	
	timing_stop ();
	
	search_result (&fr, &hyp);
	
	write_results (hyp, 1);
    }
    
    return 0;
}


int32 uttproc_stop_utt ( void )
{
    if (uttstate != UTTSTATE_BEGUN) {
	E_ERROR("uttproc_stop_utt called when utterance not begun\n");
	return -1;
    }

    uttstate = UTTSTATE_STOPPED;
    
    if (! nosearch) {
	if (query_fwdtree_flag())
	    search_finish_fwd ();
	else
	    search_fwdflat_finish ();
    }
    
    return 0;
}


int32 uttproc_restart_utt ( void )
{
    if (uttstate != UTTSTATE_STOPPED) {
	E_ERROR("uttproc_restart_utt called when decoding not stopped\n");
	return -1;
    }

    uttstate = UTTSTATE_BEGUN;
    
    if (! nosearch) {
	if (query_fwdtree_flag())
	    search_start_fwd ();
	else
	    search_fwdflat_start ();
	
	search_cep_i = 0;
	search_pow_i = 0;
	n_searchfr = 0;
    }
    
    return 0;
}


int32 uttproc_partial_result (int32 *fr, char **hyp)
{
    if ((uttstate != UTTSTATE_BEGUN) && (uttstate != UTTSTATE_ENDED)) {
	E_ERROR("uttproc_partial_result called outside utterance\n");
	*fr = -1;
	*hyp = NULL;
	return -1;
    }

    search_partial_result (fr, hyp);
    
    return 0;
}


int32 uttproc_result (int32 *fr, char **hyp, int32 block)
{
    if (uttstate != UTTSTATE_ENDED) {
	E_ERROR("uttproc_result called when utterance not ended\n");
	*hyp = NULL;
	*fr = -1;

	return -1;
    }
    
    if (search_cep_i < cep_i)
	uttproc_frame ();

    if (block) {
	while (search_cep_i < cep_i)
	    uttproc_frame ();
    }
    
    if (search_cep_i < cep_i)
	return (n_featfr - n_searchfr);

    uttproc_windup (fr, hyp);
    
    return 0;
}


void uttproc_align (char *sent)
{
    time_align_utterance ("alignment", NULL, "<s>", -1, sent, -1, "</s>");
}


void utt_seghyp_free (search_hyp_t *h)
{
    search_hyp_t *tmp;

    while (h) {
	tmp = h->next;
	listelem_free (h, sizeof(search_hyp_t));
	h = tmp;
    }
}


static void build_utt_seghyp ( void )
{
    int32 i;
    search_hyp_t *seghyp, *last, *new;

    /* Obtain word segmentation result */
    seghyp = search_get_hyp ();

    /* Fill in missing details and build segmentation linked list */
    last = NULL;
    for (i = 0; seghyp[i].wid >= 0; i++) {
	new = (search_hyp_t *) listelem_alloc (sizeof(search_hyp_t));
	new->wid = seghyp[i].wid;
	new->word = kb_get_word_str (new->wid);
	new->sf = seghyp[i].sf;
	new->ef = seghyp[i].ef;
	new->latden = seghyp[i].latden;
	new->next = NULL;

	if (! last)
	    utt_seghyp = new;
	else
	    last->next = new;
	last = new;
    }
}


int32 uttproc_partial_result_seg (int32 *fr, search_hyp_t **hyp)
{
    char *str;
    
    /* Free any previous segmentation result */
    utt_seghyp_free (utt_seghyp);
    utt_seghyp = NULL;
    
    if ((uttstate != UTTSTATE_BEGUN) && (uttstate != UTTSTATE_ENDED)) {
	E_ERROR("uttproc_partial_result called outside utterance\n");
	*fr = -1;
	*hyp = NULL;
	return -1;
    }

    search_partial_result (fr, &str);	/* Internally makes partial result */
    build_utt_seghyp();
    *hyp = utt_seghyp;
    
    return 0;
}


int32 uttproc_result_seg (int32 *fr, search_hyp_t **hyp, int32 block)
{
    char *str;
    int32 res;
    
    /* Free any previous segmentation result */
    utt_seghyp_free (utt_seghyp);
    utt_seghyp = NULL;
    
    if ((res = uttproc_result (fr, &str, block)) != 0)
	return res;	/* Not done yet; or ERROR */

    build_utt_seghyp();
    *hyp = utt_seghyp;

    return 0;
}


int32 uttproc_lmupdate (char const *lmname)
{
    lm_t *lm, *cur_lm;
    
    warn_notidle ("uttproc_lmupdate");
    
    if ((lm = lm_name2lm (lmname)) == NULL)
	return -1;
    
    cur_lm = lm_get_current ();
    if (lm == cur_lm)
	search_set_current_lm ();

    return 0;
}


int32 uttproc_set_context (char const *wd1, char const *wd2)
{
    int32 w1, w2;
    
    warn_notidle ("uttproc_set_context");
    
    if (wd1) {
	w1 = kb_get_word_id (wd1);
	if ((w1 < 0) || (! dictwd_in_lm (w1))) {
	    E_ERROR("Unknown word: %s\n", wd1);
	    search_set_context (-1, -1);

	    return -1;
	}
    } else
	w1 = -1;

    if (wd2) {
	w2 = kb_get_word_id (wd2);
	if ((w2 < 0) || (! dictwd_in_lm (w2))) {
	    E_ERROR("Unknown word: %s\n", wd2);
	    search_set_context (-1, -1);
	    
	    return -1;
	}
    } else
	w2 = -1;
    
    if (w2 < 0) {
	search_set_context (-1, -1);
	return ((w1 >= 0) ? -1 : 0);
    } else {
	/* Because of the perverse way search_set_context was defined... */
	if (w1 < 0)
	    search_set_context (w2, -1);
	else
	    search_set_context (w1, w2);
    }
    
    return 0;
}


int32 uttproc_set_lm (char const *lmname)
{
    warn_notidle ("uttproc_set_lm");
    
    if (lmname == NULL)
	E_FATAL("uttproc_set_lm called with NULL argument\n");
    
    if (lm_set_current (lmname) < 0)
	return -1;
    search_set_current_lm ();

    E_INFO("LM= \"%s\"\n", lmname);
    
    return 0;
}


int32 uttproc_set_rescore_lm (char const *lmname)
{
    searchlat_set_rescore_lm (lmname);
    return 0;
}


int32 uttproc_set_startword (char const *str)
{
    warn_notidle ("uttproc_set_startword");
    
    search_set_startword (str);
    return 0;
}


int32 uttproc_set_cmn (scvq_norm_t n)
{
    warn_notidle ("uttproc_set_cmn");
    
    cmn = n;
    return 0;
}


int32 uttproc_set_agc (scvq_agc_t a)
{
    warn_notidle ("uttproc_set_agc");
    
    agc = a;
    return 0;
}


int32 uttproc_set_silcmp (scvq_compress_t c)
{
    warn_notidle ("uttproc_set_silcmp");

    if (c != COMPRESS_NONE)
	E_WARN("Silence compression doesn't work well; use the cont_ad module instead\n");
    
    silcomp = c;
    return 0;
}


#if 0
int32 uttproc_set_uttid (char const *id)
{
    warn_notidle ("uttproc_set_uttid");
    
    assert (strlen(id) < UTTIDSIZE);
    strcpy (uttid, id);
    
    return 0;
}
#endif


char const *uttproc_get_uttid ( void )
{
    return uttid;
}


int32 uttproc_set_auto_uttid_prefix (char const *prefix)
{
    if (uttid_prefix)
	free (uttid_prefix);
    uttid_prefix = salloc(prefix);
    uttno = 0;
    
    return 0;
}


int32	uttprocGetcomp2rawfr(int16 **ptr)
{
    *ptr = comp2rawfr;
    return n_featfr;
}


void	uttprocSetcomp2rawfr(int32 num, int32 const *ptr)
{
    int32		i;
    
    n_featfr = num;
    for (i = 0; i < num; i++)
	comp2rawfr[i] = ptr[i];
}


int32 uttproc_feat2rawfr (int32 fr)
{
    if (fr >= n_featfr)
	fr = n_featfr-1;
    if (fr < 0)
	fr = 0;

    return comp2rawfr[fr+8]-4;
}


int32 uttproc_raw2featfr (int32 fr)
{
    int32 i;

    fr += 4;
    for (i = 0; (i < n_featfr) && (comp2rawfr[i] != fr); i++);
    if (i >= n_featfr)
        return -1;
    return (i-8);
}


int32 uttproc_cepmean_set (float *cep)
{
    warn_notidle ("uttproc_cepmean_set");

    return (cepmean_set (cep));
}


int32 uttproc_cepmean_get (float *cep)
{
    return (cepmean_get (cep));
}


int32 uttproc_agcemax_set (float c0max)
{
    warn_notidle ("uttproc_agcemax_set");
    agcemax_set (c0max);
    return 0;
}


double uttproc_agcemax_get ( void )
{
    extern double agcemax_get();
    
    warn_notidle ("uttproc_agcemax_get");
    return agcemax_get ();
}


int32 uttproc_nosearch (int32 flag)
{
    warn_notidle ("uttproc_nosearch");

    nosearch = flag;
    return 0;
}


int32 uttproc_set_rawlogdir (char const *dir)
{
    warn_notidle ("uttproc_set_rawlogdir");

    if (! rawlogdir) {
	if ((rawlogdir = calloc (1024,1)) == NULL) {
	    E_ERROR("calloc(1024,1) failed\n");
	    return -1;
	}
    }
    if (rawlogdir)
	strcpy (rawlogdir, dir);

    return 0;
}


int32 uttproc_set_mfclogdir (char const *dir)
{
    warn_notidle ("uttproc_set_mfclogdir");

    if (! mfclogdir) {
	if ((mfclogdir = calloc (1024,1)) == NULL) {
	    E_ERROR("calloc(1024,1) failed\n");
	    return -1;
	}
    }
    if (mfclogdir)
	strcpy (mfclogdir, dir);

    return 0;
}


search_hyp_t *uttproc_allphone_file (char const *utt)
{
    int32 nfr;
    extern search_hyp_t *allphone_utt();
    extern void build_uttid();		/* in main.c */
    extern int32 utt_file2feat();	/* in main.c */
    search_hyp_t *hyplist, *h;
    
    build_uttid (utt);

    if ((nfr = utt_file2feat (utt, 1)) < 0)
	return NULL;
    
    hyplist = allphone_utt (nfr, cep_buf, dcep_buf, dcep_80ms_buf, pcep_buf, ddcep_buf);

    /* Write match and matchseg files if needed */
    if (matchfp) {
	for (h = hyplist; h; h = h->next)
	    fprintf (matchfp, "%s ", h->word);
	fprintf (matchfp, "(%s)\n", uttid);
	fflush (matchfp);
    }
    if (matchsegfp) {
	fprintf (matchsegfp, "%s ", uttid);
	for (h = hyplist; h; h = h->next)
	    fprintf (matchsegfp, " %d %d %s", h->sf, h->ef, h->word);
	fprintf (matchsegfp, "\n");
	fflush (matchsegfp);
    }

    return hyplist;
}
