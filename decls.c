#include "yo.h"

static	Decl*	scopes[MaxScope];
static	Decl*	tails[MaxScope];
static	Node*	scopenode[MaxScope];
static	uchar	scopekind[MaxScope];
int	scope;
Src	nosrc;

static void
recmark(Node* n, int nid)
{
	assert(n->op == Oname);
	n->decl->nid = nid;
	nid = 0;
}

static void
recdasdecl(Node* n, int store, int* nid)
{
	Decl *d;

	assert(n->op == Oname);
	assert(n->decl != nil);
	d = mkids(&n->src, n->decl->sym, nil, nil);
	installids(store, d);
	assert(d->old == nil);
	n->decl = d;
	d->das = 1;
	if(*nid >= 0)
		(*nid)++;
}

void
dasdecl(Node* n)
{
	int nid=0;
	int store = scope==ScopeGlobal?Dglobal:Dlocal;

	recdasdecl(n, store, &nid);
	if(store == Dlocal && nid > 1)
		recmark(n, nid);
}

Decl*
mkdecl(Src* src, int store, Type* t)
{
	Decl* d = alloc(sizeof(Decl));
	d->src = *src;
	d->store = store;
	d->ty = t;
	d->nid = 1;
	return d;
}

Decl*
mkids(Src* src, Sym* s, Type* t, Decl* next)
{
	Decl* d = mkdecl(src, Dundef, t);
	d->next = next;
	d->sym = s;
	return d;
}

void
installids(int store, Decl* ids)
{
	Sym* s;
	Decl* d;
	Decl* last = nil;	

	for(d = ids; d; d = d->next){
		d->scope = scope;
		if(d->store == Dundef)
			d->store = store;
		s = d->sym;
		if(s){
			assert(s->decl == nil || s->decl->scope < scope);
			d->old = s->decl;
			s->decl = d;
		}
		last = d;
	}
	if(ids){
		d = tails[scope];
		if(d == nil)
			scopes[scope] = ids;
		else
			d->next = ids;
		tails[scope] = last;
	}
}

void
pushscope(Node* s, int kind)
{
	assert(scope < MaxScope);

	scope++;
	scopes[scope] = nil;
	tails[scope] = nil;
	scopenode[scope] = s;
	scopekind[scope] = kind;	
}

Decl*
popscope(void)
{
	for(Decl* id = scopes[scope]; id; id = id->next){
		if(id->sym){
			id->sym->decl = id->old;
			id->old = nil;
		}
	}
	return scopes[scope--];
}

void
declstart(void)
{
	scope = ScopeGlobal;
	scopes[ScopeGlobal]= nil;
	tails[ScopeGlobal] = nil;
}