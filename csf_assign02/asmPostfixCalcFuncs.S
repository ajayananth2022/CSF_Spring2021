/*
 * postfix calculator (assembly language version)
 * function implementation
 */

.equ TOK_INT, 0
.equ TOK_OP, 1
.equ TOK_UNKNOWN, 2

.equ MAX_STACK, 20

	.section .rodata

eNegativeValue: .string "Negative value"
eFullStack: .string "Full Stack"
eEmptyStack: .string "Empty Stack"

errorMessage: .string "Error: %s\n"


/* TODO: add other string constants and other read-only data here */

	.section .text

/*
 * addPositive function (this is just an example function)
 */
	.globl addPositive
addPositive:
	subq $8, %rsp                /* align stack pointer */

	movq $0, %r10                /* let %r10 be 0 */
	cmpq %rdi, %r10              /* compare 1st param to 0 */
	jg .LaddPositiveNegativeOperand /* if less, it's negative */
	cmpq %rsi, %r10              /* compare 2nd param to 0 */
	jg .LaddPositiveNegativeOperand /* if less, it's negative */

	/* both operands are non-negative, do the addition */
	movq %rsi, %rax              /* move second parameter to %rax */
	addq %rdi, %rax              /* add first parameter to second */
	jmp .LaddPositiveDone

.LaddPositiveNegativeOperand:
	movq $eNegativeValue, %rdi
	call fatalError
9:	jmp 9b                       /* should not get here */

.LaddPositiveDone:
	/* the sum is now in %rax */
	addq $8, %rsp                /* restore stack pointer */
	ret

/*
 * fatalError - print an error message an exit with an exit code of 1.
 * Prints a message of the form "Error: <message text>" to standard output.
 * Does not return!
 *
 * Params:
 *   msg - description of the error which occurred
 */
	.globl fatalError
fatalError:
	/* TODO: implement */
	/* note that this function should not return */
	subq $8, %rsp                /* align stack pointer */
	movq %rdi, %r10

	movq $errorMessage, %rdi
	movq %r10, %rsi 
	call printf

	movq $1, %rdi
	addq $8, %rsp                /* restore stack pointer */
	call exit

/*
 * isSpace - tests a character code to determine whether or not it
 * is a space character.
 *
 * Params:
 *   c - an ASCII character code
 *
 * Returns:
 *   1 if c is a space or tab character, 0 otherwise
 *   (the function is allowed to also return 1 if c is a newline
 *   or carriage return, but it's only required to handle
 *   spaces and tabs)
 */
	.globl isSpace
isSpace:
	/* TODO: implement */
	subq $8, %rsp                /* align stack pointer */
	cmpb $9, %dil                /* compare first argument (0th bit) with space */
	je .LspaceOrTab
	cmpb $32, %dil               /* compare first argument (0th bit) with tab */
	je .LspaceOrTab
	movq $0, %rax                /* return 0 if not space or tab */
	jmp .LdoneIsSpace

.LspaceOrTab:
	movq $1, %rax                /* return 1 if is space or tab */
	jmp .LdoneIsSpace

.LdoneIsSpace:
	addq $8, %rsp                /* restore stack pointer */
	ret

/*
 * isDigit - tests a character code to determine whether it is one
 * of the digits '0' through '9'
 *
 * Params:
 *   c - an ASCII character code
 *
 * Returns:
 *   1 if c is a digit, 0 otherwise
 */
	.globl isDigit
isDigit:
	/* TODO: implement */
	subq $8, %rsp                /* align stack pointer */
	cmpb $48, %dil               /* compare first argument (0th bit) with 0 */
	jb .LnotDigit
	cmpb $57, %dil               /* compare first argument (0th bit) with 9 */
	ja .LnotDigit
	movq $1, %rax                /* return 1 if is digit */
	jmp .LdoneIsDigit

.LnotDigit:
	movq $0, %rax                /* return 0 if is not digit */
	jmp .LdoneIsDigit

.LdoneIsDigit:
	addq $8, %rsp                /* restore stack pointer */
	ret

/*
 * skipws - skips any whitespace characters (as determined by isSpace)
 * at the beginning of a C character string.
 *
 * Params:
 *   s - pointer to a C character string
 *
 * Returns:
 *   pointer to the first non-space character in the string, or NULL
 *   if there are no non-space characters before the end of the string
 *   is reached
 */
	.globl skipws
skipws:
	/* TODO: implement */
	ret

/*
 * tokenType - return the token type (TOK_INT, TOK_OP, or TOK_UNKNOWN)
 * of the token at the beginning of given C character string.
 *
 * Params:
 *   s - nonempty C character string (which should not have any leading
 *       space characters)
 *
 * Returns:
 *   the token type
 */
	.globl tokenType
tokenType:
	/* TODO: implement */
	subq $8, %rsp                /* align stack pointer */

	movq (%rdi), %r10            /* dereference rdi and store in r10 */
	movq %r10, %rdi
	call isDigit

	cmpq $1, %rax                /* check if char is digit */
	je .LcharIsDigit

	cmpb $43, %r10b              /* check if char is "+" */
	je .LcharIsOperator
	cmpb $45, %r10b              /* check if char is "-" */
	je .LcharIsOperator
	cmpb $42, %r10b              /* check if char is "*" */
	je .LcharIsOperator
	cmpb $47, %r10b             /* check if char is "/" */
	je .LcharIsOperator

	movq $2, %rax               /* returns 2 for invalid token */
	jmp .LdonetokenType

.LcharIsDigit:	
	movq $0, %rax               /* returns 0 for digit */
	jmp .LdonetokenType

.LcharIsOperator:
	movq $1, %rax               /* returns 1 for operator */
	jmp .LdonetokenType

.LdonetokenType:
	addq $8, %rsp                /* restore stack pointer */
	ret

/*
 * consumeInt - consume one or more digit characters at the beginning
 * of given C character string, convert them to an integer value, and
 * store the integer value in the long (64 bit signed integer)
 * variable that pval points to.
 *
 * Params:
 *   s    - pointer to a nonempty C character string which should start
 *          with a digit character
 *   pval - points to a long variable where the converted integer value
 *          should be stored
 *
 * Returns:
 *   pointer to the first character in the string that is not a digit
 */
	.globl consumeInt
consumeInt:
	/* TODO: implement */
	ret

/*
 * consumeOp - consume an operator character from the beginning of the
 * given C character string.
 *
 * Params:
 *   s - pointer to a nonempty C character string
 *   op - pointer to an int (32 bit signed integer) variable where the
 *        ASCII code of the first character of s should be stored
 *
 * Returns:
 *   a pointer to the second character of s
 */
	.globl consumeOp
consumeOp:
		/* TODO: implement */
	subq $8, %rsp                /* align stack pointer */

	movq %rdi, %r10			/* copy over first argument (s pointer) to r10 */

	movq $0, %rdi 			/* clear rdi register (get rid of garbage values in higher bits)*/
	movb (%r10), %dil 		/* move first byte (first char) of s to %dil (byte 0 subreg of rdi)*/

	movl %edi, (%rsi)     	/* set rsi equal to operator (s[0]) */

	incq %r10 				/* make rdi point to s[1] */
	movq %r10, %rax			/* move %rdi to result register */
	
	addq $8, %rsp       	/* align stack pointer */

	ret

/*
 * stackPush - push a long (64 bit signed integer) value onto the stack.
 * Calls the fatalError function if the stack is full.
 * A full stack contains MAX_STACK (20) values.
 * If a value is successfully pushed on the stack, the count should
 * be incremented by 1.
 *
 * Params:
 *   stack - pointer to the beginning of the stack storage array
 *   count - pointer to a long variable storing the number of items
 *           currently on the stack
 *   val   - the value to push onto the stack
 *
 * Returns:
 *   nothing

 void stackPush(long stack[], long *count, long val) {
  if (*count == MAX_STACK) {
    fatalError("stack is full"); 
  } else {
    stack[*count] = val;
    (*count)++; //increment count of stack
  }
}
 */
	.globl stackPush
stackPush:
    /* TODO: implement */
	/* 
	rdi:1st arg, rsi = esi:2nd reg , rdx = edx: 3rd reg
	*/

	subq $8, %rsp                /* align stack pointer */


	cmpl $MAX_STACK,(%esi)       /* if count equals 20 */
	je .LFullStack

	movq (%rsi), %r10		/* dereference count pointer and store in r10 */
	
	movq %rdx,(%rdi,%r10)  		/*stack[*count] = val;*/
	incq (%rsi)				/*increment stack. shoud this be rsi or esi?*/	
	jmp .LDoneStackPush

.LFullStack:
	movq $eFullStack, %rdi
	call fatalError
9:	jmp 9b                       /* should not get here, but what does this even mean??*/

.LDoneStackPush:
	addq $8, %rsp                /* align stack pointer */
	ret

/*
 * stackPop - pops a long (64 bit signed integer) value from the stack.
 * Calls the fatalError function if the stack is empty.
 * If a value is successfully popped off the stack, the count should
 * be decremented by 1.
 *
 * Params:
 *   stack - pointer to the beginning of the stack storage array
 *   count - pointer to a long variable storing the number of items
 *           currently on the stack
 *
 * Returns:
 *   the value popped from the stack
 */
	.globl stackPop
stackPop:
	    /* TODO: implement */
	/* 
	rdi:1st arg, rsi = esi:2nd reg 
	*/

	subq $8, %rsp                /* align stack pointer */
	movq (%rsi), %r10 			/* dereference count pointer and store in r10 */

	cmpl $0, (%esi)       /* if count equals 0 */
	je .LEmptyStack

	decq (%rsi)				;//decrement count of stack

	movq (%rdi, %r10), %r11		/* store value at stack[*count] in r11*/
	
	movq %r11,%rax 			/* store r11 in result register*/
	jmp .LDoneStackPop

.LEmptyStack:
	movq $eEmptyStack, %rdi
	call fatalError
9:	jmp 9b                       /* should not get here, but what does this even mean??*/

.LDoneStackPop:
	addq $8, %rsp                /* align stack pointer */
	ret

/*
 * evalOp - evaluate a computation by applying an operator to two operands.
 *
 * Params:
 *   op - the operator ('+', '-', '*', or '/')
 *   left - the left operand
 *   right - the right operand
 *
 * Returns:
 *   the result of applying the operator to the operands
 */
	.globl evalOp
evalOp:
	subq $8, %rsp               /* align stack pointer */

	cmpb $43, %dil              /* check if char is "+" */
	je .LcharIsPlus
	cmpb $45, %dil              /* check if char is "-" */
	je .LcharIsMinus
	cmpb $42, %dil              /* check if char is "*" */
	je .LcharIsMutiplication
	cmpb $47, %dil              /* check if char is "/" */
	je .LcharIsDivision

	movq $0, %rax               /* returns 0 for invalid operator */
	jmp .LdoneEvalOp

.LcharIsPlus:
	addq %rsi, %rdx             /* add first and second argument */
	movq %rdx, %rax             /* stores first + second argument in rax */
	jmp .LdoneEvalOp

.LcharIsMinus:
	subq %rdx, %rsi             /* subtract second arg from first argument */  
	movq %rsi, %rax             /* stores first - second argument in rax */
	jmp .LdoneEvalOp

.LcharIsMutiplication:
	movq %rdx, %rax             /* multiplies first and second argument */
	mulq %rsi                   /* stores first * second argument in rax */
	jmp .LdoneEvalOp

.LcharIsDivision:
	movq %rdx, %r10              /* temporarily store %rdx in %r10 */
	movq %rsi, %rax              /* put dividend in %rax */
	cqto                         /* sign-extend dividend into %rdx */
	idivq %r10                   /* quotient is now in %rax */
	jmp .LdoneEvalOp

.LdoneEvalOp:
	addq $8, %rsp                /* restore stack pointer */
	ret

/* vim:ft=gas:
 */
