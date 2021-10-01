#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

//надеюсь она работает как вы хотите

void Split(char* string,
    char* delimiters,
    char*** tokens,
    int* tokensCount) {

    char* p = strtok(string, delimiters);

    while (p != NULL) {

        strcpy((*tokens)[*tokensCount], p);
        (*tokensCount)++;
        p = strtok(NULL, delimiters);
    }
}



#define TIMEOUT 5

int main() {
    //как и в Split, числа 100 не несут огромнго смысла и взяты с потолка.
    int tokens_number = 100;
    int i = 0, tokens_count = 0, string_counter = 0, child_id = 0;          //заполнение нужных полей
    char stdin_delimiters[] = { ' ', '\n', '\0' };
    char stdin_string[100] = { 0 };


    FILE* fp; 
    fp = fopen("tasks.txt", "r");

    char** tokens = (char**)malloc(sizeof(char*) * tokens_number);

    for (i = 0; i < tokens_number; i++) {
        tokens[i] = (char*)calloc(100, sizeof(char));                       //динамическое выделение памяти под каждое слово
    } 

    fgets(stdin_string, 100, fp);
    string_counter = atoi(stdin_string);                                    //разбивем строку

    for (i = 0; i < string_counter; i++) {
        fgets(stdin_string, 100, fp);
        pid_t pid = fork();
        if (pid == 0) {
            child_id = getpid();
            Split((char*)stdin_string, (char*)stdin_delimiters, &tokens, &tokens_count);
            sleep(atoi(tokens[0])); 
            execvp(tokens[1], tokens + 1);
            exit(0);
        }
    }
    for (i = 0; i < string_counter; i++) {
        sleep(atoi(tokens[0]) + TIMEOUT);                                   //ждемс пока отработает ребенок
        int result = 0;
        result = waitpid(-1, NULL, WNOHANG);
        if (result > 0)
            break;                                                          //проверяем нормально ли завершилось
        else
            kill(child_id, SIGTERM);                                        //если нет, добиваем
    }
    for (i = 0; i < tokens_number; i++) {
        free(tokens[i]);
    }
    free(tokens);
    fclose(fp);
    return 0;
}

