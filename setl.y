%{
#include <stdio.h>
%}

%token SETI VARI            /* Set and variable identifiers */
%token STRING NUMBER        /* Base types */
%token BOOL                 /* boolean token*/
%token CMP                  /* Comparisson operators */
%token SBSOP                /* Binary set operators */
%token SUSOP                /* Unary set operators */
%token MEM                  /* Membership (in) */
%token LOG                  /* logical operators */
%token VUSOP                /* Set operations that produce non set value */
%token FUNC                 /* function */

%right '='
%nonassoc FUNC CMP
%left SUSOP
%left SBSOP
%left '+' '-'
%left '*' '/' '%'
%nonassoc '~'

%%

exp:
   | SETI
   | VARI
   | item
   | SETI '=' item
   | VARI '=' item
   ;

item: constmathexp
    | STRING
    | setopexp
    | VUSOP setopexp
    | varmathexp
    ;

nprod: NUMBER 
     | usop setopexp 
     | var
     ;

setopexp: setexp 
        | setopexp BSOP setopexp
        ;

setexp: flist
      | plist
      | SETI
      ;

item1: item
     | items ',' item
     ;

flist: '{' item1 '}'
     ;

plist: '{' varexp '|' valueprod '}'
     | '{' varexp ':' valueprod '}'
     ;

varexp: constmathexp
      | varmathexp
      | flist
      | arrayexp
      ;

arrayexp: varprod '[' STRING ']'
        ;

valueprod: varprod
         | varprod ',' constraintexp
         ;

varprod: membership
       | varprod ',' membership
       ;

membership: VARI '<-' setexp
          ;

constraintexp:
             | constraintexp LOG constraint
             ;

constraint: varmathexp LOG constmathexp
          | varmathexp LOG varmathexp
          | constmathexp LOG constmathexp
          | '(' constraint ')'
          | '!' constraint
          ;

varmathexp: varmathexp '+' varmathexp
          | varmathexp '*' varmathexp
          | varmathexp '/' varmathexp
          | varmathexp '-' varmathexp
          | varmathexp '%' varmathexp
          | varmathexp '^' varmathexp
          | FUNC '(' varmathexp ')'
          | VARI
          | nprod
          ;

constmathexp: constmathexp '+' constmathexp
          | constmathexp '*' constmathexp
          | constmathexp '/' constmathexp
          | constmathexp '-' constmathexp
          | constmathexp '%' constmathexp
          | constmathexp '^' constmathexp
          | FUNC '(' constmathexp ')'
          | nprod
          | '~' constmathexp
          ;

