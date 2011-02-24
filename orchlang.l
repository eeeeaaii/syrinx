%{
#include "orchlangclasses.h"
#include "OrchTranslator.h"
#include "OrchFunction.h"
#include "OrchExpression.h"
#include "OrchConstructor.h"
#include "orchlang.y.h"
%}


%option noyywrap

%%

[\t\n ]+		; /* ignore whitespace including newline */
\>						{ return RIGHTCARAT; }
\<						{ return LEFTCARAT; }
\/						{ return FSLASH; }
\\						{ return BSLASH; }
\{						{ return OPENBRACE; }
\}						{ return CLOSEBRACE; }
\|						{ return PIPE; }
:						{ return COLON; }
=						{ return EQUALS; }
\[\[					{ return OPENBLOCK; }
\]\]					{ return CLOSEBLOCK; }
\"[^"]*\"  			{ char* n = new char[strlen(yytext) + 2]; sprintf(n, "%s", yytext); yylval.sval = n; return YSTRING; }
\@[a-zA-Z0-9_]+		{ char* n = new char[strlen(yytext) + 2]; sprintf(n, "%s", yytext); yylval.sval = n; return NAME; }
\@\@[a-zA-Z0-9_]+		{ char* n = new char[strlen(yytext) + 2]; sprintf(n, "%s", yytext); yylval.sval = n; return METANAME; }
\#[a-zA-Z0-9_]+		{ char* n = new char[strlen(yytext) + 2]; sprintf(n, "%s", yytext); yylval.sval = n; return METAYSTRING; }
[a-zA-Z0-9_]+		{ char* n = new char[strlen(yytext) + 2]; sprintf(n, "%s", yytext); yylval.sval = n; return IDENTIFIER; }

%%

