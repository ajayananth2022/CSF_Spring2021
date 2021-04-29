//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu

#include <stdio.h>      /* for snprintf */
#include "csapp.h"
#include "calc.h"
#include <sys/select.h>

/* buffer size for reading lines of input from user */
#define LINEBUF_SIZE 1024

//global var that keeps track of shutdown requests
int volatile shutdown_request = 0; 
int MAX_NUM_THREADS = 5;
int TIMEOUT = 1;
sem_t threads;

//struct data type encapsulating the data needed for a client connection
struct ConnInfo {
	//client socket file descriptor
	int client_fd;
	//server socket file descriptor
	int server_fd;
	//pointer to the shared struct Calc object
	struct Calc *calc;
};


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

//thread start function
void *worker(void *arg) {

	int keep_going = 1;

	struct ConnInfo *info = arg;

	//thread detaches itself
	pthread_detach(pthread_self());

	//now chat with client
	keep_going = chat_with_client(info->calc,info->client_fd, info->client_fd);
	
	// close the connection
	close(info->client_fd);
	free(info);

	//shutdown server
	if (keep_going == 2) {
		// close server socket
		shutdown_request = 1; //update global variable
	}
	sem_post(&threads);
	return NULL;
}

int main(int argc, char **argv) {
	if (argc != 2) fatal("Usage: ./calcServer <port>");

	struct Calc *calc = calc_create();

	//open a server socket given port name as string
	int server_fd = open_listenfd(argv[1]);
  	if (server_fd < 0) fatal("Couldn't open server socket\n");

	sem_init(&threads, 0, MAX_NUM_THREADS);

	fd_set readfds;

  	while (shutdown_request == 0) {
		FD_ZERO(&readfds);
		FD_SET(server_fd, &readfds);
		struct timeval tv = {TIMEOUT, 0}; //wait 1 sec until timeout
		int retval = select(server_fd + 1, &readfds, NULL, NULL, &tv); // is this right?

		if (retval == -1) fatal("select error");
		if (retval) {
			sem_wait(&threads);
    		int client_fd = Accept(server_fd, NULL, NULL);
			if (client_fd < 0) fatal("Error accepting client connection");

			//FD_SET(client_fd, &readfds);
			
			//dynamically-allocated instance of ConnInfo
			struct ConnInfo *info = malloc(sizeof(struct ConnInfo));
			info->client_fd = client_fd;
			info->server_fd = server_fd;
			info->calc = calc;


			pthread_t thr_id;

			//create a thread for each accepted client connection
			if (pthread_create(&thr_id, NULL, worker, info) != 0) {
				fatal("pthread_create failed");
			}
		} else { //retval == 0, timeout, check shutdown_request
			if (shutdown_request == 1) break;
		}
	}
	//shutting down
	close(info->server_fd);
	for (int i = 0; i < MAX_NUM_THREADS; i++) {
		sem_wait(&threads);
	}

	calc_destroy(calc);
	sem_destroy(&threads);

	return 0;
}


