/* BLURB gpl

                           Coda File System
                              Release 5

          Copyright (c) 1987-1999 Carnegie Mellon University
                  Additional copyrights listed below

This  code  is  distributed "AS IS" without warranty of any kind under
the terms of the GNU General Public Licence Version 2, as shown in the
file  LICENSE.  The  technical and financial  contributors to Coda are
listed in the file CREDITS.

                        Additional copyrights
                           none currently

#*/

#ifdef __cplusplus
extern "C" {
#endif __cplusplus


#include <unistd.h>
#include <string.h>
#ifdef __linux__
#include <sys/vfs.h>
#endif
#ifdef  __BSD44__
#include <sys/param.h>
#include <sys/mount.h>
#endif
#include <sys/file.h>

#ifdef __cplusplus
}
#endif __cplusplus

#include <util.h>
#include <lwp.h>
#include <lock.h>
#include <util.h>
#include "partition.h"

static void DP_InitPartition(Partent entry, struct inodeops *operations,
	       union PartitionData *data, Device devno);

/* 
 * operations on partitions not involving volumes
 * this depends on vicetab.h and inodeops.h
 */

struct dllist_head DiskPartitionList;

static struct inodeops *DP_InodeOpsByType(char *type);
/* InitPartitions reads the vicetab file. For each server partition it finds
   on the invoking host it initializes this partition with the correct
   method.  When the partition has been initialized successfully, it is
   inserted in the DiskPartitionList, a linked list of DiskPartitions.
   */
void DP_Init(const char *tabfile)
{
    char myname[256];
    int rc;
    Partent entry;
    FILE *tabhandle;
    struct inodeops *operations;
    union PartitionData *data;
    Device  devno, codadev;

    codadev = 1;
    list_head_init(&DiskPartitionList);

    tabhandle = Partent_set(tabfile, "r");
    if ( !tabhandle ) {
	eprint("No file vicetab file %s found.\n", tabfile);
	CODA_ASSERT(0);
    }

    while ( (entry = Partent_get(tabhandle)) ) {
        if ( ! UtilHostEq(hostname(myname), Partent_host(entry)) ) {
	    Partent_free(&entry);
	    continue;
	}

	operations = DP_InodeOpsByType(Partent_type(entry));
	if ( !operations ) {
	    eprint("Partition entry %s, %s has unknown type %s.\n",
		   Partent_host(entry), Partent_dir(entry), 
		   Partent_type(entry));
	    CODA_ASSERT(0);
	}

	if ( operations->init ) {
	    rc = operations->init(&data, entry, &devno);
	    if ( rc != 0 ) {
		eprint("Partition entry %s, %s had initialization error.\n");
		CODA_ASSERT(0);
	    }
	}

	/* the devno is written to RVM storage in the vnodes - 
	   whatever scheme for numbering partitions is used should 
	   take note of this */
        DP_InitPartition(entry, operations, data, devno);
	codadev++; 
    }
    Partent_end(tabhandle);

    /* log the status */
    DP_PrintStats(stdout);
}

static void
DP_InitPartition(Partent entry, struct inodeops *operations,
	       union PartitionData *data, Device devno)
{
    struct DiskPartition *dp, *pp;
    struct dllist_head *tmp;

    /* allocate */
    dp = (struct DiskPartition *) malloc(sizeof (struct DiskPartition));
    if ( ! dp ) {
	eprint("Out of memory\n");
	CODA_ASSERT(0);
    }
    bzero(dp, sizeof(struct DiskPartition));
    list_head_init(&dp->dp_chain);

    
    tmp = &DiskPartitionList;
    while ((tmp = tmp->next) != &DiskPartitionList) {
	    pp = list_entry(tmp, struct DiskPartition, dp_chain);
	    if ( pp->device == devno ) {
		    eprint("Device %d requested by partition %s used by %s!\n",
			   devno, Partent_dir(entry), pp->name);
		    CODA_ASSERT(0);
	    }
    }

    /* Add it to the end.  Preserve order for printing. Check devno. */
    list_add(&dp->dp_chain, DiskPartitionList.prev);
    /*  fill in the structure */
    strncpy(dp->name, Partent_dir(entry), MAXPATHLEN);
    dp->device = devno;
    dp->ops = operations;
    dp->d = data;

    DP_SetUsage(dp);
}

struct DiskPartition *
DP_Find(Device devno)
{
    struct DiskPartition *dp;
    struct dllist_head *tmp;

    tmp = &DiskPartitionList;
    while( (tmp = tmp->next) != &DiskPartitionList) {
	    dp = list_entry(tmp, struct DiskPartition, dp_chain);
	    if (dp->device == devno)
		    break;
    }
    if (dp == NULL) {
	    SLog(0, "FindPartition Couldn't find partition %d", devno);
    }
    return dp;	
}

struct DiskPartition *
DP_Get(char *name)
{
    struct DiskPartition *dp;
    struct dllist_head *tmp;

    tmp = &DiskPartitionList;
    dp = list_entry(tmp, struct DiskPartition, dp_chain);
    
    while((dp) && (strcmp(dp->name, name) != 0) &&
	  ((tmp = tmp->next) != &DiskPartitionList)) {
	    dp = list_entry(tmp, struct DiskPartition, dp_chain);
    }

    if ((strcmp(dp->name,name)) != 0)
	    dp = NULL;
    if (dp == NULL) {
	VLog(0, "VGetPartition Couldn't find partition %s", name);
    }
    return dp;	
}


void 
DP_SetUsage(register struct DiskPartition *dp)
{
#if defined(__CYGWIN32__) || defined(DJGPP)
    dp->free = 10000000;  /* free blocks for non s-users */
    dp->totalUsable = 10000000; 
    dp->minFree = 10;
#else
    struct statfs fsbuf;
    int rc;
    long reserved_blocks;

    rc = statfs(dp->name, &fsbuf);
    if ( rc != 0 ) {
	eprint("Error in statfs of %s\n", dp->name);
	perror("");
	CODA_ASSERT( 0 );
    }
    
    reserved_blocks = fsbuf.f_bfree - fsbuf.f_bavail; /* reserved for s-user */
    dp->free = fsbuf.f_bavail;  /* free blocks for non s-users */
    dp->totalUsable = fsbuf.f_blocks - reserved_blocks; 
    dp->minFree = 100 * reserved_blocks / fsbuf.f_blocks;

#endif
}

void 
DP_ResetUsage() 
{
    struct DiskPartition *dp;
    struct dllist_head *tmp;

    tmp = &DiskPartitionList;
    while( (tmp = tmp->next) != &DiskPartitionList) {
	    dp = list_entry(tmp, struct DiskPartition, dp_chain);
	    DP_SetUsage(dp);
	    LWP_DispatchProcess();
    }
}

void 
DP_PrintStats(FILE *fp) 
{
    struct DiskPartition *dp;
    struct dllist_head *tmp;

    tmp = &DiskPartitionList;
    while( (tmp = tmp->next) != &DiskPartitionList) {
	    dp = list_entry(tmp, struct DiskPartition, dp_chain);
	    if (dp->free >= 0)
		    SLog(0, "Partition %s: %dK available (minfree=%d%%), %dK free.",
			 dp->name, dp->totalUsable, dp->minFree, dp->free);
	    else
		    SLog(0, "Partition %s: %dK available (minfree=%d%%), overallocated %dK.",
			 dp->name, dp->totalUsable, dp->minFree, dp->free);
    }
}

void 
DP_LockPartition(char *name)
{
    register struct DiskPartition *dp = DP_Get(name);
    CODA_ASSERT(dp != NULL);
    if (dp->lock_fd == -1) {
	dp->lock_fd = open(dp->name, O_RDONLY, 0);
	CODA_ASSERT(dp->lock_fd != -1);
	CODA_ASSERT (flock(dp->lock_fd, LOCK_EX) == 0);
    }
}

void 
DP_UnlockPartition(char *name)
{
    register struct DiskPartition *dp = DP_Get(name);
    CODA_ASSERT(dp != NULL);
    close(dp->lock_fd);
    dp->lock_fd = -1;
}


static struct inodeops *DP_InodeOpsByType(char *type) 
{
  
    if ( strcmp(type, "simple") == 0  ) {
	return &inodeops_simple;
    }

    if( strcmp(type, "ftree") == 0 ) {
	return &inodeops_ftree;
    }

    if( strcmp(type, "backup") == 0 ) {
	return &inodeops_backup;
    }

    return NULL;
}
