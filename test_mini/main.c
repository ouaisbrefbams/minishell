
#include "lexer.h"
#include "parse/parse.h"

int         main(int argc, char **argv)
{
    int i = 0;
    char *input;

    if (!(input = malloc(sizeof(char) * ft_strlen(argv[1]) + 2)))
        return(0);
    ft_strlcpy(input, argv[1], ft_strlen(argv[1]) + 1);
    i = ft_strlen(input);
    input[i + 1] = '\0';
    printf("%s\n",argv[1] );
    printf("%s\n",input );
	lexer(input);
    free(input);
	exit(0);
    return (0);
}
/*
#include <stdio.h>
int main(int argc, char **argv, char **envp)
{
    printf("ARGV:\n");
    for (int i = 0; i < argc; i++)
        printf("[%d] %s\n", i, argv[i]);
    printf("\nENV:\n");
    for (int i = 0; envp[i] != NULL && i < 10; i++)
        printf("[%d] %s\n", i, envp[i]);
    return 0;
}
*/
