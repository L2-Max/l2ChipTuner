%{
    #include "l2registrydefs.h"
    #include "y.tab.h"
%}

digit   [0-9]
alpha   [A-Za-z_]
char	.

identifier {alpha}({alpha}|{digit})*

int		[-]?{digit}{1,9}
hex     	"0x"(({digit}|[A-Fa-f]){1,8})
real		[-]?(({digit}*\.{digit}+)|({digit}+\.{digit}*))
character	\'{char}\'
string		("\"")({char}*)("\"")
comment		("\\\\".*\n)|("\\*".*"*\\")
%%

{identifier}	{
		    if(yyleng>MAX_ID_NAME_LEN)
                    	return yyerror("Identifier too big");
		    
		    strcpy(yylval.id,yytext);
		    return IDENTIFIER;
		}

{int}		{
		    sscanf(yytext,"%d",&yylval.ih);
		    return INT;
		}

{hex}           {
		    sscanf(yytext,"%x",&yylval.ih);
		    return HEX;
		}

{real}		{
		    sscanf(yytext,"%lf",&yylval.rl);
		    return REAL;
		}

{character}	{
		    yylval.chr=yytext[1];
		    return CHARACTER;
		}
		
{string}	{
		    if((yyleng-2)>MAX_STRING_BUF_LEN)
			return yyerror("String too big");

		    strncpy(yylval.str,yytext+1,yyleng-2);
		    return STRING;
		}

{comment}	{
			return COMMENT;
		}

[={};]		return *yytext;
[ \t\n\r]+	;
.		return yyerror(yytext);

%%
int yywrap(){
    return 1;
}

int yyerror(char* err){
    printf("Line: %d. %s.\n",yylineno,err);
    return 0;
}
