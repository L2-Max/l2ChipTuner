%{
   #include <l2LexScaner.h>
   #include <l2CategoryParserDriver.h>
%}

%skeleton "lalr1.cc"

%name-prefix = "l2max"

%parse-param { Registry::Driver::CCategoryParserDriver& aDriver }
%lex-param { Registry::Driver::CCategoryParserDriver& aDriver }

%locations
%initial-action
{
   @$.begin.filename = @$.end.filename = &aDriver.file();
};

%union
{
   std::string*      _identifier;
   
   l2max::CVariant*  _literalConstant;
   std::string*      _literalConstantString;
   CByteArray*       _literalConstantBinary;
   
   Registry::Driver::TCategory* _category;
   Registry::Driver::TVariable* _variable;
};

%token TOKEN_COMMENT_BEGIN
%token TOKEN_COMMENT_END
%token TOKEN_CATEGORY_IDETIFIER_BEGIN
%token TOKEN_CATEGORY_IDETIFIER_END

%token TOKEN_OPERATOR_ASSIGN

%token TOKEN_STATEMENT_END

%token < _identifier >              TOKEN_IDENTIFIER

%token < _literalConstant >         TOKEN_LITERAL_BOOL
%token < _literalConstant >         TOKEN_LITERAL_DECIMAL
%token < _literalConstant >         TOKEN_LITERAL_FLOATING

%token < _literalConstantString >   TOKEN_LITERAL_STRING
%token < _literalConstantBinary >   TOKEN_LITERAL_HEXADECIMAL

%type< _literalConstantString >  literalString
%type< _literalConstantBinary >  literalBinary
%type< _literalConstant >        literal

%type< _variable > variableDefinitionExp
%type< _variable > variableDefinitionExpStmt

%type< _category > categoryDefinition

%{
   #include <l2ParserDriverImpl.h>
   
   #define yylex aDriver.lexer().yylex
%}

%%
%start registryMembersSeqOpt;

literalString:
   TOKEN_LITERAL_STRING                               { $$ = $1 }
   |literalString TOKEN_LITERAL_STRING                { *$$ += *$2; delete $2 }
   ;
   
literalBinary:
   TOKEN_LITERAL_HEXADECIMAL                          { $$ = $1 }
   |literalBinary TOKEN_LITERAL_HEXADECIMAL           { $$->Append( *$2 ); delete $2 }
   ;
   
literal:
   TOKEN_LITERAL_BOOL                                 { $$ = $1 }
   |TOKEN_LITERAL_DECIMAL                             { $$ = $1 }
   |TOKEN_LITERAL_FLOATING                            { $$ = $1 }
   |literalString                                     { $$ = new CVariant( *$1 ); delete $1 }
   |literalBinary                                     { $$ = new CVariant( *$1 ); delete $1 }
   ;

variableDefinitionExp:
   TOKEN_IDENTIFIER TOKEN_OPERATOR_ASSIGN literal     { $$ = new Registry::Driver::TVariable( *$1, *new Registry::Driver::CVariable( *$3 ) );
                                                        delete $1;
                                                        delete $3 }
   ;

variableDefinitionExpStmt:
   variableDefinitionExp TOKEN_STATEMENT_END          { $$ = $1 }
   ;

categoryDefinition:
   TOKEN_CATEGORY_IDETIFIER_BEGIN TOKEN_IDENTIFIER TOKEN_CATEGORY_IDETIFIER_END     { $$ = new Registry::Driver::TCategory( *$2, *new Registry::Driver::CCategory() );
                                                                                      delete $2 }
   ;

registryMembers:
   categoryDefinition                                 { Registry::Driver::SCategoryMembers members;
                                                        members._categories.insert( Registry::Driver::TCategorySeq::value_type( $1->first, *$1 ) );
                                                        
                                                        delete $1;
                                                        
                                                        if( aDriver.isTerminate() )
                                                        {
                                                           YYABORT;
                                                        }
                                                        
                                                        if( aDriver.categoryStackSize() > 1 )
                                                        {
                                                           aDriver.PopCategory();
                                                        }
                                                        
                                                        if( !aDriver.PushCategory( Registry::Driver::TCategory( "::", *new Registry::Driver::CCategory( members ) ) ) )
                                                        {
                                                           aDriver.error( yylloc, "Category already defined" );
                                                        } }

   |variableDefinitionExpStmt                         { Registry::Driver::SCategoryMembers members;
                                                        members._variables.insert( Registry::Driver::TVariableSeq::value_type( $1->first, *$1 ) );
                                                        
                                                        delete $1;
                                                        
                                                        if( aDriver.isTerminate() )
                                                        {
                                                           YYABORT;
                                                        }
                                                        
                                                        if( !aDriver.PushCategory( Registry::Driver::TCategory( "::", *new Registry::Driver::CCategory( members ) ) ) )
                                                        {
                                                           aDriver.error( yylloc, "Variable already defined" );
                                                        } }
   ;
   
registryMembersSeq:
   registryMembers
   |registryMembersSeq registryMembers
   ;

registryMembersSeqOpt:
   |registryMembersSeq                                { if( aDriver.categoryStackSize() > 1 )
                                                        {
                                                           aDriver.PopCategory();
                                                        } }
   ;
   
%%
