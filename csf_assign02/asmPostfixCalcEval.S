/*
 * postfix calculator (assembly language version)
 * eval function implementation
 */

.equ TOK_INT, 0
.equ TOK_OP, 1
.equ TOK_UNKNOWN, 2

	.section .rodata

eUnknownToken: .string "Unknown token"
eInvalidExpression: .string "Invalid Expression"

/* TODO: add string constants and other read-only data here */

	.section .text

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
	.globl eval
eval:
	/* TODO: implement */

	pushq %rbp                   /* save previous frame pointer */
	pushq %r12                   /* save previous frame pointer */
	pushq %r13                   /* save previous frame pointer */
	subq $176, %rsp              /* reserve space of 20 elements and local variables */
	movq %rsp, %rbp              /* make %rbp point to local variables area */	

	/*
 	* Local variables:
 	*    0(%rbp) - base of stack array
 	*    160(%rbp) - stack count 
	*    %r12 - address extracted number or operator
	*    %r13 - store Postfix char array
 	*/

	movq $0, 160(%rbp)           /* initialize stack count to 0 */
	leaq 168(%rbp), %r12         /* store address on the stack to r12 */
	call skipws                  /* skip white space in postfix char array */
	
.LcheckPostfix:
	cmpb $0, %al                 /* compare postfix (skipped white space) with null */
	je .LPostfixIsEmpty          /* postfix char array is empty, time to return */

	movq %rax, %rdi              /* use skipped white space postfix to call tokenType */
	call tokenType               /* check the first char in postfix */

	cmpq $TOK_INT, %rax          /* if the next char is int */
	je .LtokenIsNum

	cmpq $TOK_OP, %rax           /* if the next char is operator */
	je .LtokenIsOp

	movq $eUnknownToken, %rdi    /* if the next char is not int or operator */
	call fatalError              /* call fatalError with unknown token error */
9:	jmp 9b                       /* should not get here */

.LtokenIsNum:
	movq %r12, %rsi              /* use r12 (pointer) to store the the extracted int */
	call consumeInt
	movq %rax, %r13              /* store the postfix with int consumed */

	movq 0(%rbp), %rdi           /* move the stack base pointer to first argument */
	leaq 160(%rbp), %rsi         /* move the address of stack count to second argument */
	movq (%r12), %rdx            /* dereference the stored int and move to third argument */
	call stackPush               /* push the extracted number into stack */

	movq %r13, %rdi              /* call skipws with the postfix array */
	call skipws
	jmp .LcheckPostfix

.LtokenIsOp:
	movq %r12, %rsi              /* use r12 (pointer) to store the the extracted int */
	call consumeOp
	movq %rax, %r13              /* store the postfix with operator consumed */

	movq 0(%rbp), %rdi           /* move the stack base pointer to first argument */
	leaq 160(%rbp), %rsi         /* move the address of stack count to second argument */
	call stackPop                /* pop the first operand from the stack */
	movq %rax, %rdx              /* move the first operand to third argument for evaluation */
	
	movq 0(%rbp), %rdi           /* move the stack base pointer to first argument */
	leaq 160(%rbp), %rsi         /* move the address of stack count to second argument */
	call stackPop                /* pop the second operand from the stack */

	movq %rax, %rsi              /* move the second operand to second argument for evaluation */   
	movq (%r12), %rdi            /* dereference the extracted operator and move to first arg */
	call evalOp                  /* evaluate with the operator, 1st & 2nd operands */

	movq %rax, %rdx              /* move the result of evaluation to third arg */
	leaq 160(%rbp), %rsi         /* move the stack base pointer to first argument */
	movq 0(%rbp), %rdi           /* move the address of stack count to second argument */
	call stackPush               /* push the evaluated result to the stack */

	movq %r13, %rdi              /* call skipws with the postfix array */
	call skipws
	jmp .LcheckPostfix

.LPostfixIsEmpty:
	cmpq $1, 160(%rbp)           /* if the stack count is 1 */
	je .LdoneEval                /* done evaluating postfix */

	movq $eInvalidExpression, %rdi /* call fatalError with invalid expression error */
	call fatalError
9:	jmp 9b                       /* should not get here */

.LdoneEval:
	movq 0(%rbp), %rdi           /* move the stack base pointer to first argument */
	leaq 160(%rbp), %rsi         /* move the address of stack count to second argument */
	call stackPop                /* pop the postfix result from stack */

	addq $176, %rsp              /* deallocate space for local variable(s) */
	popq %r13
	popq %r12
	popq %rbp                    /* restore previous frame pointer */
	ret

/* vim:ft=gas:
 */
