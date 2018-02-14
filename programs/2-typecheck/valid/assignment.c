#include <stdio.h>
#define TRUE 1
#define FALSE 0
char __READ_BOOL[5];
char* strconcat(char* str, int num) { char buf[1024] = ""; for(int i = 0; i < num; i++) { strcat(buf, str); } return strdup(buf); }
char* strplus(char* str1, char* str2) { char buf[1024] = ""; strcat(buf, str1); strcat(buf, str2); return strdup(buf); }
void readBool(int* boolVal) { if (strcmp(__READ_BOOL, "TRUE") == 0) { *boolVal = TRUE; } else if (strcmp(__READ_BOOL, "FALSE") == 0) { *boolVal = FALSE; } else { fprintf(stderr, "Error! Bool value readed is invalid"); exit(1); } } 
char* printBool(int boolVal) { return boolVal == TRUE ? "TRUE" : "FALSE"; }

int main(int argc, char* argv[]) {
int testInt = 0;
float testFloat = 0.00000;

char* testString = "";

int testBool = FALSE;


testInt = testInt;
printf("%d", testInt);

testFloat = testFloat;

printf("%.5f", testFloat);

testFloat = testInt;

printf("%.5f", testFloat);

testString = testString;

printf("%s", testString);

testBool = testBool;

printf("%s", printBool(testBool));

return 0;
}