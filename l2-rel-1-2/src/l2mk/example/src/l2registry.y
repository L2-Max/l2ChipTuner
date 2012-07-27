%{
    #include "l2registry_parser_api.h"
%}

%token IDENTIFIER
%token COMMENT

%union{
    char id[MAX_ID_NAME_LEN+1];
    int ih;
    double rl;
    char chr;
    char str[MAX_STRING_BUF_LEN+1];
    struct SNode* node;
};

%token <id>IDENTIFIER
%token <ih>INT
%token <ih>HEX
%token <rl>REAL
%token <chr>CHARACTER
%token <str>STRING

%type <node>scope operator l_value r_value string binary

%%

prog:
    |scope				{save_scopes($1);}
    ;

scope:
    operator				{$$=add_scope(0,$1,0);}
    |operator scope			{$$=add_scope(0,$1,$2);}
    |IDENTIFIER '{' scope '}'		{$$=add_scope($1,$3,0);}
    |IDENTIFIER '{' scope '}' scope	{$$=add_scope($1,$3,$5);}
    ;

operator:
    l_value '=' r_value ';'	{$$=add_value($1,$3);}
    ;

l_value:
    IDENTIFIER		{$$=add_identifier($1);}
    ;
    
r_value:
    INT			{$$=add_integer($1);}
    |binary		{$$=$1;}
    |REAL		{$$=add_real($1);}
    |CHARACTER		{$$=add_char($1);}
    |string		{$$=$1;}
    ;

string:
    STRING		{$$=add_string($1,0);}
    |STRING string	{$$=add_string($1,$2);}
    ;

binary:
    HEX			{$$=add_hex($1,0);}
    |HEX binary		{$$=add_hex($1,$2);}
    ;
%%
