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
	*    %r13 - store Postfix
 	*/

	movq $0, 160(%rbp)           /* initialize stack count to 0 */
	leaq 168(%rbp), %r12
	call skipws
	
.LcheckPostfix:
	cmpb $0, %al                 /* compare postfix (skipped white space) with null */
	je .LPostfixIsEmpty

	movq %rax, %rdi
	call tokenType

	cmpq $TOK_INT, %rax
	je .LtokenIsNum

	cmpq $TOK_OP, %rax
	je .LtokenIsOp

	movq $eUnknownToken, %rdi
	call fatalError
9:	jmp 9b                       /* should not get here */

.LtokenIsNum:
	movq %r12, %rsi
	call consumeInt
	movq %rax, %r13

	movq 0(%rbp), %rdi
	leaq 160(%rbp), %rsi
	movq (%r12), %rdx
	call stackPush

	movq %r13, %rdi
	call skipws
	jmp .LcheckPostfix

.LtokenIsOp:
	movq %r12, %rsi
	call consumeOp
	movq %rax, %r13

	movq 0(%rbp), %rdi
	leaq 160(%rbp), %rsi
	call stackPop
	movq %rax, %rdx
	
	call stackPop

	movq %rax, %rsi
	movq (%r12), %rdi
	call evalOp

	movq %rax, %rdx
	leaq 160(%rbp), %rsi
	movq 0(%rbp), %rdi
	call stackPush

	movq %r13, %rdi
	call skipws
	jmp .LcheckPostfix

.LPostfixIsEmpty:
	cmpq $1, 160(%rbp)
	je .LdoneEval

	movq $eInvalidExpression, %rdi /* call fatalError with invalid expression error */
	call fatalError
9:	jmp 9b                       /* should not get here */

.LdoneEval:
	movq 0(%rbp), %rdi
	leaq 160(%rbp), %rsi
	call stackPop

	addq $176, %rsp              /* deallocate space for local variable(s) */
	popq %r12
	popq %r13
	popq %rbp                    /* restore previous frame pointer */
	ret

/* vim:ft=gas:
 */
