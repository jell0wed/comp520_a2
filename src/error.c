#include "error.h"

extern char *yytext;

void yyerror(char *s)
{ fprintf(stderr,"%s\n",s);
  fprintf(stderr,"*** syntax error before %s at line %i\n",
                 yytext,lineno);
  fprintf(stderr,"*** compilation terminated\n");
  exit(1);
}

void reportError(char *s, int lineno)
{ printf("*** %s at line %i\n",s,lineno);
  errors++;
}

void reportStrError(char *s, char *name, int lineno)
{ printf("*** ");
  printf(s,name);
  printf(" at line %i\n",lineno);
  errors++;
}

void reportGlobalError(char *s)
{ printf("*** %s\n",s);
  errors++;
}

void reportStrGlobalError(char *s, char *name)
{ printf("*** ");
  printf(s,name);
  printf("\n");
  errors++;
}

void noErrors()
{ if (errors!=0) {
     fprintf(stderr,"*** compilation terminated\n");
     exit(1);
  }
}