%{
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "scorelangclasses.h"
#include "scorelang.y.h"
/* ^[_a-z0-9*]\n			{ printf("SECONDLINE\n"); } */


/*
=section
should be its own token, so the parser can figure out
what is going on.
*/


int actOnToken(int token);
void printToken(int token);


#ifdef LSOLO
#define PROCESSTOKEN(X) printToken(X)
#else
#define PROCESSTOKEN(X) if(actOnToken(X)) return (X)
#endif

#ifndef LSOLO
#define PASSSTRING \
	{ \
		char *n = new char[strlen(yytext) + 2];\
		sprintf(n, "%s", yytext);\
		scyylval.sval = n;\
	}
#else
#define PASSSTRING ;
#endif


%}
%option noyywrap

%%
^\+\:.*\:\+[ \t]*\n	{ PROCESSTOKEN(COMMENT); /* should ignore */}
^[ \t]*\n				{ PROCESSTOKEN(BLANKLINE); }
^[ .(){}[\]<>]*\n 	{ PASSSTRING ; PROCESSTOKEN(LINE); }
^\+\=[ \t]*				{ PROCESSTOKEN(START_SECTION_DIRECTION); }
[ \t]*\=\+[ \t]*\n 	{ PROCESSTOKEN(END_SECTION_DIRECTION); }
^=[ \t]*					{ PROCESSTOKEN(START_HILEVEL_DIRECTION); }
[ \t]*=[ \t]*\n 		{ PROCESSTOKEN(END_HILEVEL_DIRECTION); }
^-[ \t]*					{ PROCESSTOKEN(START_DIRECTION); }
[ \t]*-[ \t]*\n 		{ PROCESSTOKEN(END_DIRECTION); }
^~[ \t]*					{ PROCESSTOKEN(START_INSERT); }
[ \t]*~[ \t]*\n 		{ PROCESSTOKEN(END_INSERT); }
section					{ PROCESSTOKEN(SECTION); }
set\(						{ PROCESSTOKEN(SET1); }
set\{						{ PROCESSTOKEN(SET2); }
set\[						{ PROCESSTOKEN(SET3); }
set\<						{ PROCESSTOKEN(SET4); }
[a-zA-Z0-9_]+			{ PASSSTRING ; PROCESSTOKEN(NAME); }
\".*\"					{ PASSSTRING ; PROCESSTOKEN(VALUE); }
[0-9]*&[0-9]*			{ PASSSTRING ; PROCESSTOKEN(JOINER); }
[ \t]+					{ PROCESSTOKEN(SPACE); /* we DON'T ignore space */ }

%%

#ifdef LSOLO
main()
{
	yylex();
}
#endif



int actOnToken(int token)
{
	switch(token) {
	case COMMENT: return 0;
	case BLANKLINE: return 0;
	default: return 1;
	}
}



void printToken(int token) {
	switch(token) {
	case COMMENT: printf("COMMENT\n"); break;
	case LINE: printf("LINE\n"); break;
	case BLANKLINE: printf("BLANKLINE\n"); break;
	case START_SECTION_DIRECTION: printf("START_SECTION_DIRECTION\n"); break;
	case END_SECTION_DIRECTION: printf("END_SECTION_DIRECTION\n"); break;
	case START_HILEVEL_DIRECTION: printf("START_HILEVEL_DIRECTION\n"); break;
	case END_HILEVEL_DIRECTION: printf("END_HILEVEL_DIRECTION\n"); break;
	case START_DIRECTION: printf("START_DIRECTION\n"); break;
	case END_DIRECTION: printf("END_DIRECTION\n"); break;
	case START_INSERT: printf("START_INSERT\n"); break;
	case END_INSERT: printf("END_INSERT\n"); break;
	case NAME: printf("NAME\n"); break;
	case SECTION: printf("SECTION\n"); break;
	case SET1: printf("SET1\n"); break;
	case SET2: printf("SET2\n"); break;
	case SET3: printf("SET3\n"); break;
	case SET4: printf("SET4\n"); break;
	case VALUE: printf("VALUE\n"); break;
	case JOINER: printf("JOINER\n"); break;
	case SPACE: printf("SPACE\n"); break;
	default: printf("UNKNOWN, CHECK LIST\n"); break;
	}
}
