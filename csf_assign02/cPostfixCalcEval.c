#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cPostfixCalc.h"

/*
 * eval - evaluate a postfix expression
 * Should directly or indirectly call fatalError if the expression is invalid.
 *
 * Params:
 *   s - C string containing a postfix expression
 *
 * Returns:
 *   the result of evaluating the expression
 */
long eval(const char *s) {
  long stack[MAX_STACK];
  long stackCount = 0; 
  long extractNum = 0; //keeps the extracted number, default to 0
  int32_t operator = '+'; //keeps the extracted operator, default to +

  long operand1, operand2;
  s = skipws(s); 
  while (s != NULL && strlen(s) != 0) {
    if (tokenType(s) == 0) { //if token is a number
      s = consumeInt(s, &extractNum); 
      stackPush(stack, &stackCount, extractNum); 
      s = skipws(s);
    } else if (tokenType(s) == 1) { //if token is an operator
      s = consumeOp(s, &operator); 
      operand1 = stackPop(stack, &stackCount); 
      operand2 = stackPop(stack, &stackCount); 
      stackPush(stack, &stackCount, evalOp(operator, operand2, operand1)); 
      s = skipws(s); 
    } else { //if token is unknown
      fatalError("unknown token");
    }
  }
  if (stackCount != 1) fatalError("Invalid expression");
  return stackPop(stack, &stackCount);
}