
#include "lexer.h"
#include "parse/parse.h"


int 			len_is_not_sep(char *input)
{
	int i;

	i = -1;
	while(input[++i])
	{
		if (lexer_sep(input[i]))
		{
			if (input[i + 1] == ' ')
				while(input[++i] == ' ')
					;
			return(i);
		}
		if (input[i] == '\'' || input[i] == '"')
		{
			i = lexer_verif_entre_cote(input, i);
			if (input[i] == ' ')
				return(i + 1);
			return(i);
		}
		if (input[i] == ' ')
		{
			while(input[++i] == ' ')
				;
			return (i);
		}
	}
	return(i);
}

int				check_input(char *input)
{
	int 				i;

	i = 0;
	if (lexer_sep(input[i]))
	{
		while(input[i] == input[i + 1])
			i++;
		i += lexe_space(&input[i + 1]);
		return (i + 1);
	}
	if (input[i] == 39 || input[i] == '"')
		return(lexer_verif_entre_cote(input, i));
	if (input[i] == ' ')
	{
		while(input[++i] == ' ')
			;
		return (i);
	}
	return(len_is_not_sep(&input[i]));
}

void  					check_input_out(char *input)
{
	int i;
	int j;
	char *str;

	i = 0;
	while(input[i] != '\0')
	{
		j = 0;
		j += len_is_not_sep(&input[i]);
		if (j != 0)
		{
			str = malloc(sizeof(char) * j + 1);
			ft_strlcpy(str, &input[i], j + 1);
			printf("%s%d\n",str, j);
			free(str);
		}
		i += j;
		j = check_input(&input[i]);
		str = malloc(sizeof(char) * j + 1);
		ft_strlcpy(str, &input[i], j + 1);
		printf("%s%d\n",str, j);
		free(str);
		i += j;
	}

}

t_token					*create_token_list(void)
{
	t_token *lst_token;

	if (!(lst_token = malloc(sizeof(t_token) * 1)))
		return (NULL);

	free(lst_token);
	return (lst_token);
}

t_ftlst        			*lexer(char *input)
{
	t_ftlst				*lst;

	if (!input)
		return (0);
	if(!(lst = malloc(sizeof(t_ftlst) * 1)))
		return(NULL);

	check_input_out(input);
	free(lst);
	return (0);
}
