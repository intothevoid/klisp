
#include "mpc.h"
#include <stdio.h>

#define BUFF_SIZE 2048
/* Declare a buffer for user input of size 2048 */
static char input[BUFF_SIZE];


// Error types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

// lval types
enum { LVAL_NUM, LVAL_ERR };

// lval struct
typedef struct 
{
  int type;
  long num;
  int err;
} lval;

// Create a new number type lval
lval lval_num(long x)
{
  lval v;
  v.type = LVAL_NUM;
  v.num = x;

  return v;
}

// Create a new error type lval
lval lval_err(int x)
{
  lval v;
  v.type = LVAL_ERR;
  v.err = x;

  return v;
}

// Print an lval
void lval_print(lval v)
{
  switch(v.type)
  {
    case LVAL_NUM:
      printf("%li", v.num);
      break;

    case LVAL_ERR:
      if(v.err == LERR_DIV_ZERO)
      {
        printf("Error: Division by zero!");
      }
      if(v.err == LERR_BAD_OP)
      {
        printf("Error: Invalid operator!");
      }
      if(v.err == LERR_BAD_NUM)
      {
        printf("Error: Invalid number!");
      }
      break;

      default: break;
  }
}

void lval_println(lval v)
{
  lval_print(v);
  printf("\n");
}

lval eval_op(lval x, char* op, lval y)
{
  // If either value is an error, return it
  if(x.type == LVAL_ERR) return x;
  if(y.type == LVAL_ERR) return y;

  if(!strcmp(op, "+")) { return lval_num(x.num + y.num); }
  if(!strcmp(op, "-")) { return lval_num(x.num - y.num); }
  if(!strcmp(op, "*")) { return lval_num(x.num * y.num); }
  if(!strcmp(op, "/")) 
  { 
    return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
  }

  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t)
{
  if(strstr(t->tag, "number"))
  {
    // Check if there is some error in conversion
    errno = 0;
    long x = strtol(t->contents, NULL, 10); 
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }

  // Operator is always the second child
  char* op = t->children[1]->contents;

  // Store the 3rd child
  lval x = eval(t->children[2]);

  int i = 3;
  while(strstr(t->children[i]->tag, "expr"))
  {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}

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

      mpc_ast_t* tree = r.output;

      // Evaluate the AST
      lval result = eval(tree);

      // Print result
      lval_println(result);

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