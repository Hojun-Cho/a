#include "yo.h"

int
isvalidtype(Type* t)
{
	assert(t != nil);
	switch(t->kind){
	case Tu8:
	case Tu16:
	case Tu32:
	case Tu64:
	case Ti8:
	case Ti16:
	case Ti32:
	case Ti64:
		return 1;
	default:
		return 0;
	}
}

int
isvaliddastype(Node* n, Type* t)
{
	assert(n->op == Oname);
	assert(n->decl);
	n->decl->ty = t;
	n->ty = t;
	return 1;
}

void
echeck(Node* n, int isglobal, Node* par)
{
	if(n == nil)
		return;
	if(n->op == Oseq){
		for(; n && n->op == Oseq; n = n->r){
			echeck(n->l, isglobal, n);
			n->ty = nil;
		}
		if(n == nil)
			return;
	}

	Node *l=n->l, *r=n->r;
	switch(n->op){
	case Odas:
		assert(r->ty != nil);
		echeck(r, isglobal, n);
		dasdecl(l);
		assert(isvalidtype(r->ty));
		assert(isvaliddastype(l, r->ty));
		l->ty = n->ty = r->ty;
		return;
	case Oconst:
		assert(n->ty != nil);
		break;
	default:
		assert(0);
	}
}