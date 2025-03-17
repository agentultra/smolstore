#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INSERT_COMMAND_PREFIX ".insert "

enum EvalResult {
    RESULT_OK,
    RESULT_ERROR,
    RESULT_QUIT
};

enum EvalResult smol_eval(const char* input) {
    if (strcmp(".quit", input) == 0) {
        return RESULT_QUIT;
    } else if (strncmp(INSERT_COMMAND_PREFIX, input, strlen(INSERT_COMMAND_PREFIX)) == 0) {
        return RESULT_OK;
    } else {
        return RESULT_ERROR;
    }
}

void smol_repl() {
    bool running = true;
    while (running) {
        char* line = readline("smol> ");
        if (line) {
            enum EvalResult result = smol_eval(line);
            free(line);
            switch (result) {
            case RESULT_OK:
                printf("Command OK!\n");
                break;
            case RESULT_ERROR:
                printf("Command ERROR!\n");
                break;
            case RESULT_QUIT:
                printf("Goodbye!\n");
                running = false;
                break;
            }
        }
    }
}

int main() {
    smol_repl();
    return 0;
}
