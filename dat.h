#define	nil	((void*)(0))
#define PtrSz	(sizeof(void*))
#define StrSz	(32)

//	Env
enum{
	MaxScope	= 64,
	ScopeBltin	= 0,
	ScopeGlobal	= 1,
};

//	Scope
typedef enum{
	Sother,
	Sloop,
	Sscope
}Wscope;

//	Op
typedef enum{
	Onone,
	Oseq,
	Oconst,
	Oname,
	Odas,
	Oend,
}Wop;

//	Decl
typedef enum{
	Dundef,
	Dtype,
	Dlocal,
	Dglobal,
	Dunbound,
	Dend,
}Wdecl;

typedef enum{
	Tu8,
	Tu16,
	Tu32,
	Tu64,
	Ti8,
	Ti16,
	Ti32,
	Ti64,
}Wtype;

//	Type check
typedef enum{
	OKbind		= 1 << 0,
	OKverify	= 1 << 1,
	OKmask		= 0xff,
}Wtok;

typedef	long long	vlong;
typedef	unsigned long	ulong;
typedef	unsigned int	uint;
typedef	unsigned char	uchar;
typedef	unsigned short	ushort;

typedef struct	String	String;
typedef struct	FLine	FLine;
typedef struct	File	File;
typedef struct	Line	Line;
typedef struct	Src	Src;

typedef struct	Sym	Sym;
typedef struct	Token	Token;
typedef struct	Node	Node;
typedef struct	Type	Type;
typedef struct	Decl	Decl;

typedef struct	TypeList	TypeList;

struct String
{
	char	*beg;
	char	*ptr;
	char	*end;
	char	raw[StrSz+1];
	enum{
		Allocated	= 0x01,
		NoAlloc		= 0x02,
		Fixed		= 0x04,
		StrSize		= 32,
	}flag;
};

struct FLine
{
	File*	file;
	int	line;	
};

struct File
{
	String	name;
	int	abs;
	int	off;
	int	in;
};

struct Line
{
	int	line;
	int	pos;
};

struct Src
{
	Line	start;
	Line	stop;
};

struct Sym
{
	int	tok;
	int	hash;
	String	name;
	Decl*	decl;
	Decl*	unbound;
	Sym*	next;
};

struct Decl
{
	Src	src;
	Wscope	scope;
	int	nid;
	int	das;
	int store;
	int offset;
	int tref;
	int flag;
	Sym*	sym;
	Type*	ty;
	Decl*	next;
	Decl*	old;
	Decl*	dot;
	Node*	init;
	Decl*	locals;
	Decl*	link;
};

struct Type
{
	Src	src;
	Wtok	ok;
	Wtype	kind;
	uchar	align;
	uchar	flags;
	Decl*	decl;
	Type*	tof;
	Decl*	ids;
	Node*	val;
	int	size;
	int	len;
	int	offset;
};

struct	TypeList
{
	Type*	t;
	TypeList*	next;
};

struct Node
{
	Src	src;
	Wop	op;
	Node*	l;
	Node*	r;
	Type*	ty;
	Decl*	decl;
	vlong	val;
};

extern Type tu8	;
extern Type tu16;
extern Type tu32;
extern Type tu64;
extern Type ti8;
extern Type ti16;
extern Type ti32;
extern Type ti64;
extern int	scope;
extern Src	nosrc;
extern Node*	tree;