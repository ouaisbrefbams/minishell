
#include "lexer.h"

int 			len_is_not_sep(char *input)
{
	int i;

	i = -1;
	while(input[++i])
	{
		if (input[i] == '\\')
			i++;

		if (input[i] == '\\' && input[i + 1] == ' ')
			i += 2;
		if (lexer_sep(input[i]))
		{
			if (input[i + 1] == ' ')
				while(input[++i] == ' ')
					;
			return(i);
		}
		if (input[i] == '\'' || input[i] == '"')
		{
			i += lexer_verif_entre_cote(input, i);
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
	if (input[i] == '\\')
		i++;
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


int 					check_input_out(char *input)
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
			return(j);
		i += j;
		j = check_input(&input[i]);
		return(j);
	}
	return(0);
}

t_token 						*lexer_lst_token_str(char *input, int i, int j)
{
	t_token 					*lst_token;

	if (!(lst_token = malloc(sizeof(t_token) * 1)))
		return (NULL);
	lst_token->token = 0;
	lst_token->value = NULL;
	if (!(lst_token->value = malloc(sizeof(char) * j + 1)))
		return(0);
	if (!(ft_strlcpy(lst_token->value, &input[i], j + 1)))
	{
		free(lst_token);
		return(0);
	}
	printf("%s\n", lst_token->value);
	return (lst_token);
}

static t_ftlst				*create_token_list(char *input, t_ftlst **lst)
{
	t_token 				*lst_token;
	t_ftlst					*new;
	int 					i;
	int						j;

	i = 0;
	while (i < ft_strlen(input))
	{
		j = 0;
		j += check_input(&input[i]);
		lst_token = lexer_lst_token_str(input,i,j);
		new = ft_lstnew((void *) lst_token);
		ft_lstpush_back(lst, new);
		i += j;
	}
	return (*lst);
}

t_ftlst        			*lexer(char *input)
{
	t_ftlst				**lst;
	int 				i;

	if (!input)
		return (0);
	lst = malloc(sizeof(t_ftlst *) * 1);
	if (!lst)
		return(0);
	*lst = create_token_list(input, lst);
	i = ft_lstsize(*lst);
	printf("%d\n", i);
	free(lst);
	return (0);
}
