/*

            Coda: an Experimental Distributed File System
                             Release 3.1

          Copyright (c) 1987-1998 Carnegie Mellon University
                         All Rights Reserved

Permission  to  use, copy, modify and distribute this software and its
documentation is hereby granted,  provided  that  both  the  copyright
notice  and  this  permission  notice  appear  in  all  copies  of the
software, derivative works or  modified  versions,  and  any  portions
thereof, and that both notices appear in supporting documentation, and
that credit is given to Carnegie Mellon University  in  all  documents
and publicity pertaining to direct or indirect use of this code or its
derivatives.

CODA IS AN EXPERIMENTAL SOFTWARE SYSTEM AND IS  KNOWN  TO  HAVE  BUGS,
SOME  OF  WHICH MAY HAVE SERIOUS CONSEQUENCES.  CARNEGIE MELLON ALLOWS
FREE USE OF THIS SOFTWARE IN ITS "AS IS" CONDITION.   CARNEGIE  MELLON
DISCLAIMS  ANY  LIABILITY  OF  ANY  KIND  FOR  ANY  DAMAGES WHATSOEVER
RESULTING DIRECTLY OR INDIRECTLY FROM THE USE OF THIS SOFTWARE  OR  OF
ANY DERIVATIVE WORK.

Carnegie  Mellon  encourages  users  of  this  software  to return any
improvements or extensions that  they  make,  and  to  grant  Carnegie
Mellon the rights to redistribute these changes without encumbrance.
*/

__RCSID("$Header: /afs/cs/project/coda-src/cvs/coda/kernel-src/vfs/bsd44/cfs/coda_opstats.h,v 1.3 98/01/23 11:53:53 rvb Exp $");


/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */

/*
 * This code was written for the Coda file system at Carnegie Mellon
 * University.  Contributers include David Steere, James Kistler, and
 * M. Satyanarayanan.  */

/*
 * HISTORY
 * $Log:	cfs_subr.c,v $
 * Revision 1.8  98/01/31  20:53:12  rvb
 * First version that works on FreeBSD 2.2.5
 * 
 * Revision 1.7  98/01/23  11:53:42  rvb
 * Bring RVB_CFS1_1 to HEAD
 * 
 * Revision 1.6.2.3  98/01/23  11:21:05  rvb
 * Sync with 2.2.5
 * 
 * Revision 1.6.2.2  97/12/16  12:40:06  rvb
 * Sync with 1.3
 * 
 * Revision 1.6.2.1  97/12/06  17:41:21  rvb
 * Sync with peters coda.h
 * 
 * Revision 1.6  97/12/05  10:39:17  rvb
 * Read CHANGES
 * 
 * Revision 1.5.4.8  97/11/26  15:28:58  rvb
 * Cant make downcall pbuf == union cfs_downcalls yet
 * 
 * Revision 1.5.4.7  97/11/20  11:46:42  rvb
 * Capture current cfs_venus
 * 
 * Revision 1.5.4.6  97/11/18  10:27:16  rvb
 * cfs_nbsd.c is DEAD!!!; integrated into cfs_vf/vnops.c; cfs_nb_foo and cfs_foo are joined
 * 
 * Revision 1.5.4.5  97/11/13  22:03:00  rvb
 * pass2 cfs_NetBSD.h mt
 * 
 * Revision 1.5.4.4  97/11/12  12:09:39  rvb
 * reorg pass1
 * 
 * Revision 1.5.4.3  97/11/06  21:02:38  rvb
 * first pass at ^c ^z
 * 
 * Revision 1.5.4.2  97/10/29  16:06:27  rvb
 * Kill DYING
 * 
 * Revision 1.5.4.1  97/10/28 23:10:16  rvb
 * >64Meg; venus can be killed!
 *
 * Revision 1.5  97/08/05  11:08:17  lily
 * Removed cfsnc_replace, replaced it with a cfs_find, unhash, and
 * rehash.  This fixes a cnode leak and a bug in which the fid is
 * not actually replaced.  (cfs_namecache.c, cfsnc.h, cfs_subr.c)
 * 
 * Revision 1.4  96/12/12  22:10:59  bnoble
 * Fixed the "downcall invokes venus operation" deadlock in all known cases.  There may be more
 * 
 * Revision 1.3  1996/12/05 16:20:15  bnoble
 * Minor debugging aids
 *
 * Revision 1.2  1996/01/02 16:57:01  bnoble
 * Added support for Coda MiniCache and raw inode calls (final commit)
 *
 * Revision 1.1.2.1  1995/12/20 01:57:27  bnoble
 * Added CFS-specific files
 *
 * Revision 3.1.1.1  1995/03/04  19:07:59  bnoble
 * Branch for NetBSD port revisions
 *
 * Revision 3.1  1995/03/04  19:07:58  bnoble
 * Bump to major revision 3 to prepare for NetBSD port
 *
 * Revision 2.8  1995/03/03  17:00:04  dcs
 * Fixed kernel bug involving sleep and upcalls. Basically if you killed
 * a job waiting on venus, the venus upcall queues got trashed. Depending
 * on luck, you could kill the kernel or not.
 * (mods to cfs_subr.c and cfs_mach.d)
 *
 * Revision 2.7  95/03/02  22:45:21  dcs
 * Sun4 compatibility
 * 
 * Revision 2.6  95/02/17  16:25:17  dcs
 * These versions represent several changes:
 * 1. Allow venus to restart even if outstanding references exist.
 * 2. Have only one ctlvp per client, as opposed to one per mounted cfs device.d
 * 3. Allow ody_expand to return many members, not just one.
 * 
 * Revision 2.5  94/11/09  15:56:26  dcs
 * Had the thread sleeping on the wrong thing!
 * 
 * Revision 2.4  94/10/14  09:57:57  dcs
 * Made changes 'cause sun4s have braindead compilers
 * 
 * Revision 2.3  94/10/12  16:46:26  dcs
 * Cleaned kernel/venus interface by removing XDR junk, plus
 * so cleanup to allow this code to be more easily ported.
 * 
 * Revision 1.2  92/10/27  17:58:22  lily
 * merge kernel/latest and alpha/src/cfs
 * 
 * Revision 2.4  92/09/30  14:16:26  mja
 * 	Incorporated Dave Steere's fix for the GNU-Emacs bug.
 * 	Also, included his cfs_flush routine in place of the former cfsnc_flush.
 * 	[91/02/07            jjk]
 * 
 * 	Added contributors blurb.
 * 	[90/12/13            jjk]
 * 
 * 	Hack to allow users to keep coda venus calls uninterruptible. THis
 * 	basically prevents the Gnu-emacs bug from appearing, in which a call
 * 	was being interrupted, and return EINTR, but gnu didn't check for the
 * 	error and figured the file was buggered.
 * 	[90/12/09            dcs]
 * 
 * Revision 2.3  90/08/10  10:23:20  mrt
 * 	Removed include of vm/vm_page.h as it no longer exists.
 * 	[90/08/10            mrt]
 * 
 * Revision 2.2  90/07/05  11:26:35  mrt
 * 	Initialize name cache on first call to vcopen.
 * 	[90/05/23            dcs]
 * 
 * 	Created for the Coda File System.
 * 	[90/05/23            dcs]
 * 
 * Revision 1.5  90/05/31  17:01:35  dcs
 * Prepare for merge with facilities kernel.
 * 
 * Revision 1.2  90/03/19  15:56:25  dcs
 * Initialize name cache on first call to vcopen.
 * 
 * Revision 1.1  90/03/15  10:43:26  jjk
 * Initial revision
 * 
 */ 

/* @(#)cfs_subr.c	1.5 87/09/14 3.2/4.3CFSSRC */

/* NOTES: rvb
 * 1.	Added cfs_unmounting to mark all cnodes as being UNMOUNTING.  This has to
 *	 be done before dounmount is called.  Because some of the routines that
 *	 dounmount calls before cfs_unmounted might try to force flushes to venus.
 *	 The vnode pager does this.
 * 2.	cfs_unmounting marks all cnodes scanning cfs_cache.
 * 3.	cfs_checkunmounting (under DEBUG) checks all cnodes by chasing the vnodes
 *	 under the /coda mount point.
 * 4.	cfs_cacheprint (under DEBUG) prints names with vnode/cnode address
 */

#include <vcfs.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/proc.h>
#include <sys/select.h>
#include <sys/mount.h>

#include <cfs/coda.h>
#include <cfs/cnode.h>
#include <cfs/cfs_subr.h>
#include <cfs/cfsnc.h>


__RCSID("$Header: /afs/cs/project/coda-src/cvs/coda/kernel-src/vfs/bsd44/cfs/cfs_subr.c,v 1.8 98/01/31 20:53:12 rvb Exp $");

#if	NVCFS

int cfs_active = 0;
int cfs_reuse = 0;
int cfs_new = 0;

struct cnode *cfs_freelist = NULL;
struct cnode *cfs_cache[CFS_CACHESIZE];

#define cfshash(fid) \
    (((fid)->Volume + (fid)->Vnode) & (CFS_CACHESIZE-1))

#define	CNODE_NEXT(cp)	((cp)->c_next)

#define ODD(vnode)        ((vnode) & 0x1)

/*
 * Allocate a cnode.
 */
struct cnode *
cfs_alloc(void)
{
    struct cnode *cp;

    if (cfs_freelist) {
	cp = cfs_freelist;
	cfs_freelist = CNODE_NEXT(cp);
	cfs_reuse++;
    }
    else {
	CFS_ALLOC(cp, struct cnode *, sizeof(struct cnode));
	/* NetBSD vnodes don't have any Pager info in them ('cause there are
	   no external pagers, duh!) */
#define VNODE_VM_INFO_INIT(vp)         /* MT */
	VNODE_VM_INFO_INIT(CTOV(cp));
	cfs_new++;
    }
    bzero(cp, sizeof (struct cnode));

    return(cp);
}

/*
 * Deallocate a cnode.
 */
void
cfs_free(cp)
     register struct cnode *cp;
{

    CNODE_NEXT(cp) = cfs_freelist;
    cfs_freelist = cp;
}

/*
 * Put a cnode in the hash table
 */
void
cfs_save(cp)
     struct cnode *cp;
{
	CNODE_NEXT(cp) = cfs_cache[cfshash(&cp->c_fid)];
	cfs_cache[cfshash(&cp->c_fid)] = cp;
}

/*
 * Remove a cnode from the hash table
 */
void
cfs_unsave(cp)
     struct cnode *cp;
{
    struct cnode *ptr;
    struct cnode *ptrprev = NULL;
    
    ptr = cfs_cache[cfshash(&cp->c_fid)]; 
    while (ptr != NULL) { 
	if (ptr == cp) { 
	    if (ptrprev == NULL) {
		cfs_cache[cfshash(&cp->c_fid)] 
		    = CNODE_NEXT(ptr);
	    } else {
		CNODE_NEXT(ptrprev) = CNODE_NEXT(ptr);
	    }
	    CNODE_NEXT(cp) = (struct cnode *)NULL;
	    
	    return; 
	}	
	ptrprev = ptr;
	ptr = CNODE_NEXT(ptr);
    }	
}

/*
 * Lookup a cnode by fid. If the cnode is dying, it is bogus so skip it.
 * NOTE: this allows multiple cnodes with same fid -- dcs 1/25/95
 */
struct cnode *
cfs_find(fid) 
     ViceFid *fid;
{
    struct cnode *cp;

    cp = cfs_cache[cfshash(fid)];
    while (cp) {
	if ((cp->c_fid.Vnode == fid->Vnode) &&
	    (cp->c_fid.Volume == fid->Volume) &&
	    (cp->c_fid.Unique == fid->Unique) &&
	    (!IS_UNMOUNTING(cp)))
	    {
		cfs_active++;
		return(cp); 
	    }		    
	cp = CNODE_NEXT(cp);
    }
    return(NULL);
}

/*
 * cfs_kill is called as a side effect to vcopen. To prevent any
 * cnodes left around from an earlier run of a venus or warden from
 * causing problems with the new instance, mark any outstanding cnodes
 * as dying. Future operations on these cnodes should fail (excepting
 * cfs_inactive of course!). Since multiple venii/wardens can be
 * running, only kill the cnodes for a particular entry in the
 * cfs_mnttbl. -- DCS 12/1/94 */

int
cfs_kill(whoIam, dcstat)
	struct mount *whoIam;
	enum dc_status dcstat;
{
	int hash, count = 0;
	struct cnode *cp;
	
	/* 
	 * Algorithm is as follows: 
	 *     Second, flush whatever vnodes we can from the name cache.
	 * 
	 *     Finally, step through whatever is left and mark them dying.
	 *        This prevents any operation at all.
	 */
	
	/* This is slightly overkill, but should work. Eventually it'd be
	 * nice to only flush those entries from the namecache that
	 * reference a vnode in this vfs.  */
	cfsnc_flush(dcstat);
	
	for (hash = 0; hash < CFS_CACHESIZE; hash++) {
		for (cp = cfs_cache[hash]; cp != NULL; cp = CNODE_NEXT(cp)) {
			if (CTOV(cp)->v_mount == whoIam) {
#ifdef	DEBUG
				printf("cfs_kill: vp %p, cp %p\n", CTOV(cp), cp);
#endif
				count++;
				CFSDEBUG(CFS_FLUSH, 
					 myprintf(("Live cnode fid %lx.%lx.%lx flags %d count %d\n",
						   (cp->c_fid).Volume,
						   (cp->c_fid).Vnode,
						   (cp->c_fid).Unique, 
						   cp->c_flags,
						   CTOV(cp)->v_usecount)); );
			}
		}
	}
	return count;
}

/*
 * There are two reasons why a cnode may be in use, it may be in the
 * name cache or it may be executing.  
 */
void
cfs_flush(dcstat)
	enum dc_status dcstat;
{
    int hash;
    struct cnode *cp;
    
    cfs_clstat.ncalls++;
    cfs_clstat.reqs[CFS_FLUSH]++;
    
    cfsnc_flush(dcstat);	    /* flush files from the name cache */

    for (hash = 0; hash < CFS_CACHESIZE; hash++) {
	for (cp = cfs_cache[hash]; cp != NULL; cp = CNODE_NEXT(cp)) {  
	    if (!ODD(cp->c_fid.Vnode)) /* only files can be executed */
		cfs_vmflush(cp);
	}
    }
}

/*
 * As a debugging measure, print out any cnodes that lived through a
 * name cache flush.  
 */
void
cfs_testflush(void)
{
    int hash;
    struct cnode *cp;
    
    for (hash = 0; hash < CFS_CACHESIZE; hash++) {
	for (cp = cfs_cache[hash];
	     cp != NULL;
	     cp = CNODE_NEXT(cp)) {  
	    myprintf(("Live cnode fid %lx.%lx.%lx count %d\n",
		      (cp->c_fid).Volume,(cp->c_fid).Vnode,
		      (cp->c_fid).Unique, CTOV(cp)->v_usecount));
	}
    }
}

/*
 *     First, step through all cnodes and mark them unmounting.
 *         NetBSD kernels may try to fsync them now that venus
 *         is dead, which would be a bad thing.
 *
 */
void
cfs_unmounting(whoIam)
	struct mount *whoIam;
{	
	int hash;
	struct cnode *cp;

	for (hash = 0; hash < CFS_CACHESIZE; hash++) {
		for (cp = cfs_cache[hash]; cp != NULL; cp = CNODE_NEXT(cp)) {
			if (CTOV(cp)->v_mount == whoIam) {
				if (cp->c_flags & (C_LOCKED|C_WANTED)) {
					printf("cfs_unmounting: Unlocking %p\n", cp);
					cp->c_flags &= ~(C_LOCKED|C_WANTED);
					wakeup((caddr_t) cp);
				}
				cp->c_flags |= C_UNMOUNTING;
			}
		}
	}
}

#ifdef	DEBUG
cfs_checkunmounting(mp)
	struct mount *mp;
{	
	register struct vnode *vp, *nvp;
	struct cnode *cp;
	int count = 0, bad = 0;
loop:
	for (vp = mp->mnt_vnodelist.lh_first; vp; vp = nvp) {
		if (vp->v_mount != mp)
			goto loop;
		nvp = vp->v_mntvnodes.le_next;
		cp = VTOC(vp);
		count++;
		if (!(cp->c_flags & C_UNMOUNTING)) {
			bad++;
			printf("vp %p, cp %p missed\n", vp, cp);
			cp->c_flags |= C_UNMOUNTING;
		}
	}
}

int
cfs_cacheprint(whoIam)
	struct mount *whoIam;
{	
	int hash;
	struct cnode *cp;
	int count = 0;

	printf("cfs_cacheprint: cfs_ctlvp %p, cp %p", cfs_ctlvp, VTOC(cfs_ctlvp));
	cfsnc_name(cfs_ctlvp);
	printf("\n");

	for (hash = 0; hash < CFS_CACHESIZE; hash++) {
		for (cp = cfs_cache[hash]; cp != NULL; cp = CNODE_NEXT(cp)) {
			if (CTOV(cp)->v_mount == whoIam) {
				printf("cfs_cacheprint: vp %p, cp %p", CTOV(cp), cp);
				cfsnc_name(cp);
				printf("\n");
				count++;
			}
		}
	}
	printf("cfs_cacheprint: count %d\n", count);
}
#endif

/*
 * There are 6 cases where invalidations occur. The semantics of each
 * is listed here.
 *
 * CFS_FLUSH     -- flush all entries from the name cache and the cnode cache.
 * CFS_PURGEUSER -- flush all entries from the name cache for a specific user
 *                  This call is a result of token expiration.
 *
 * The next two are the result of callbacks on a file or directory.
 * CFS_ZAPDIR    -- flush the attributes for the dir from its cnode.
 *                  Zap all children of this directory from the namecache.
 * CFS_ZAPFILE   -- flush the attributes for a file.
 *
 * The fifth is a result of Venus detecting an inconsistent file.
 * CFS_PURGEFID  -- flush the attribute for the file
 *                  If it is a dir (odd vnode), purge its 
 *                  children from the namecache
 *                  remove the file from the namecache.
 *
 * The sixth allows Venus to replace local fids with global ones
 * during reintegration.
 *
 * CFS_REPLACE -- replace one ViceFid with another throughout the name cache 
 */

int handleDownCall(opcode, out)
     int opcode; union outputArgs *out;
{
    int error;

    /* Handle invalidate requests. */
    switch (opcode) {
      case CFS_FLUSH : {

	  cfs_flush(IS_DOWNCALL);
	  
	  CFSDEBUG(CFS_FLUSH,cfs_testflush();)    /* print remaining cnodes */
	      return(0);
      }
	
      case CFS_PURGEUSER : {
	  cfs_clstat.ncalls++;
	  cfs_clstat.reqs[CFS_PURGEUSER]++;
	  
	  /* XXX - need to prevent fsync's */
	  cfsnc_purge_user(out->cfs_purgeuser.cred.cr_uid, IS_DOWNCALL);
	  return(0);
      }
	
      case CFS_ZAPFILE : {
	  struct cnode *cp;

	  error = 0;
	  cfs_clstat.ncalls++;
	  cfs_clstat.reqs[CFS_ZAPFILE]++;
	  
	  cp = cfs_find(&out->cfs_zapfile.CodaFid);
	  if (cp != NULL) {
	      vref(CTOV(cp));
	      
	      cp->c_flags &= ~C_VATTR;
	      if (CTOV(cp)->v_flag & VTEXT)
		  error = cfs_vmflush(cp);
	      CFSDEBUG(CFS_ZAPFILE, myprintf(("zapfile: fid = (%lx.%lx.%lx), 
                                              refcnt = %d, error = %d\n",
					      cp->c_fid.Volume, 
					      cp->c_fid.Vnode, 
					      cp->c_fid.Unique, 
					      CTOV(cp)->v_usecount - 1, error)););
	      if (CTOV(cp)->v_usecount == 1) {
		  cp->c_flags |= C_PURGING;
	      }
	      vrele(CTOV(cp));
	  }
	  
	  return(error);
      }
	
      case CFS_ZAPDIR : {
	  struct cnode *cp;

	  cfs_clstat.ncalls++;
	  cfs_clstat.reqs[CFS_ZAPDIR]++;
	  
	  cp = cfs_find(&out->cfs_zapdir.CodaFid);
	  if (cp != NULL) {
	      vref(CTOV(cp));
	      
	      cp->c_flags &= ~C_VATTR;
	      cfsnc_zapParentfid(&out->cfs_zapdir.CodaFid, IS_DOWNCALL);     
	      
	      CFSDEBUG(CFS_ZAPDIR, myprintf(("zapdir: fid = (%lx.%lx.%lx), 
                                          refcnt = %d\n",cp->c_fid.Volume, 
					     cp->c_fid.Vnode, 
					     cp->c_fid.Unique, 
					     CTOV(cp)->v_usecount - 1)););
	      if (CTOV(cp)->v_usecount == 1) {
		  cp->c_flags |= C_PURGING;
	      }
	      vrele(CTOV(cp));
	  }
	  
	  return(0);
      }
	
      case CFS_ZAPVNODE : {
	  cfs_clstat.ncalls++;
	  cfs_clstat.reqs[CFS_ZAPVNODE]++;
	  
	  myprintf(("CFS_ZAPVNODE: Called, but uniplemented\n"));
	  /*
	   * Not that below we must really translate the returned coda_cred to
	   * a netbsd cred.  This is a bit muddled at present and the cfsnc_zapnode
	   * is further unimplemented, so punt!
	   * I suppose we could use just the uid.
	   */
	  /* cfsnc_zapvnode(&out->cfs_zapvnode.VFid, &out->cfs_zapvnode.cred,
			 IS_DOWNCALL); */
	  return(0);
      }	
	
      case CFS_PURGEFID : {
	  struct cnode *cp;

	  error = 0;
	  cfs_clstat.ncalls++;
	  cfs_clstat.reqs[CFS_PURGEFID]++;

	  cp = cfs_find(&out->cfs_purgefid.CodaFid);
	  if (cp != NULL) {
	      vref(CTOV(cp));
	      if (ODD(out->cfs_purgefid.CodaFid.Vnode)) { /* Vnode is a directory */
		  cfsnc_zapParentfid(&out->cfs_purgefid.CodaFid,
				     IS_DOWNCALL);     
	      }
	      cp->c_flags &= ~C_VATTR;
	      cfsnc_zapfid(&out->cfs_purgefid.CodaFid, IS_DOWNCALL);
	      if (!(ODD(out->cfs_purgefid.CodaFid.Vnode)) 
		  && (CTOV(cp)->v_flag & VTEXT)) {
		  
		  error = cfs_vmflush(cp);
	      }
	      CFSDEBUG(CFS_PURGEFID, myprintf(("purgefid: fid = (%lx.%lx.%lx), refcnt = %d, error = %d\n",
                                            cp->c_fid.Volume, cp->c_fid.Vnode,
                                            cp->c_fid.Unique, 
					    CTOV(cp)->v_usecount - 1, error)););
	      if (CTOV(cp)->v_usecount == 1) {
		  cp->c_flags |= C_PURGING;
	      }
	      vrele(CTOV(cp));
	  }
	  return(error);
      }

      case CFS_REPLACE : {
	  struct cnode *cp = NULL;

	  cfs_clstat.ncalls++;
	  cfs_clstat.reqs[CFS_REPLACE]++;
	  
	  cp = cfs_find(&out->cfs_replace.OldFid);
	  if (cp != NULL) { 
	      /* remove the cnode from the hash table, replace the fid, and reinsert */
	      vref(CTOV(cp));
	      cfs_unsave(cp);
	      cp->c_fid = out->cfs_replace.NewFid;
	      cfs_save(cp);

	      CFSDEBUG(CFS_REPLACE, myprintf(("replace: oldfid = (%lx.%lx.%lx), newfid = (%lx.%lx.%lx), cp = %p\n",
					   out->cfs_replace.OldFid.Volume,
					   out->cfs_replace.OldFid.Vnode,
					   out->cfs_replace.OldFid.Unique,
					   cp->c_fid.Volume, cp->c_fid.Vnode, 
					   cp->c_fid.Unique, cp));)
	      vrele(CTOV(cp));
	  }
	  return (0);
      }
      default:
      	myprintf(("handleDownCall: unknown opcode %d\n", opcode));
	return (EINVAL);
    }
}

/* cfs_grab_vnode: lives in either cfs_mach.c or cfs_nbsd.c */

int
cfs_vmflush(cp)
     struct cnode *cp;
{
#if	0
  /* old code */
    /* Unset <device, inode> so that page_read doesn't try to use
       (possibly) invalid cache file. */
    cp->c_device = 0;
    cp->c_inode = 0;

    return(inode_uncache_try(VTOI(CTOV(cp))) ? 0 : ETXTBSY);
#else /* __NetBSD__ || __FreeBSD__ */
    return 0;
#endif /* __NetBSD__ || __FreeBSD__ */
}


/* 
 * kernel-internal debugging switches
 */

void cfs_debugon(void)
{
    cfsdebug = -1;
    cfsnc_debug = -1;
    cfs_vnop_print_entry = 1;
    cfs_psdev_print_entry = 1;
    cfs_vfsop_print_entry = 1;
}

void cfs_debugoff(void)
{
    cfsdebug = 0;
    cfsnc_debug = 0;
    cfs_vnop_print_entry = 0;
    cfs_psdev_print_entry = 0;
    cfs_vfsop_print_entry = 0;
}

/*
 * Utilities used by both client and server
 * Standard levels:
 * 0) no debugging
 * 1) hard failures
 * 2) soft failures
 * 3) current test software
 * 4) main procedure entry points
 * 5) main procedure exit points
 * 6) utility procedure entry points
 * 7) utility procedure exit points
 * 8) obscure procedure entry points
 * 9) obscure procedure exit points
 * 10) random stuff
 * 11) all <= 1
 * 12) all <= 2
 * 13) all <= 3
 * ...
 */


#endif	/* NVCFS */
