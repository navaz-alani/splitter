target ^[ \n\t]*[A-Za-z0-9]+[ ]*[A-Za-z0-9][ \n\t]*$
item ^[ \n\t]*([0-9]*[.])?[0-9]+[ \t\n]*[A-Za-z0-9-]+([ ]*[ie])?[ \n\t]*$
comment ^[ \n\t]*#(.)+

%{
#include "src/lexer.c"
%}

%%

:items
{comment}
\n

:targets      s = mk_splitter();

{target}      add_target(yytext, yyleng);

:end-targets  splitter_chmod(s);

{item}        add_item(yytext, yyleng);

:end-items    {
                  splitter_print_all(s);
                  free_splitter(s);
                  exit(0);
              }

%%

int main() {
    yylex();
}
