%{
   #include <l2TextIniParser.tab.hh>
   #include <l2LexScanerImpl.h>
%}

%option c++ noyywrap nounput batch

digit    [0-9]
alpha    [A-Za-z_]
any_char .

keyword        "true"|"false"

identifier     {alpha}({alpha}|{digit})*

decimal        [-]?{digit}{1,9}
hexadecimal    "0x"(({digit}|[A-Fa-f]){1,8})
floating       [-]?(({digit}*\.{digit}+)|({digit}+\.{digit}*))
string         ("\"")({any_char}*)("\"")
comment        ("//"{any_char}*"\n")|("/*"({any_char}|"\n")*"*/")

%{
   #define YY_USER_ACTION yylloc->columns( yyleng );
%}

%%

%{
   yylloc->step();
%}

{keyword}      {
                  parser::token::yytokentype tokenType;
                  
                  if( yytext == std::string( "true" ) )
                  {
                     yylval->_literalConstant = new CVariant( true );
                     
                     tokenType = parser::token::TOKEN_LITERAL_BOOL;
                  }
                  else if( yytext == std::string( "false" ) )
                  {
                     yylval->_literalConstant = new CVariant( false );
                     
                     tokenType = parser::token::TOKEN_LITERAL_BOOL;
                  }
                  
                  return tokenType;
               }

{identifier}   {
                  yylval->_identifier = new std::string( yytext );
                  
                  return parser::token::TOKEN_IDENTIFIER;
               }

{decimal}      {
                  yylval->_literalConstant = new CVariant( CVariant( yytext ).toInt() );
                  
                  return parser::token::TOKEN_LITERAL_DECIMAL;
               }

{hexadecimal}  {
                  yylval->_literalConstantBinary = new CByteArray( CVariant( yytext + 2 ).toByteArray() );
                  
                  return parser::token::TOKEN_LITERAL_HEXADECIMAL;
               }

{floating}     {
                  yylval->_literalConstant = new CVariant( CVariant( yytext ).toDouble() );
                  
                  return parser::token::TOKEN_LITERAL_FLOATING;
               }

{string}       {
                  yylval->_literalConstantString = new std::string( yytext + 1 );
                  yylval->_literalConstantString->resize( yylval->_literalConstantString->size() - 1 );
                  
                  const char* escSeq[][ 2 ] =
                     {
                      { "\n", "\\n" },
                      { "\t", "\\t" },
                      { "\r", "\\r" }
                     };
               
                  for( int i( 0 ); i < sizeof( escSeq ) / sizeof( *escSeq ); ++i )
                  {
                     int pos( 0 );
               
                     while( pos != std::string::npos )
                     {
                        pos = yylval->_literalConstantString->find( escSeq[ i ][ 1 ], pos );
               
                        if( pos != std::string::npos )
                        {
                           yylval->_literalConstantString->replace( pos, 2, escSeq[ i ][ 0 ] );
                        }
                     }
                  }
                  
                  return parser::token::TOKEN_LITERAL_STRING;
               }

"="            return parser::token::TOKEN_OPERATOR_ASSIGN;
"["            return parser::token::TOKEN_CATEGORY_IDETIFIER_BEGIN;
"]"            return parser::token::TOKEN_CATEGORY_IDETIFIER_END;
";"            return parser::token::TOKEN_STATEMENT_END;

[\n]+          yylloc->lines( yyleng ); yylloc->step();

[ \t\r]+       yylloc->step();

{comment}      {
                  std::string comm( yytext );
                  
                  for( std::string::iterator i( comm.begin() ); i != comm.end(); ++i )
                  {
                     if( *i == '\n' )
                     {
                        yylloc->lines( 1 );
                     }
                     
                     yylloc->step();
                  }
               }

.              aDriver.error( *yylloc, "Unrecognized symbol" );
%%
