#include "yo.h"

Type tu8	= {.kind=Tu8,	.size=1,	.align=1,	.ok=OKmask};
Type tu16	= {.kind=Tu16,	.size=2,	.align=2,	.ok=OKmask};
Type tu32	= {.kind=Tu32,	.size=4,	.align=4,	.ok=OKmask};
Type tu64	= {.kind=Tu64,	.size=8,	.align=8,	.ok=OKmask};
Type ti8	= {.kind=Ti8,	.size=1,	.align=1,	.ok=OKmask};
Type ti16	= {.kind=Ti16,	.size=2,	.align=2,	.ok=OKmask};
Type ti32	= {.kind=Ti32,	.size=4,	.align=4,	.ok=OKmask};
Type ti64	= {.kind=Ti64,	.size=8,	.align=8,	.ok=OKmask};

void
typeinit(Type* t, Line* start, Line* stop, int kind, Type* tof, Decl* args)
{
	memset(t, 0, sizeof(*t));
	t->src.start = *start;
	t->src.stop = *stop;
	t->kind = kind;
	t->tof = tof;
	t->ids = args;
}

static void
bltintype(char *name, Type* t)
{
	Sym* sym;
	Decl* d;
	String str = {0,};

	strinit(&str, name, NoAlloc|Fixed);
	sym = enter(&str, 0);
	d = mkids(&nosrc, sym, nil, nil);
	d->ty = t;
	t->decl = d;	
	installids(Dtype, d);
}

void
typestart(void)
{
	bltintype("U8", &tu8);
	bltintype("U16", &tu16);
	bltintype("U32", &tu32);
	bltintype("U64", &tu64);
	bltintype("I8", &ti8);
	bltintype("I16", &ti16);
	bltintype("I32", &ti32);
	bltintype("I64", &ti64);
}