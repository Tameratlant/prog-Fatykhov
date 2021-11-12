#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
   	pid_t pid = fork();
	if (pid == 0) {
		execlp("gcc", "gcc", "-Wall", "-o", "Hello world to problem4", "Hello world to problem4.c", NULL);
	}
	execlp("./Hello world to problem4", "./Hello world to problem4", NULL);
	return 0;
}
