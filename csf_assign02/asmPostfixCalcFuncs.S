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
eDivisionByZero: .string "Division by zero"

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
	movq %rdi, %r10              /* temporarily stores the error message in r10 */

	movq $errorMessage, %rdi     /* move the error message format into first arg */
	movq %r10, %rsi              /* move error message into second arg */
	call printf

	movq $1, %rdi                /* move 1 to first arg to exit with code 1 */
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
	je .LspaceOrTab              /* jumps to this label when the char is a space */
	cmpb $32, %dil               /* compare first argument (0th bit) with tab */
	je .LspaceOrTab              /* jumps to this label when the char is a tab */
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
	jb .LnotDigit                /* jump to the not digit label when char is below '0' */
	cmpb $57, %dil               /* compare first argument (0th bit) with 9 */
	ja .LnotDigit                /* jump to the not digit label when char is above '9' */
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
	subq $8, %rsp                /* align stack pointer */               
	movq %rdi, %r11              /* temporarily store the pointer to current char */
	movq (%rdi), %rdi            /* dereference the char pointer and call isSpace */
	call isSpace

.Ltop:
	cmpb $0, (%r11)              /* if current char is null */
	je .LemptyString

	cmpq $0, %rax                /* if current char is not space */
	je .LdoneSkipping

	incq %r11                    /* update pointer to the current char */
	movq (%r11), %rdi            /* dereference the char pointer and call isSpace */
    call isSpace
	jmp .Ltop

.LemptyString:
	movq $0, %rax                /* return NULL */
	jmp .LdoneSkipWs

.LdoneSkipping:
	movq %r11, %rax              /* return a pointer to the current char */
	jmp .LdoneSkipWs

.LdoneSkipWs:
	addq $8, %rsp                /* restore stack pointer */
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

	movq %rdi, %r11              /* temporarily store the postfix in r11 */
	movq (%rdi), %r10            /* dereference rdi and store in r10 */
	movq %r10, %rdi              /* move dereferenced rdi to rdi */
	call isDigit                 /* check if dereferenced rdi is a digit */

	cmpq $1, %rax                /* check if char is digit */
	je .LcharIsDigit

	cmpb $43, %r10b              /* check if char is "+" */
	je .LcharIsOperator          /* jump to isOperator label when char is "+" */
	cmpb $45, %r10b              /* check if char is "-" */
	je .LcharIsOperator          /* jump to isOperator label when char is "-" */
	cmpb $42, %r10b              /* check if char is "*" */
	je .LcharIsOperator          /* jump to isOperator label when char is "*" */
	cmpb $47, %r10b              /* check if char is "/" */
	je .LcharIsOperator          /* jump to isOperator label when char is "/" */

	movq $TOK_UNKNOWN, %rax      /* returns 2 for invalid token */
	jmp .LdonetokenType

.LcharIsDigit:	
	movq $TOK_INT, %rax          /* returns 0 for digit */
	jmp .LdonetokenType

.LcharIsOperator:
	movq $TOK_OP, %rax           /* returns 1 for operator */
	jmp .LdonetokenType

.LdonetokenType:
	movq %r11, %rdi              /* restore the value of rdi (postfix) */
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

 /* 
  C Code: 
  *pval = 0; 
  int i = 0;
  while (isDigit(s[i])) { //keep reading in digits
    *pval = *pval*10 + (s[i] - 48); 
    i++; 
  }
  return &s[i]; 
  */	

	.globl consumeInt
consumeInt:
	/*
 	* Local variables:
 	*    %r10 - base address of char array
 	*    %r11 - dereferenced value of pval 
	*    %rbp - address extracted number or operator
 	*/
	pushq %rbp                  /* push rbp onto the stack to save its value */
	subq $16, %rsp              /* allocate space for local variables */
	movq %rsp, %rbp             /* make %rbp point to local variables area */	

	movq %rdi, %r10				/* copy over first argument (s pointer) to r10 */
	movq (%rsi), %r11    		/* dereference pval pointer and store value */
	movq $0, %r11 				/* necessary? */
	movq $0, %rbp  				/* loop variable, i */

	movq (%r10), %rdi  			/* move s[0] char to 1st arg. register */
	jmp .LcheckDigit

.LcheckDigit: 
	call isDigit
	cmpq $1, %rax 				/* check if char is digit */
	je .LupdatePVal         	/* jump to .LupdatePval if digit */

	jmp .LEnd

.LupdatePVal: 
	imulq $10, %r11         	/* multiply pval (r11) by 10 */ 
    movb (%r10, %rbp), %r15b
	addq %r15, %r11 	/* add pval to s[i] */
	/*  *pval = *pval*10 + (s[i] - 48); */
	subq $48, %r11  			/* sub 48 to account for ASCII offset */ 
	incq %rbp          			/* increment rbp (loop var) */ 

	movq (%r10, %rbp), %rdi 	/* store s[i] in 1st arg register */
	jmp .LcheckDigit

.LEnd: 
	movq $0, (%rsi)
	movq %r11, (%rsi) 			/* make pval pointer point to r11 */
	leaq (%r10, %rbp), %rax  	/* store pointer to first s[i] that is not a digit in result arg */

	addq $16, %rsp              /* deallocate space for local variable(s) */
	popq %rbp                   /* restore value of rbp */
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

	movq $0, %rdi 			/* clear rdi register (get rid of garbage values in higher bits) */
	movb (%r10), %dil 		/* move first byte (first char) of s to %dil (byte 0 subreg of rdi) */

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

	cmpl $MAX_STACK, (%rsi)      /* if count equals 20 */
	je .LFullStack

	movq (%rsi), %r10		     /* dereference count pointer and store in r10 */
	
	movq %rdx, (%rdi, %r10, 8)   /* store the long into the stack at right position */
	incq (%rsi)				     /* increment stack */	
	jmp .LDoneStackPush

.LFullStack:
	movq $eFullStack, %rdi       /* call fatalError with Full Stack error message */
	call fatalError
9:	jmp 9b                       /* should not get here */

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

	cmpl $0, (%rsi)              /* if count equals 0 */
	je .LEmptyStack

	decq (%rsi)				     /* decrement count of stack */
	movq (%rsi), %r10 			 /* dereference count pointer and store in r10 */

	movq (%rdi, %r10, 8), %rax	 /* store value at stack[*count] in rax to return it*/
	jmp .LDoneStackPop

.LEmptyStack:
	movq $eEmptyStack, %rdi      /* call fatalError with empty stack error */
	call fatalError
9:	jmp 9b                       /* should not get here */

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
	je .LcharIsPlus             /* perform addition when char is "+" */
	cmpb $45, %dil              /* check if char is "-" */
	je .LcharIsMinus            /* perform subtraction when char is "-" */
	cmpb $42, %dil              /* check if char is "*" */
	je .LcharIsMutiplication    /* perform multiplication when char is "*" */
	cmpb $47, %dil              /* check if char is "/" */
	je .LcharIsDivision         /* perform multiplication when char is "/" */

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
	cmpq $0, %rdx
	je .LdivisionByZero
	movq %rdx, %r10              /* temporarily store %rdx in %r10 */
	movq %rsi, %rax              /* put dividend in %rax */
	cqto                         /* sign-extend dividend into %rdx */
	idivq %r10                   /* quotient is now in %rax */
	jmp .LdoneEvalOp

.LdivisionByZero:
	movq $eDivisionByZero, %rdi
	call fatalError
9:	jmp 9b                       /* should not get here */

.LdoneEvalOp:
	addq $8, %rsp                /* restore stack pointer */
	ret

/* vim:ft=gas:
 */
