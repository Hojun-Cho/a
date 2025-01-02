#include "yo.h"

Node*
mknode(int op, Node* l, Node* r)
{
	Node *n = alloc(sizeof(Node));
	n->op = op;
	n->l = l;
	n->r = r;
	return n;	
}

Node*
mkbin(int op, Node* l, Node* r)
{
	Node *n = mknode(op, l, r);
	n->src.start = l->src.start;
	n->src.stop = r->src.stop;	
	return n;
}

Node*
mkname(Src* src, Sym* sym)
{
	Node *n = mknode(Oname, nil, nil);
	n->src = *src;
	if(sym->unbound == nil){
		sym->unbound = mkdecl(src, Dunbound, nil);
		sym->unbound->sym = sym;
	}
	n->decl = sym->unbound;
	return n;
}

Node*
mkconst(Src* src, long v)
{
	Node *n = mknode(Oconst, nil, nil);
	n->ty = &ti32;	
	n->val = v;
	n->src = *src;
	return n;
}

Node*
mkunary(int op, Node* l)
{
	Node* n = mknode(op, l, nil);
	n->src = l->src;
	return n;	
}

//	Oseq(Oseq(a, b), c) => Oseq(a, Oseq(b, Oseq(c, nil)))
Node*
rotatenode(Node* e)
{
	if(e == nil)
		return nil;
	if(e->op != Oseq)
		return mkunary(Oseq, e);
	e->r = mkunary(Oseq, e->r);
	while(e->l->op == Oseq){
		Node* l = e->l;
		e->l = l->r;
		l->r = e;
		e = l; 
	}
	return e;
}