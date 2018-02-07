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
{ fprintf(stderr, "*** %s at line %i\n",s,lineno);
  errors++;
}

void reportStrError(char *s, char *name, int lineno)
{ fprintf(stderr, "*** ");
  fprintf(stderr, s,name);
  fprintf(stderr, " at line %i\n",lineno);
  errors++;
}

void reportGlobalError(char *s)
{ fprintf(stderr, "*** %s\n",s);
  errors++;
}

void reportStrGlobalError(char *s, char *name)
{ fprintf(stderr, "*** ");
  fprintf(stderr, s,name);
  fprintf(stderr, "\n");
  errors++;
}

void noErrors()
{ if (errors!=0) {
     fprintf(stderr,"*** compilation terminated\n");
     exit(1);
  }
}