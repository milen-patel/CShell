#include "Exec.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define FORKED_CHILD 0

typedef struct Context {
	int fd[2]; /* For standard IO */
	int fd_close; /* Close this fd, -1 if not */
} Context;

static int exec_node(Node *node, Context *ctx);
static int exec_command(Node *node, Context *ctx);
static int exec_pipe(Node *node, Context *ctx);

void exec(Node *node) {
	Context initial = {
		{STDIN_FILENO, STDOUT_FILENO},
		-1
	};

	/* Wait on children */
	int childrenSpawned = exec_node(node, &initial);
	for (int i=0; i< childrenSpawned; i++) {
		wait(NULL);
	}
}

static int exec_node(Node *node, Context *ctx) {
	if (node->type == COMMAND_NODE) {
		return exec_command(node, ctx);					
	} else if (node->type == PIPE_NODE) {
		return exec_pipe(node, ctx);	
	} else {
		printf("TODO");
		return 1;
	}
}

static int exec_pipe(Node *node, Context *ctx) {
	/* Create Pipe */
	int p[2];
	pipe(p); /* p[0]=read, p[1]=write */

	int childrenSpawned = 0;

	/* Create context for LHS */
	Context lhs_context = *ctx;
	lhs_context.fd[STDOUT_FILENO] = p[STDOUT_FILENO]; /* Redirect STDOUT */
	lhs_context.fd_close = p[STDIN_FILENO]; /* Close pipe in since LHS doesn't need it */
	Node *lhs = node->data.pipe.left;
	childrenSpawned += exec_node(lhs, &lhs_context);
	
	/* Create Context for RHS */
	Context rhs_context = *ctx;
	rhs_context.fd[STDIN_FILENO] = p[STDIN_FILENO]; /* Redirect STDIN */
	rhs_context.fd_close = p[STDOUT_FILENO]; /* Close pipe out since RHS doesn't need it */
	Node *rhs = node->data.pipe.right;
	childrenSpawned += exec_node(rhs, &rhs_context);

	/* Close both sides of the pipe on the parents end */
	close(p[STDIN_FILENO]);
	close(p[STDOUT_FILENO]);

	return childrenSpawned;
}

static int exec_command(Node *node, Context *ctx) {
	int execSuccessVal = 1;
	if (fork() == FORKED_CHILD) {
		/* Evaluate Context */
		dup2(ctx->fd[STDIN_FILENO], STDIN_FILENO); /* Redirect STDIN */
		dup2(ctx->fd[STDOUT_FILENO], STDOUT_FILENO); /* Redirect STDOUT */
		if (ctx->fd_close >= 0) {
			/* If applicable, close the FD passed by context */
			close(ctx->fd_close);
		}

		StrVec *words = &node->data.command;
		size_t argc = StrVec_length(words);
		char *argv[argc + 1];
		argv[argc] = NULL; /* Make last character NULL */

		for(size_t i = 0; i<argc; i++) {
			Str *word = StrVec_ref(words, i);
			argv[i] = Str_ref(word, 0);
		}

		if(execvp(argv[0], argv) == -1) {
			execSuccessVal=0;
			//execSuccessVal = 1;
			//temp++;
			exit(0); /* If the forked process fails, kill it */
		}
	}
	return execSuccessVal; /* One child was spawned */	
}
