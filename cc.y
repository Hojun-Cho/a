%{
	#include "yo.h"
%}

%union
{
	struct{
		Src	src;
		union{
			Sym*	idval;
			long	ival;
		};
	}tok;
	Decl*	ids;
	Node*	node;
	Type*	type;
	TypeList*	types;
}

%type	<node>	expr idatom
				topdecls topdecl
%right	<tok.src>	Ldas

%token	Lu8 Lu16 Lu32 Lu64 Li8 Li16 Li32 Li64

%nonassoc	<tok.ival>	Lconst
%nonassoc	<tok.idval>	Lid Ltid Lsconst

%%
prog
	:topdecls{
		tree = rotatenode($1);
	}

topdecls
	:topdecl
	|topdecls topdecl {
		if($1 == nil)
			$$ = $2;
		else if($2 == nil)
			$$ = $1;
		else
			$$ = mkbin(Oseq, $1, $2);
	}
	;

topdecl
	:idatom Ldas expr ';'{
		$$ = mkbin(Odas, $1, $3);
	}
	;

idatom
	:Lid {
		$$ = mkname(&$<tok.src>1, $1);
	}
	;

expr
	: Lconst{
		$$ = mkconst(&$<tok.src>1, $1);
	}
	;

%%

Node*	tree;

void
main(int argc, char *argv[])
{
	lexinit();

	lexstart(argv[1]);	
	popscope();
	typestart();
	declstart();

	yyparse();
	echeck(tree, 0, nil);
}