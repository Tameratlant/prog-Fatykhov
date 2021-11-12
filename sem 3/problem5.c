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

//Объявляем define:

#define TIMEOUT 5
#define max_string_lenght 100
#define max_tokens_count 100
#define max_token_lenght 100

// ваш код прям 1 в 1 совпадает с кодом, который уже проверял

int main() {
   
    //int tokens_number = 100;
    int i = 0; 
    int tokens_count = 0; 
    int string_counter = 0; 
    int child_id = 0;          //заполнение нужных полей
    char stdin_delimiters[] = { ' ', '\n', '\0' };
    char stdin_string[max_string_lenght] = { 0 };


    FILE* fp;
    if ((fp = fopen("tasks.txt", "r")) < 0) {
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }

    char** tokens = (char**)malloc(sizeof(char*) * max_tokens_count);

    for (i = 0; i < max_tokens_count; i++) {
        tokens[i] = (char*)calloc(max_tokens_count, sizeof(char));                       //динамическое выделение памяти под каждое слово
    }

    fgets(stdin_string, max_string_lenght, fp);
    string_counter = atoi(stdin_string);                                    //разбивем строку

    for (i = 0; i < string_counter; i++) {
        fgets(stdin_string, max_string_lenght, fp);

        if ((pid_t pid = fork()) < 0) {
            printf("Can\'t fork child\n");
            exit(-1);
        }
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

    for (i = 0; i < max_tokens_count; i++) {
        free(tokens[i]);
    }

    free(tokens);
    fclose(fp);
    return 0;
}
