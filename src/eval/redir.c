/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 11:05:34 by charles           #+#    #+#             */
/*   Updated: 2020/09/14 15:41:03 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

static int	st_open_replace(char *filename, int *fd, int oflag)
{
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

int			redir_extract(t_tok_lst **redirs, t_env env, int fds[2])
{
	t_tok_lst	*after;
	t_tok_lst	*curr;
	char		*filename;
	int			status;

	if (*redirs == NULL)
		return (0);
	if (!((*redirs)->tag & TAG_IS_REDIR) || (*redirs)->next == NULL
		|| !((*redirs)->next->tag & TAG_IS_STR))
		return (EVAL_FATAL);
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
	{
		tok_lst_destroy(redirs, free);
		tok_lst_destroy(&after, free);
		return (status);
	}
	if ((status = st_open_replace_dispatch(filename, fds, (*redirs)->tag)) != 0)
	{
		tok_lst_destroy(redirs, free);
		tok_lst_destroy(&after, free);
		return (status);
	}
	tok_lst_destroy(redirs, free);
	free(filename);
	return (redir_extract(&after, env, fds));
}
