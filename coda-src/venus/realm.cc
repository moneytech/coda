#include <stdio.h>
#include <string.h>
#include <rvmlib.h>
#include "rec_dllist.h"
#include "realm.h"
#include "server.h"

Realm::Realm(const char *rname, struct dllist_head *h) : PersistentObject(h)
{
    int len = strlen(rname) + 1;

    RVMLIB_REC_OBJECT(name);
    name = (char *)rvmlib_rec_malloc(len); 
    CODA_ASSERT(name);
    strcpy(name, rname);

    RVMLIB_REC_OBJECT(id);
#warning "realm.id"
    id = 0;

    /* better keep a reference until volumes/VDBs can hold a reference on this
     * realm... */
    Rec_GetRef();

    ResetTransient();
}

void Realm::ResetTransient(void)
{
    list_head_init(&servers);

    PersistentObject::ResetTransient();
}

Realm::~Realm(void)
{
    struct dllist_head *p;
    Server *s;

    rvmlib_rec_free(name); 

    for (p = servers.next; p != &servers; ) {
	s = list_entry(p, Server, servers);
	p = p->next;
	s->PutRef();
    }
    list_del(&servers);
}

Server *Realm::GetServer(struct in_addr *host)
{
    struct dllist_head *p;
    Server *s;

    CODA_ASSERT(host != 0);
    CODA_ASSERT(host->s_addr != 0);

    list_for_each(p, servers) {
	s = list_entry(p, Server, servers);
	if (s->ipaddr()->s_addr == host->s_addr) {
	    s->GetRef();
	    return s;
	}
    }

    s = new Server(host, &servers, this);

    return s;
}

#if 0
volent *Realm::GetVolume(const char *volname)
{
    volent *v;

    VDB->Get(&v, volname);

#if 0
    list_for_each(p, repvols) {
	v = list_entry(p, volent, volumes);
	if (strcmp(v->volname, volname) == 0) {
	    v->GetRef();
	    return v;
	}
    }
    list_for_each(p, volreps) {
	v = list_entry(p, volent, volumes);
	if (strcmp(v->volname, volname) == 0) {
	    v->GetRef();
	    return v;
	}
    }

    Recov_BeginTrans();
    v = new volent(volname, &volumes);
    Recov_EndTrans(0);
#endif

    return v;
}
#endif

void Realm::print(FILE *f)
{
    fprintf(f, "%08x realm '%s'\n", id, name);
}


