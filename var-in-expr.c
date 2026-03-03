#include <stdio.h>

#define VARCOUNT 3
char vars[VARCOUNT] = {'a', 'b', 'c'};
double values[VARCOUNT] = {0.25, -3.0, 0.5};

void set_3vars(double a, double b, double c){
  values[0] = a;
  values[1] = b;
  values[2] = c;
}

int found_var(char c)
{
  for(int i =  0; i < VARCOUNT; i++)
    if(c == vars[i])
      return 1;
  return 0;
}

double value(char c)
{
  for(int i =  0; i < VARCOUNT; i++)
    if(c == vars[i])
      return values[i];
  fprintf(stderr, "Error: Variable '%c' not found.\n", c);
  return 0.0; // default value if variable not found
}