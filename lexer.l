%{

	/* test */

%}

%%

[\t ]+		; /* ignore whitespace */


[a-zA-Z_]+:[abcdefgABCDEFG](#|b)?[0123456789]{1,3}	{printf("%s:note specifier1\n", yytext);}
[a-zA-Z_]+:														{printf("%s:note specifier2\n", yytext);}
:[abcdefgABCDEFG](#|b)?[0123456789]{1,3}				{printf("%s:note specifier3\n", yytext);}


[a-zA-Z_]+	{printf("%s:identifier\n", yytext);}
\[				{printf("open bracket");}
\]				{printf("close bracket");}




%%

main()
{
	yylex();
}
