#include "yo.h"

int
strleng(String* a)
{
	return a->ptr-a->beg;	
}

int
streq(String* a, String* b)
{
	if(a == b)
		return 1;
	int l = strleng(a);
	if(l != strleng(b))
		return 0;
	return memcmp(a->beg, b->beg, l)==0;
}

void
strgrow(String* p, uint n)
{
	uint ol;

	if(p->flag & NoAlloc)
		assert(0);
	ol = p->end - p->beg;
	if(p->flag & Allocated)
		p->beg = ralloc(p->beg, ol*n+1);
	else{
		p->beg =alloc(ol*n+1);	
		memcpy(p->beg, p->raw, ol);
		p->flag |= Allocated;
	}
	p->ptr = p->beg+ol-1;
	p->end = p->beg+ol*n;
}

void
strputc(String* s, int c)
{
	if(s->ptr + 1 >= s->end)
		strgrow(s, 2);
	*s->ptr++ = c;
	*s->ptr = 0;
}

void
strputs(String* s, char* p)
{
	assert((s->flag&Fixed) == 0);

	for(char c=*p++; c; c=*p++){
		if(s->ptr + 1 >= s->end)
			strgrow(s, 2);
		*s->ptr++ = c;
		*s->ptr = 0;
	}
}

void
strinit(String* s, char* p, uchar flag)
{
	s->ptr = s->beg = s->raw;
	s->end = &s->raw[sizeof(s->raw)];
	if(p)
		strputs(s, p);	
	s->flag = flag;
}