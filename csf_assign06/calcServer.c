//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu

#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"

/* buffer size for reading lines of input from user */
#define LINEBUF_SIZE 1024


//helper function that takes in a Calc struct object, in&out file descriptor,
// and evaluate the input expression
//Returns 1 if client sends "quit" message or if end of input/error encounted
//Returns 2 if client sends "shutdown" message
int chat_with_client(struct Calc *calc, int infd, int outfd) {
	rio_t in;
	char linebuf[LINEBUF_SIZE];

	/* wrap standard input (which is file descriptor 0) */
	rio_readinitb(&in, infd);

	/*
	 * Read lines of input, evaluate them as calculator expressions,
	 * and (if evaluation was successful) print the result of each
	 * expression.  Quit when "quit" command is received.
	 */
	int status = 0;
	while (!status) {
		ssize_t n = rio_readlineb(&in, linebuf, LINEBUF_SIZE);
		if (n <= 0) {
			/* error or end of input */
			status = 1;
		} else if (strcmp(linebuf, "quit\n") == 0 || strcmp(linebuf, "quit\r\n") == 0) {
			/* quit command */
			status = 1;
		} else if (strcmp(linebuf, "shutdown\n") == 0 || strcmp(linebuf, "shutdown\r\n") == 0) {
			/* shutdown command */
			status = 2;
		} else {
			/* process input line */
			int result;
			if (calc_eval(calc, linebuf, &result) == 0) {
				/* expression couldn't be evaluated */
				rio_writen(outfd, "Error\n", 6);
			} else {
				/* output result */
				int len = snprintf(linebuf, LINEBUF_SIZE, "%d\n", result);
				if (len < LINEBUF_SIZE) {
					rio_writen(outfd, linebuf, len);
				}
			}
		}
	}
	return status; 
}

//helper function that prints out error message and
//exit the program
void fatal(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main(int argc, char **argv) {
	if (argc != 2) {
    	fatal("Usage: ./calcServer <port>");
  	}

	struct Calc *calc = calc_create();

	//open a server socket given port name as string
	int server_fd = open_listenfd(argv[1]);
  	if (server_fd < 0) {
    	fatal("Couldn't open server socket\n");
  	}

	int keep_going = 1;

  	while (keep_going) {
    	int client_fd = Accept(server_fd, NULL, NULL);
    	if (client_fd > 0) {
      		keep_going = chat_with_client(calc, client_fd, client_fd);
      		// close the connection
      		close(client_fd);
    	}

		//shutdown server
		if (keep_going == 2) {
			// close server socket
  			close(server_fd);
			break; 
		}
  	}

	calc_destroy(calc);

	return 0;
}
