#include "io.h"

static FILE *input_file;

char set_inputstream(char* from_file)
{
  if(from_file == NULL){
    input_file = stdin;
    return 1; // Success: using stdin
  }

  input_file = fopen(from_file, "r");
  return input_file != NULL;
}

char get_char(void)
{
  if (input_file == NULL)
    return (char)EOF;
  
  // casting explicitly (int variable)tmp to (char) produces unexpected behavior (i.e. space, tab)
  // TODO: investigate this issue further?!
  char tmp = fgetc(input_file);
  
  if (tmp == EOF) {
    if (input_file != stdin) {
      fclose(input_file);
      input_file = NULL;
    }
  }

  return tmp;
}