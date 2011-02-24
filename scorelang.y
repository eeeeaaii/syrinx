%{
#include <stdio.h>
#include <stdlib.h>
#include "scorelangclasses.h"

int scyyerror(char* s);
extern int scyylex();

scchunklist *scresult;


%}

%union {
	int ival;
	bracetype bval;
	char* sval;
	scline* lineval;
	scchunk* chunkval;
	scchunklist* chunklistval;
	schilevlist* hilevlistval;
	schilevdirective* hilevdirectiveval;
}
%token <ival> COMMENT
%token <ival> BLANKLINE
%token <ival> START_SECTION_DIRECTION
%token <ival> END_SECTION_DIRECTION
%token <ival> START_HILEVEL_DIRECTION
%token <ival> END_HILEVEL_DIRECTION
%token <ival> START_DIRECTION
%token <ival> END_DIRECTION
%token <ival> START_INSERT
%token <ival> END_INSERT
%token <ival> SPACE
%token <ival> SECTION
%token <ival> SET1
%token <ival> SET2
%token <ival> SET3
%token <ival> SET4
%token <sval> LINE
%token <sval> NAME
%token <sval> VALUE
%token <sval> JOINER

%type <chunkval> linelist
%type <chunkval> section
%type <chunklistval> sectionlist
%type <lineval> line
%type <lineval> direction
%type <lineval> insertion_expression
%type <lineval> insertion
%type <sval> startsection
%type <hilevlistval> hilevlist;
%type <hilevdirectiveval> hilevdirection;
%type <hilevdirectiveval> hilev_direction_contents;
%type <hilevdirectiveval> setstatement;
%type <bval> setkeyword;

%%

score:	sectionlist { scresult = $1; }

sectionlist:	{ $$ = new scchunklist(); }
	|		sectionlist section
	{
		$1->addChunk($2);
		$$ = $1;
	}

section:	startsection hilevlist linelist endsection
	{
		$3->setName($1);
		$3->setHiLevList($2);
		$$ = $3;
	}

startsection: START_SECTION_DIRECTION SECTION SPACE NAME END_SECTION_DIRECTION
	{
		$$ = $4;
	}

endsection:	START_SECTION_DIRECTION END_SECTION_DIRECTION ;

hilevlist:				{ $$ = new schilevlist(); }
	|	hilevlist hilevdirection
	{
		$1->addDir($2);
		$$ = $1;
	}

hilevdirection: START_HILEVEL_DIRECTION hilev_direction_contents END_HILEVEL_DIRECTION
	{ $$ = $2; }


hilev_direction_contents: setstatement			{ $$ = $1; }
	|														{ $$ = 0; }

setstatement:	setkeyword SPACE NAME 	
	{
		scsetdirective* sd = new scsetdirective();
		sd->setType($1);
		sd->setName($3);
		$$ = sd;
	}

setkeyword: SET1	{ $$ = BRACETYPE_PAREN; }
	|			SET2	{ $$ = BRACETYPE_CURLY; }
	|			SET3	{ $$ = BRACETYPE_SQUARE; }
	|			SET4	{ $$ = BRACETYPE_ANGLE; }

linelist: 		{ $$ = new scchunk(); }
	|		 linelist line 
	{
		$1->addLine($2);
		$$ = $1;
	}


line:		LINE				{ $$ = new scmusicline($1); }
	|		direction		{ $$ = $1; }
	|		insertion		{ $$ = $1; }


direction:	START_DIRECTION direction_contents END_DIRECTION
	{
		$$ = new scdirectiveline();
	}

direction_contents:					;


insertion: START_INSERT insertion_expression END_INSERT
	{
		$$ = $2;
	}

insertion_expression: 	NAME			{ $$ = new scinsertionline($1); }
	|		NAME SPACE JOINER SPACE NAME 	{ $$ = new scinsertionline($1, $3, $5); }



%%

extern FILE* scyyin;

#ifdef YSOLO
main()
{
	do {
		scyyparse();
	} while(!feof(scyyin));
	printf("done.\n");
}
#endif

int scyyerror(char* s)
{
	fprintf(stderr, "%s\n", s);
}
