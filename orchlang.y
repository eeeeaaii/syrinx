%{
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include "orchlangclasses.h"
#include "OrchExpression.h"
#include "OrchFunction.h"
#include "OrchConstructor.h"
#include "OrchTranslator.h"

using namespace std;

OrchTranslator* gl_orchTranslator;

int yyerror(char* s);
extern int yylex();

%}

%union {
	int ival;
	char* sval;
	OrchFunction *ofval;
	OrchTranslator *otval;
	OrchExpression *oeval;
	OrchConstructor *ocval;
	stringlist* slval;
	pairlist* plval;
	oelist* oelval;

}
%token <ival> RIGHTCARAT
%token <ival> LEFTCARAT
%token <ival> BSLASH
%token <ival> FSLASH
%token <ival> OPENBRACE
%token <ival> CLOSEBRACE
%token <ival> OPENBLOCK
%token <ival> CLOSEBLOCK
%token <ival> PIPE
%token <ival> COLON
%token <ival> EQUALS
%token <sval> YSTRING
%token <sval> METAYSTRING
%token <sval> NAME
%token <sval> METANAME
%token <sval> IDENTIFIER

%type <plval> stringattribute
%type <plval> linkattribute
%type <plval> metalinkattribute
%type <plval> metastringattribute
%type <plval> attribute
%type <plval> attributelist
%type <ocval> moduledefinition
%type <oeval> moduledeclaration
%type <oeval> anonymousmoduledeclaration
%type <oeval> modulename
%type <oeval> modulereference
%type <oelval> inputdefinition
%type <oelval> inputlist
%type <oeval> input
%type <oelval> outputdefinition
%type <oelval> outputlist
%type <oeval> output
%type <oeval> definition
%type <ofval> definitionlist
%type <ofval> moddef
%type <otval> moddeflist
%type <slval> inparamlist
%type <slval> inparams
%type <slval> outparamlist
%type <slval> outparams
%type <plval> attdef
%type <plval> attdeflist
%type <plval> metalinkattdef
%type <plval> metastringattdef

%%

orchestra:	moddeflist 				{ gl_orchTranslator = $1; }

moddeflist:	moddef moddeflist		{ $2->addFunction($1); $$ = $2;}
	|										{ $$ = new OrchTranslator();}

moddef: inparams IDENTIFIER outparams attdeflist OPENBLOCK definitionlist CLOSEBLOCK
	{
		OrchFunction *of = $6;
		of->setName(string($2));
		delete [] $2;
		stringlist *sl;
		for (sl = $1 ; sl != 0 ; sl = sl->next) {
			of->addInputParam(sl->s);
		}
		delete $1;
		for (sl = $3 ; sl != 0 ; sl = sl->next) {
			of->addOutputParam(sl->s);
		}
		delete $3;
		pairlist *pl;
		for (pl = $4 ; pl != 0 ; pl = pl->next) {
			of->addAttributeParam(pl->first, pl->second);
		}
		delete $4;
		$$ = of;
	}

inparams: inparamlist PIPE			{ $$ = $1; }
	|										{ $$ = 0; }

outparams: PIPE outparamlist		{ $$ = $2; }
	|										{ $$ = 0; }

inparamlist: METANAME inparamlist  { $$ = new stringlist() ; $$->s = string($1); delete[] $1; $$->next = $2; }
	|										{	$$ = 0; }

outparamlist: METANAME outparamlist { $$ = new stringlist() ; $$->s = string($1); delete[] $1 ; $$->next = $2; }
	|											{ $$ = 0; }

attdeflist: attdef attdeflist		{ $1->next = $2; $$ = $1; }
	|										{ $$ = 0; }

attdef:	metalinkattdef				{ $$ = $1;}
	|		metastringattdef			{ $$ = $1;}

metalinkattdef: IDENTIFIER EQUALS METANAME
	{
		string nm($1);
		string val($3);
		delete[] $1;
		delete[] $3;
		$$ = new pairlist();
		$$->first = nm;
		$$->second = val;
	}

metastringattdef: IDENTIFIER EQUALS METAYSTRING	
	{
		string nm($1);
		string val($3);
		delete[] $1;
		delete[] $3;
		$$ = new pairlist();
		$$->first = nm;
		$$->second = val;
	}

definitionlist: definition definitionlist { $2->addExpression($1); $$ = $2; }
	|								 					{ $$ = new OrchFunction(); }

definition: inputdefinition modulereference outputdefinition
	{
		OrchExpression *oe = $2;
		oelist *oel;
		for ( oel = $1 ; oel != 0 ; oel = oel->next) {
			oe->addInput(oel->oe);
		}
		delete oel;
		for ( oel = $3 ; oel != 0 ; oel = oel->next) {
			oe->addOutput(oel->oe);
		}
		delete oel;
		$$ = oe;
	}

inputdefinition:	RIGHTCARAT inputlist FSLASH { $$ = $2; }
	|														{ $$ = 0; }

outputdefinition:	BSLASH outputlist LEFTCARAT	{ $$ = $2; }
	|																{ $$ = 0; }

inputlist: 													{ $$ = 0; }	
	|			input inputlist				{ $$ = new oelist(); $$->oe = $1; $$->next = $2; }

input:		definition						{ $$ = $1; }
	|			METANAME
	{
		string s($1);
		delete [] $1;
		$$ = new OrchExpressionReference();
		$$->setName(s);
	}

outputlist: 														{ $$ = 0; }	
	|			output outputlist		{ $$ = new oelist(); $$->oe = $1; $$->next = $2; }

output:		definition						{ $$ = $1; }
	|			METANAME
	{
		string s($1);
		delete [] $1;
		$$ = new OrchExpressionReference();
		$$->setName(s);
	}


modulereference:	modulename				
	|					moduledeclaration		
	|					anonymousmoduledeclaration		
	{
		//cout << "modulereference\n";
		$$ = $1;
	}

modulename:		NAME
	{
		//cout << "modulename\n";
		string s($1);
		delete [] $1;
		$$ = new OrchExpressionReference();
		$$->setName(s);
	}

moduledeclaration:	NAME COLON moduledefinition
	{
		//cout << "moduledeclaration\n";
		//cout << $1 << "\n";
		string s($1);
		delete [] $1;
		$$ = new OrchExpressionComplete();
		$$->setName(s);
		$$->setConstructor($3);
	}

anonymousmoduledeclaration:	moduledefinition
	{
		//cout << "anonymousmoduledeclaration\n";
		$$ = new OrchExpressionAnonymous();
		$$->setConstructor($1);
	}


moduledefinition:	OPENBRACE IDENTIFIER attributelist CLOSEBRACE	
	{
		//cout << "moduledefinition\n";
		OrchConstructor *oc = new OrchConstructor();
		pairlist *p;
		for (p = $3 ; p != 0 ; p = p->next) {
			oc->addAttribute(p->first, p->second);
		}
		oc->setName(string($2));
		delete [] $2;
		$$ = oc;
	}

attributelist:									{ $$ = 0; }
	|				attributelist attribute { $2->next = $1; $$ = $2; }

attribute:	stringattribute	
	|			linkattribute				
	|			metalinkattribute				
	|			metastringattribute				
		{ $$ = $1; }

linkattribute:		IDENTIFIER EQUALS NAME
	{
			$$ = new pairlist();
			$$->first = string($1);
			$$->second = string($3);
			delete [] $1 ;
			delete [] $3 ;
	}

stringattribute:	IDENTIFIER EQUALS YSTRING	
		{
			$$ = new pairlist();
			$$->first = string($1);
			$$->second = string($3);
			delete [] $1 ;
			delete [] $3 ;
		}

metalinkattribute:		IDENTIFIER EQUALS METANAME
	{
			$$ = new pairlist();
			$$->first = string($1);
			$$->second = string($3);
			delete [] $1 ;
			delete [] $3 ;
	}

metastringattribute:	IDENTIFIER EQUALS METAYSTRING	
		{
			$$ = new pairlist();
			$$->first = string($1);
			$$->second = string($3);
			delete [] $1 ;
			delete [] $3 ;
		}


%%

extern FILE* yyin;

int yyerror(char* s)
{
	fprintf(stderr, "%s\n", s);
}
