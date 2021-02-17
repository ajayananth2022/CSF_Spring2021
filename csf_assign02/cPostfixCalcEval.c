#include <stdio.h>
#include <stdlib.h>
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
  /* TODO: implement */

  /* Note: this function should be implemented by calling functions
   * declared in cPostfixCalc.h and defined in cPostfixCalcFuncs.c
   */
  int i = 0; 

  long stack[MAX_STACK];
  long stackCount = 0; 

  long extractNum; 
  int32_t operator; 

  long operand1; 
  long operand2; 

  while (strlen(s) != 0) {

    s = skipws(s); 

    if (tokenType(s) == 0) {
      s = consumeInt(s, extractNum); 
      stackPush(stack, stackCount, extractNum); 
    }

    else if (tokenType(s) == 1) {
      s = consumeOp(s, operator); 

      operand1 = stackPop(stack, stackCount); 
      operand2 = stackPop(stack, stackCount); 

      stackPush(stack, stackCount, evalOp(operator, operand1, operand2)); 
    }
  }

  return stackPop(stack, stackCount);
}