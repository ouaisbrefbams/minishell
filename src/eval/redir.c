/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 11:05:34 by charles           #+#    #+#             */
/*   Updated: 2020/08/28 17:14:19 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eval.h"

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
		g_last_status_code = 1;
		errorf("%s: %s\n", filename, strerror(errno));
		free(filename);
		return (false);
	}
	return (true);
}

bool					redir_extract(
		t_tok_lst **redirs,
		t_env env,
		int fds[2])
{
	t_tok_lst	*after;
	t_tok_lst	*curr;
	char		*filename;

	if (*redirs == NULL)
		return (true);
	if (!((*redirs)->tag & TAG_IS_REDIR) || (*redirs)->next == NULL
		|| !((*redirs)->next->tag & TAG_IS_STR))
		return (false);
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
	if ((filename = preprocess_filename(&(*redirs)->next, env)) == NULL)
	{
		tok_lst_destroy(redirs, free);
		tok_lst_destroy(&after, free);
		return (false);
	}
	if (((*redirs)->tag == TAG_REDIR_IN
			&& !st_open_replace(&fds[FDS_READ], filename, O_RDONLY))
		|| ((*redirs)->tag == TAG_REDIR_OUT
			&& !st_open_replace(&fds[FDS_WRITE], filename, O_WRONLY | O_CREAT | O_TRUNC))
		|| ((*redirs)->tag == TAG_REDIR_APPEND
			&& !st_open_replace(&fds[FDS_WRITE], filename, O_WRONLY | O_CREAT | O_APPEND)))
	{
		tok_lst_destroy(redirs, free);
		tok_lst_destroy(&after, free);
		return (false);
	}
	tok_lst_destroy(redirs, free);
	free(filename);
	return (redir_extract(&after, env, fds));
}
