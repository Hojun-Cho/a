#include "yo.h"
#include "y.tab.h"

enum
{
	Leof		= -1,

	HashSize	= 1024,
};

typedef	struct	Keywd	Keywd;
struct Keywd
{
	char*	name;
	int	tok;
};

static	FILE*	bin;
static	Sym*	symbols[HashSize];
static	YYSTYPE	lastyylval;
static	int	curchar;
static	int	lasttok;
static	int	ineof;
static	int	lineno;
static	int	linepos;
static	Keywd	keywds[] =
{
	{"U8",	Lu8},
	{"U16",	Lu16},
	{"U32",	Lu32},
	{"U64",	Lu64},
	{"I8",	Li8},
	{"I16",	Li16},
	{"I32",	Li32},
	{"I64",	Li64},
	0,
};

void*
alloc(uint n)
{
	void* p;

	p = calloc(1, n);
	assert(p != nil);
	return p;	
}

void*
ralloc(void* p, uint n)
{
	p = realloc(p, n);	
	assert(p != nil);
	return p;
}

static int
getch(void)
{
	if(ineof)
		return Leof;
	curchar = fgetc(bin);
	if(curchar == Leof)
		ineof = 1;
	linepos++;
	return curchar;
}

static int
ungetch(void)
{
	assert(ineof == 0);
	ungetc(curchar, bin);	
	curchar = Leof;
	linepos++;
}

Sym*
enter(String* name, int tok)
{
	Sym* s;
	char* p;
	ulong h;
	int c0, c;

	c0 = name->beg[0];
	h = 0;
	for(p = name->beg; p < name->ptr; p++){
		c ^= c << 6;
		h += (c << 11) ^ (c >> 1);
		c = *p;
		h ^= (c << 14) + (c << 7) + (c << 4) + c;
	}
	h %= HashSize;
	for(s = symbols[h]; s; s=s->next){
		if(streq(&s->name, name))
			return s;
	}
	s = alloc(sizeof(Sym));
	strinit(&s->name, name->beg, NoAlloc|Fixed);
	s->hash = h;
	if(tok == 0)
		tok = Lid;
	s->tok = tok;
	s->next = symbols[h];
	symbols[h] = s;
	return s;
}

static int
lexid(int c)
{
	String str;
	Sym* sym;
	int t;

	strinit(&str, nil, NoAlloc);
	for(;;){
		strputc(&str, c);
		c = getch();
		if(c == Leof || (!isalnum(c) && c != '_')){
			ungetch();
			break;
		}
	}	
	sym = enter(&str, Lid);
	t = sym->tok;
	if(t == Lid || t == Ltid)
		yylval.tok.idval = sym;
	return t;
}

static int
lexnum(int c)
{
	char buf[32];
	int i=0, j=0;
	long v;

	buf[i++] = c;
	for(;;){
		c = getch();
		if(c == Leof){
			yyerror("end of file in numeric!");
			return Leof;
		}
		if(isdigit(c) == 0){
			ungetch();
			break;
		}
		assert(i+1 < sizeof(buf));
		buf[i++] = c;
	}
	v = buf[j++]-'0';
	for(; j < i; ++j)
		v = v*10 + buf[j]-'0';
	yylval.tok.ival = v;
	return Lconst;
}

static int
lex(void)
{
	int c;

loop:
	yylval.tok.src.start.line = lineno;
	yylval.tok.src.start.pos = linepos;
	c = getch();
	switch(c){
	case Leof:
		ineof = 1;
		return Leof;
	case '\n':
		lineno++;
		linepos = 0;
		goto loop;
	case ' ':
	case '\t':
	case '\r':
	case '\v':
	case '\f':
		goto loop;
	case ';':
		return c;
	case ':':
		c = getch();
		if(c == '=')
			return Ldas;
		ungetch();
		return '=';
	default:
		if(isdigit(c))
			return lexnum(c);
		if(isalpha(c) || c == '_')
			return lexid(c);
		yyerror("unknown character [%c]", c);
	}
	goto loop;
}

void
yyerror(char *a, ...)
{
	va_list ap;

	va_start(ap, a);
	vfprintf(stderr, a, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	exit(1);
}

int
yylex(void)
{
	int t = lex();
	yylval.tok.src.stop.line = lineno;
	yylval.tok.src.stop.pos = linepos;
	lasttok = t;
	lastyylval = yylval;
	return t;
}

void
lexstart(char* in)
{
	char* p;

	bin = stdin;
	ineof = 0;
	linepos = 0;
	lineno = 1;
}

void
lexinit(void)
{
	for(Keywd* p = keywds; p->name; p++){
		String str = {0,};
		strinit(&str, p->name, NoAlloc|Fixed);
		enter(&str, p->tok);
	}
}