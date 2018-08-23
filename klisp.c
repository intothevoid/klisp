
#include "mpc.h"
#include <stdio.h>

#define BUFF_SIZE 2048
/* Declare a buffer for user input of size 2048 */
static char input[BUFF_SIZE];

int main(int argc, char** argv) 
{

  // Declare our grammar for MPC
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr = mpc_new("expr");
  mpc_parser_t* Klisp = mpc_new("klisp");

  mpca_lang(
    MPCA_LANG_DEFAULT,
    "                                                       \
    number      : /-?[0-9]+/ ;                              \
    operator    : '+' | '-' | '*' | '/' ;                   \
    expr        : <number> | '(' <operator> <expr>+ ')' ;   \
    klisp       : /^/ <operator> <expr>+ /$/ ;              \
    ",
    Number, Operator, Expr, Klisp);

  /* Print Version and Exit Information */
  puts("klisp v0.3");
  puts("Press ctrl+c to Exit\n");

  // Parse input from user
  while (1) 
  {

    /* Output our prompt */
    fputs("klisp> ", stdout);

    /* Read a line of user input of maximum size 2048 */
    fgets(input, BUFF_SIZE, stdin);

    // Let's parse that glorious input shall we
    mpc_result_t r;

    if (mpc_parse("<stdin>", input, Klisp, &r))
    {
      // Print the AST
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    }
    else
    {
      // Print error
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
  }

  // Clean up
  mpc_cleanup(4, Number, Operator, Expr, Klisp);

  return 0;
}