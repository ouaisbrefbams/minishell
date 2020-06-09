

#include <stdio.h>

static int  pass_parenthese(char **input, int i, int j)
{
    while(input[i][j++])
        if (input[i][j] == ')')
            return (i);
    return (i);
}

int          verif_part(char **input, char chr)
{
    int i;
    int j;

    i = 0;
    while (input[i])
    {

        j = 0;
        while (input[i][j])
        {
            if (input[i][j] == '(')
                pass_parenthese(input, i, j);
            if (input[i][j] == chr)
                return (i);
            j++;
        }

        i++;
    }
    return (0);
}
