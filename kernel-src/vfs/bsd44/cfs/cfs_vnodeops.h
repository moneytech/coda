/* 
 * Mach Operating System
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */

/*
 * This code was written for the Coda file system at Carnegie Mellon
 * University.  Contributers include David Steere, James Kistler, and
 * M. Satyanarayanan.  
 */

/*
 * HISTORY
 * $Log: cfs_vnodeops.h,v $
 * Revision 1.1.2.1  1995/12/20 01:57:40  bnoble
 * Added CFS-specific files
 *
 */

int
cfs_open __P((struct vnode **,
	      int,
	      struct ucred *,
	      struct proc *));

int
cfs_close __P((struct vnode *,
	       int,
	       struct ucred *,
	       struct proc *));

int
cfs_rdwr __P((struct vnode *,
	      struct uio *,
	      enum uio_rw,
	      int,
	      struct ucred *,
	      struct proc *));

int
cfs_ioctl __P((struct vnode *,
	       int,
	       caddr_t,
	       int,
	       struct ucred *,
	       struct proc  *));

int
cfs_select __P((struct vnode *,
		int,
		struct ucred *,
		struct proc *));
		
int
cfs_getattr __P((struct vnode *,
		 struct vattr *,
		 struct ucred *,
		 struct proc *));

int
cfs_setattr __P((struct vnode *,
		 register struct vattr *,
		 struct ucred *,
		 struct proc *));

int
cfs_access __P((struct vnode *,
		int,
		struct ucred *,
		struct proc *));
		
int
cfs_readlink __P((struct vnode *,
		  struct uio *,
		  struct ucred *,
		  struct proc *));

int
cfs_fsync __P((struct vnode *,
	       struct ucred *,
	       struct proc *));

int
cfs_inactive __P((struct vnode *,
		  struct ucred *,
		  struct proc *));

int
cfs_lookup __P((struct vnode *,
		char *,
		struct vnode **,
		struct ucred *,
		struct proc *));

int
cfs_create __P((struct vnode *,
		char *,
		struct vattr *,
		enum vcexcl,
		int,
		struct vnode **,
		struct ucred *,
		struct proc *));

int
cfs_remove __P((struct vnode *,
		char *,
		struct ucred *,
		struct proc *));

int
cfs_link __P((struct vnode *,
	      struct vnode *,
	      char *,
	      struct ucred *,
	      struct proc *));

int
cfs_rename __P((struct vnode *,
		char *,
		struct vnode *,
		char *,
		struct ucred *,
		struct proc *));

int
cfs_mkdir __P((struct vnode *,
	       char *,
	       register struct vattr *,
	       struct vnode **,
	       struct ucred *,
	       struct proc *));

int
cfs_rmdir __P((struct vnode *,
	       char *,
	       struct ucred *,
	       struct proc *));

int
cfs_symlink __P((struct vnode *,
		 char *,
		 struct vattr *,
		 char *,
		 struct ucred *,
		 struct proc *));

int
cfs_readdir __P((struct vnode *,
		 register struct uio *,
		 struct ucred *,
		 int *,
		 u_long *,
		 int,
		 struct proc *));

int
cfs_bmap __P((struct vnode *,
	      daddr_t,	
	      struct vnode **,
	      daddr_t *,	
	      struct proc *));

int
cfs_strategy __P((struct buf *,
		  struct proc *));


/*
 * The following don't exist in NetBSD, but are needed in Mach
 */

#ifdef MACH

int
cfs_bread __P((struct vnode *,
	       daddr_t,
	       struct buf **)); 

int
cfs_brelse __P((struct vnode *vp,
		struct buf *));

int
cfs_badop __P(());

int
cfs_noop __P(());

int
cfs_fid __P((struct vnode *,
	     struct fid **))

int
cfs_freefid __P((struct vnode *,
		 struct fid *));

int
cfs_lockctl __P((struct vnode *,
		 struct flock *,
		 int,
		 struct ucred *))

int
cfs_page_read __P((struct vnode	*,
		   caddr_t,
		   int,
		   vm_offset_t,
		   struct ucred *));

int
cfs_page_write __P((struct vnode *,
		    caddr_t,
		    int,
		    vm_offset_t,
		    struct ucred *,
		    boolean_t));

#endif
