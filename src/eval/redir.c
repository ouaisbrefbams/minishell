/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 11:05:34 by charles           #+#    #+#             */
/*   Updated: 2020/06/15 11:44:38 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

static enum e_token_tag	st_lst_tag(t_ftlst *lst)
{
	return (((t_token*)lst->data)->tag);
}

static bool				st_open_replace(int *fd, char *filename, int oflag)
{
	if (*fd != MS_NO_FD)
		close(*fd);
	if (oflag & O_CREAT)
		*fd = open(filename, oflag, 0644);
	else
		*fd = open(filename, oflag);
	if (*fd == -1)
	{
		error_eval_put(ERROR_OPEN, filename);
		free(filename);
		return (false);
	}
	return (true);
}

bool					redir_extract(
		t_ftlst *redirs,
		t_env env,
		int *fd_in,
		int *fd_out)
{
	t_ftlst	*after;
	t_ftlst	*curr;
	char	*filename;

	if (redirs == NULL)
		return (true);
	if (!(st_lst_tag(redirs) & TAG_IS_REDIR) || redirs->next == NULL
		|| !(st_lst_tag(redirs->next) & TAG_IS_STR))
		return (false);
	curr = redirs->next;
	while (curr != NULL && st_lst_tag(curr) & TAG_IS_STR)
	{
		if (curr->next == NULL || st_lst_tag(curr->next) & TAG_IS_REDIR)
		{
			after = curr->next;
			curr->next = NULL;
		}
		curr = curr->next;
	}
	if ((filename = preprocess_filename(&redirs->next, env)) == NULL)
	{
		token_destroy_lst2(redirs, after);
		return (false);
	}
	if ((st_lst_tag(redirs) == TAG_REDIR_IN
			&& !st_open_replace(fd_in, filename, O_RDONLY))
		|| (st_lst_tag(redirs) == TAG_REDIR_OUT
			&& !st_open_replace(fd_out, filename, O_WRONLY | O_CREAT | O_TRUNC))
		|| (st_lst_tag(redirs) == TAG_REDIR_APPEND
			&& !st_open_replace(fd_out, filename, O_WRONLY | O_CREAT | O_APPEND)))
	{
		token_destroy_lst2(redirs, after);
		return (false);
	}
	token_destroy_lst(redirs);
	free(filename);
	return (redir_extract(after, env, fd_in, fd_out));
}
