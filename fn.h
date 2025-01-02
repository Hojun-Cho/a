//	string.c
void	strgrow(String*, uint);
void	strputc(String*, int);
void	strputs(String*, char*);
void	strcopy(String*, String*, uchar);
void	strinit(String* s, char* p, uchar flag);
int	streq(String*, String*);

//	lex.c
void*	alloc(uint);
void*	ralloc(void*, uint);
void	lexinit(void);
void	lexstart(char*);
Sym*	enter(String*, int);
void	yyerror(char*, ...);
int	yylex(void);

//	node.c
Node*	mknode(int, Node*, Node*);
Node*	mkname(Src*, Sym*);
Node*	mkconst(Src*, long);
Node*	mkbin(int, Node*, Node*);
Node*	rotatenode(Node*);
Node*	mkunary(int, Node*);

//	decls.c
void	declstart(void);
Decl*	popscope(void);
Decl*	mkdecl(Src*, int, Type*);
Decl*	mkids(Src*, Sym*, Type*, Decl*);
void	installids(int, Decl*);
void	dasdecl(Node*);

//	types.c
void 	typeinit(Type*, Line*, Line*, int, Type*, Decl*);
void	typestart(void);

//	typecheck.c
int	isvalidtype(Type*);
int	isvaliddastype(Node*, Type*);
void	echeck(Node*, int, Node*);

