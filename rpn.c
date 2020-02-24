#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MAXVAL  100

size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values

char buf[BUFSIZ];
size_t bufp = 0;

int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}

int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';

  if (!isdigit(c) && c != '.'&& c!='-') { return c; }  // if not a digit, return

  i = 0;
  if (isdigit(c)) {  // get digits before '.'
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '.') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '-') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }

  s[i] = '\0';
  if (c != EOF) { ungetch_(c); }
  return NUMBER;      // return type is NUMBER, number stored in s
}

double pop(void) {
  if (sp == 0) { fprintf(stderr, "stack underflow\n");  return 0.0; }
  return val[--sp];
}

void push(double f) {
  if (sp == MAXVAL) { fprintf(stderr, "stack overflow -- can't push %g\n", f);  return; }
  val[sp++] = f;
}

void rpn(void) {
  int type;
  double temp,op2,temp1,r, var;
  char s[BUFSIZ];
  int char1[26];

  while ((type = getop(s)) != EOF) {
    switch(type) {

      case NUMBER:  push(atof(s));              break;
      case '+':     push(pop() + pop());        break;
      case '*':     push(pop() * pop());        break;
      case '-':     push(-(pop() - pop()));     break;
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
      case '%':
        if ((op2 = pop()) == 0.0  ) { fprintf(stderr, "modelus of zero   \n");  break; }
        push((int)pop()%(int)op2);
        break;
      case 't':
           op2 = pop();
           printf(" %f",op2);
            push(op2);// print out the top of the stack
      case 'd':
          op2 = pop();
          temp = op2;
          push(op2);
          push(temp);// duplicate make a temp variable
              break;
      case 'f':
      temp =pop();
      op2 = pop();
      push(temp);
      push(op2); // flips

         break;
      case 'c':     sp=0;     break; // clear the stack
      case 's':     push(sin(pop()));        break;// sin
      case 'e':     push(exp(pop()));        break;// e to the power
      case 'p':     push(pow(pop(), pop()));     break; // n ^ TO THE power

      case '=':
      pop();
      if (isalpha(var)&& var!= 't'&&var!= 'd'&&var!= 'f'&&var!= 'c'&&var!= 's'&&var!= 'p'&&var!= 'e'&& var!= 'r'){
char1[(int)var-'A']=pop();
      }
      break;
      case '\n':
        r = pop();
        printf("\t%.8g\n",r);
        break;
      default:
      if (isalpha(type)&& type!= 't'&&type!= 'd'&&type!= 'f'&&type!= 'c'&&type!= 's'&&type!= 'p'&&type!= 'e' &&type!= 'r'){
        push(char1[type-'A']);
      }
      else if(type =='r') // if this is the command it prints out the last thing odn the stack 
      push(r);
      else fprintf(stderr, "unknown command or that command is already being used %s\n", s);
          break;
    }
    var=type;
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}
