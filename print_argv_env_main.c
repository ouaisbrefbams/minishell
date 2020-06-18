#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char **argv, char **envp)
{
	char buf[PATH_MAX];

	getcwd(buf, PATH_MAX);
	printf("%s\n", buf);
	/* printf("ARGV:\n"); */
	/* for (int i = 0; i < argc; i++) */
	/* 	printf("[%d] %s\n", i, argv[i]); */
	/* printf("\nENV:\n"); */
	/* for (int i = 0; envp[i] != NULL && i < 10; i++) */
	/* 	printf("[%d] %s\n", i, envp[i]); */
	return 0;
}
