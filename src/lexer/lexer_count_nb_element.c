
#include "lexer.h"

int             lexer_count_nb_element(char *input)
{
    int         i;
    int         j;

    i = 0;
    j = 1;
    while(input[i] != '\0')
    {
        if (lexer_sep(input[i]) || input[i] == '"' || input[i] == '\'')
        {
            if (input[i] == '"' || input[i] == '\'')
            {
                i = lexer_verif_entre_cote(input,i);
                j++;
            }
            if (lexer_sep(input[i]))
                {
                    while (lexer_sep(input[i]) || input[i] == ' ')
                        ++i;
                    j++;
                }
            j++;
        }
        i++;
    }
    return (j);
}
