#include <wait.h>   
#include "listnode.h"
#include "exec.h"
#include <string.h>

//////////////////////////////////////////// change back to wait .h when submitting 
typedef struct cla{
	char *command;
	char *argsarr[20];
}cla;


typedef struct historyBuffer{
	int head;
	int tail;
	int count;
	char *commands[100];
}historyBuffer;

char* get_input(char *buffer){


		printf("262$");
		fgets(buffer,10000,stdin);
		fflush(stdout);

	return buffer;
}



void parse(char *buffer, cla *s) {
    s->command = strtok(buffer, " \t\n");  // Set the command

    char *arg;
    int i = 0;
    while ((arg = strtok(NULL, " \t\n")) != NULL && i < 20) {  
        s->argsarr[i++] = arg; 
    }
    s->argsarr[i] = NULL;  
}

void quit(ListNode *head, historyBuffer *hsb) {
    // Free the linked list (ListNode)
    ListNode *current = head;
    while (current != NULL) {
        ListNode *temp = current;
        free(temp->command);

        for (int i = 0; i < temp->arguments_length; i++) {
            free(temp->arguments[i]);
        }
        free(temp->arguments);

        if (temp->file_contents != NULL) {
            free(temp->file_contents);
        }

        current = current->next;
        free(temp);
    }

    // Free the history buffer
    for (int i = 0; i < 100; i++) {
        if (hsb->commands[i] != NULL) {
            free(hsb->commands[i]);
            hsb->commands[i] = NULL;
        }
    }

    // Exit the program
    exit(0);
}

int cd(cla *s){
	if(s->argsarr[1]!= NULL ){
		fprintf(stderr, "error:too many arguments provided\n");
		return 0;
	}
	
	if(s->argsarr[0] == NULL){
		return 0;
	}

	if( chdir(s->argsarr[0])== -1){
		fprintf(stderr,"error: %s\n",strerror(errno));
		return -1;
	}
	
	return 0;
}
int add_to_history(historyBuffer *hsb, cla *s) {
    if (s->command == NULL || strcmp(s->command, "") == 0 ||
        strcmp(s->command, "history") == 0 || strcmp(s->command, "history -c") == 0) {
        return 0;
    }

    if (hsb->commands[hsb->tail] != NULL) {
        free(hsb->commands[hsb->tail]);
    }

    hsb->commands[hsb->tail] = strdup(s->command);

    hsb->tail = (hsb->tail + 1) % 100;

    if (hsb->count == 100) {
        hsb->head = (hsb->head + 1) % 100;
    } else {
        hsb->count++;
    }

    return 1;
}


void history(historyBuffer *hsb, cla *s) {
    if (strcmp(s->command, "history") == 0 && s->argsarr[0] == NULL) {
        for (int i = 0; i < hsb->count; i++) {
            int index = (hsb->head + i) % 100;
            printf("%d: %s\n", i, hsb->commands[index]);
        }
        return;
    }

    if (strcmp(s->command, "history") == 0 && strcmp(s->argsarr[0], "-c") == 0) {
        for (int i = 0; i < hsb->count; i++) {
            free(hsb->commands[i]);
            hsb->commands[i] = NULL;
        }
        hsb->count = 0;
        hsb->head = 0;
        hsb->tail = 0;
        return;
    }

    if (strcmp(s->command, "history") == 0 && s->argsarr[0] != NULL) {
        int index = atoi(s->argsarr[0]);

        if (index < 0 || index >= hsb->count) {
            fprintf(stderr, "error: %s\n", "Index in history list does not exist");
            return;
        }

        int real_index = (hsb->head + index) % 100;
        char *command_to_execute = strdup(hsb->commands[real_index]);

        add_to_history(hsb, s);
        free(command_to_execute);
    }
}

void new(cla *s ,ListNode **head,int *next_id){
	   if (s->argsarr[0] == NULL) {
        fprintf(stderr, "error: too few arguments provided\n");
        return;
    }

    ListNode *node = malloc(sizeof(ListNode));
    node->id = (*next_id)++;
    node->command = strdup(s->argsarr[0]);

    int args_count = 0;
    while (s->argsarr[args_count] != NULL) {
        args_count++;
    }

    node->arguments = malloc((args_count + 1) * sizeof(char *));
    for (int i = 0; i < args_count; i++) {
        node->arguments[i] = strdup(s->argsarr[i]);
    }
    node->arguments[args_count] = NULL;
    node->arguments_length = args_count;

    node->file_contents = NULL;
    node->next = NULL;

    if (*head == NULL) {
        *head = node;
    } else {
        ListNode *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = node;
    }
}

void list(ListNode *head) {
    ListNode *current = head;

    while (current != NULL) {
        // Print List Node ID
        printf("List Node %d\n", current->id);

        // Print Command
        printf("\tCommand: %s\n", current->command);

        // Print File Contents
        if (current->file_contents != NULL) {
            char *file_copy = strdup(current->file_contents); 
            char *line = strtok(file_copy, "\n");             

            while (line != NULL) {
                printf("\t\t%s\n", line); 
                line = strtok(NULL, "\n");
            }

            free(file_copy); 
        }

        current = current->next;
    }
}


void open(cla *s, ListNode *head) {
    if (s->argsarr[0] == NULL || s->argsarr[1] == NULL) {
        fprintf(stderr, "error: incorrect number of arguments\n");
        return;
    }

    int id = atoi(s->argsarr[0]);
    ListNode *current = head;

    while (current != NULL && current->id != id) {
        current = current->next;
    }

    if (current == NULL) {
        fprintf(stderr, "error: %s\n", "Id does not exist");
        return;
    }

    FILE *file = fopen(s->argsarr[1], "r");
    if (file == NULL) {
        fprintf(stderr, "error: file cannot be opened\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        fclose(file);
        fprintf(stderr, "error: memory allocation failed\n");
        return;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    if (current->file_contents != NULL) {
        free(current->file_contents);
    }
    current->file_contents = buffer;
}

void execute(cla *s, ListNode *head) {
    if (s->argsarr[0] == NULL || s->argsarr[1] != NULL) {
        fprintf(stderr, "error: incorrect number of arguments\n");
        return;
    }

    int id = atoi(s->argsarr[0]);
    if (id == 0 && strcmp(s->argsarr[0], "0") != 0) {
        fprintf(stderr, "error: %s\n", "Invalid id argument");
        return;
    }

    ListNode *current = head;

    while (current != NULL && current->id != id) {
        current = current->next;
    }

    if (current == NULL) {
        fprintf(stderr, "error: %s\n", "Id does not exist");
        return;
    }

    if (current->arguments_length > 127) {
        fprintf(stderr, "error: %s\n", "too many arguments");
        return;
    }

    if (current->command == NULL || current->arguments == NULL) {
        fprintf(stderr, "error: %s\n", "Invalid ListNode structure");
        return;
    }

    int status = run_command(current);
    int exit_status = (status >> 8) & 0xFF;

    if (exit_status != 0) {
        fprintf(stderr, "error: %s\n", strerror(exit_status));
    }
}


void whatCommand(cla *s, ListNode **head, historyBuffer *hsb, int *next_id) {
    if (strcmp(s->command, "quit") == 0) {
        quit(*head, hsb);
    } else if (strcmp(s->command, "cd") == 0) {
        cd(s);
    } else if (strcmp(s->command, "history") == 0) {
        history(hsb, s);
    } else if (strcmp(s->command, "new") == 0) {
        new(s, head, next_id);
    } else if (strcmp(s->command, "list") == 0) {
        list(*head);
    } else if (strcmp(s->command, "open") == 0) {
        open(s, *head);
    } else if (strcmp(s->command, "execute") == 0) {
        // Parse the ID argument and find the matching ListNode
        if (s->argsarr[0] != NULL) {
            int id = atoi(s->argsarr[0]);
            ListNode *current = *head;

            while (current != NULL && current->id != id) {
                current = current->next;
            }

            if (current != NULL) {
                execute(s, current); // Pass both cla *s and ListNode *current
            } else {
                fprintf(stderr, "error: %s\n", "Id does not exist");
            }
        } else {
            fprintf(stderr, "error: incorrect number of arguments\n");
        }
    } else {
        fprintf(stderr, "error: command not found\n");
    }
}


int main() {
    cla s;
    historyBuffer hsb = {0, 0, 0, {NULL}};
    ListNode *head = NULL;
    int next_id = 0;
    char buffer[10000];

    char *val = get_input(buffer);

    while (val != NULL) {
        parse(buffer, &s);
        add_to_history(&hsb, &s);
        whatCommand(&s, &head, &hsb, &next_id);
        val = get_input(buffer);
    }

    return 0;
}
