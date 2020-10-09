/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 11:05:34 by charles           #+#    #+#             */
/*   Updated: 2020/10/09 14:38:16 by cacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

static int	st_open_replace(char *filename, int *fd, int oflag)
{
	if (fd == NULL)
		return (EVAL_FATAL);
	if (*fd != FD_NONE)
		close(*fd);
	if (oflag & O_CREAT)
		*fd = open(filename, oflag, 0644);
	else
		*fd = open(filename, oflag);
	if (*fd == -1)
	{
		errorf("%s: %s\n", filename, strerror(errno));
		free(filename);
		return (1);
	}
	return (0);
}

static int	st_open_replace_dispatch(char *filename, int fds[2], enum e_tok tag)
{
	int	*fd;
	int	oflag;

	fd = NULL;
	oflag = 0;
	if (tag == TAG_REDIR_IN)
	{
		fd = &fds[FD_READ];
		oflag = O_RDONLY;
	}
	else if (tag == TAG_REDIR_OUT)
	{
		fd = &fds[FD_WRITE];
		oflag = O_WRONLY | O_CREAT | O_TRUNC;
	}
	else if (tag == TAG_REDIR_APPEND)
	{
		fd = &fds[FD_WRITE];
		oflag = O_WRONLY | O_CREAT | O_APPEND;
	}
	return (st_open_replace(filename, fd, oflag));
}

static int	st_tok_lsts_destroy_ret(
	int ret, t_tok_lst **tokens1, t_tok_lst **tokens2)
{
	tok_lst_destroy(tokens1, free);
	tok_lst_destroy(tokens2, free);
	return (ret);
}

int			redir_extract(t_tok_lst **redirs, t_env env, int fds[2])
{
	t_tok_lst	*after;
	t_tok_lst	*curr;
	char		*filename;
	int			status;

	if (*redirs == NULL)
		return (0);
	curr = (*redirs)->next;
	after = NULL;
	while (curr != NULL && curr->tag & TAG_IS_STR)
	{
		if (curr->next == NULL || curr->next->tag & TAG_IS_REDIR)
		{
			after = curr->next;
			curr->next = NULL;
		}
		curr = curr->next;
	}
	if ((status = preprocess_filename(&(*redirs)->next, env, &filename)))
		return (st_tok_lsts_destroy_ret(status, redirs, &after));
	if ((status = st_open_replace_dispatch(filename, fds, (*redirs)->tag)) != 0)
		return (st_tok_lsts_destroy_ret(status, redirs, &after));
	tok_lst_destroy(redirs, free);
	free(filename);
	return (redir_extract(&after, env, fds));
}
