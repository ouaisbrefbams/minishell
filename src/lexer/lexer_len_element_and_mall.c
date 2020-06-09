
#include "lexer.h"

int 		lexer_count_len_element(char *input, int i)
{
	int  j = -1;
	if (input[i] == '"' || input[i] == '\'')
    {
        return(j = lexer_verif_entre_cote(input,i));
    }
	else if (lexer_sep(input[i]) || input[i] == ' ')
	{
		while(lexer_sep(input[i]) || input[i] == ' ')
		{
			if(input[j] == '\0')
				return(j);
			++i;
			++j;
		}
	}
	else
	{
		while (!lexer_sep(input[i]) && input[i] != ' ')
		{
			if(input[j] == '\0')
				return(j);
			++i;
			++j;
		}
	}
	++j;
	return(j);
}

char 		**lexer_malloc_len_elem(char *input, int i, char **out)
{
	int j = 0;
	int k = 0;
	int temp = 0;

	(void)i;
	j += lexer_count_len_element(&input[j], 0);
	k = j;
	out[temp] = malloc(sizeof(char) * k);
	ft_strlcpy(out[temp], input, k + 1);
	while (input[j] != '\0')
	{
		temp++;
		j += lexer_count_len_element(&input[j], 0);
		k -= j;
		if (k < 0)
			k *= -1;
		out[temp] = malloc(sizeof(char) * j - k + 1);
		ft_strlcpy(out[temp], &input[j - k], k + 1);
		k = j;
	}
	return(out);
}
