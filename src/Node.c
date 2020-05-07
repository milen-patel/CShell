#include "Node.h"
#include "Guards.h"

Node* ErrorNode_new(const char *msg) {
	Node* rval = malloc(sizeof(Node));
	OOM_GUARD(rval, __FILE__, __LINE__);
	rval->type = ERROR_NODE;
	rval->data.error = msg;
	return rval;
}

Node* CommandNode_new(StrVec words) {
	Node* rval = malloc(sizeof(Node));
	OOM_GUARD(rval, __FILE__, __LINE__);
	rval->type = COMMAND_NODE;
	rval->data.command = words;
	return rval;
}

Node* PipeNode_new(Node *left, Node *right) {
	Node* rval = malloc(sizeof(Node));
	OOM_GUARD(rval, __FILE__, __LINE__);
	rval->type = PIPE_NODE;
	rval->data.pipe.left = left;
	rval->data.pipe.right = right;
	return rval;
}

void* Node_drop(Node *self) {
	/* Handle By Cases */
	if (self->type == ERROR_NODE) {
		free(self);
	} else if (self->type == COMMAND_NODE) {
		StrVec_drop(&(self->data.command));
		free(self);
	} else if (self->type == PIPE_NODE) {
		/* Pipe node is special because you have to recursively free the command nodes */
		Node_drop(self->data.pipe.left);
		Node_drop(self->data.pipe.right);
		free(self);
	}
	return NULL;
}
