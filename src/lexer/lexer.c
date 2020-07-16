#include "lexer.h"

int 			len_is_not_sep(char *input)
{
	int i;

	i = -1;
	while(input[++i])
	{
		if (input[i] == '\\')
			i += 1;
		if (lexer_sep(input[i]))
			return(i);
		if (input[i] == '\'' || input[i] == '"')
			return(i);
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
	int 				op;

	i = 0;
	op = 1;
	if (input[i] == '\\' && lexer_sep(input[i + 1]))
		i += 2;
	if (input[i] == '(' || input[i] == ')')
		return (i + 1);
	if (lexer_sep(input[i]))
	{
		while(input[i] == input[i + 1] && op < 2)
		{
			i++;
			op++;
		}
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

enum e_token_tag token_verif_stick(t_token *lst_token)
{
	int i;

	i = ft_strlen(lst_token->content);
	if (i > 0)
		if (lst_token->content[i - 1] == ' ')
			return(lst_token->tag);
	return(lst_token->tag | TAG_STICK);
}

enum e_token_tag token_str_or_cote(t_token *lst_token)
{
	int i;

	i = 0;
	while(lst_token->content[i] != '\0')
	{
		if(lst_token->content[i] == '\'')
		{
			lst_token->tag = TAG_STR_SINGLE;
			return(token_verif_stick(lst_token));
		}
		if(lst_token->content[i] == '"')
		{
			lst_token->tag = TAG_STR_DOUBLE;
			return(token_verif_stick(lst_token));
		}
		else
		{
			lst_token->tag = TAG_STR;
			return(token_verif_stick(lst_token));
		}
		i++;
	}
	return(0);
}

t_token			*push_token_enum(t_token *lst_token)
{
	enum e_token_tag 		tk;

	tk = ret_token(lst_token->content, 0);

	if (tk == 0)
		lst_token->tag = token_str_or_cote(lst_token);
	else
		lst_token->tag = tk;
	return (lst_token);
}

static t_ftlst				*create_token_list(char *input, t_ftlst **lst)
{
	t_token 				*lst_token;
	t_ftlst					*new;
	int 					i;
	int						j;

	i = 0;
	while (i < (int)ft_strlen(input))
	{
		j = 0;
		j += check_input(&input[i]);
		lst_token = token_new_until(0, input + i, j);
		lst_token = push_token_enum(lst_token);
		new = ft_lstnew(lst_token);
		ft_lstpush_back(lst, new);
		i += j;
	}
	return (*lst);
}

t_ftlst        			*lexer(char *input)
{
	t_ftlst		*lst;

	if (!input)
		return (NULL);
	lst = NULL;
	lst = create_token_list(input, &lst);
	lst = lexe_trim_out(lst);
	return (lst);
}
