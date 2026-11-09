/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waihein <waihein@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 15:50:22 by waihein           #+#    #+#             */
/*   Updated: 2026/11/09 15:51:18 by waihein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	find_newline(t_stash *stash)
{
	size_t	i;

	i = 0;
	while (i < stash->len)
	{
		if (stash->data[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

int	has_newline(t_stash *stash)
{
	size_t	i;

	i = 0;
	while (i < stash->len)
	{
		if (stash->data[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*cleanup_stash(t_stash *stash, char *buffer)
{
	free(stash->data);
	free(buffer);
	stash->data = NULL;
	stash->len = 0;
	stash->capacity = 0;
	return (NULL);
}

int	fill_stash(int fd, t_stash *stash, char *buffer)
{
	ssize_t	bytes_read;

	while (!has_newline(stash))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (0);
		if (bytes_read == 0)
			break ;
		if (!append_stash(stash, buffer, bytes_read))
			return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static t_stash	stash;
	char			*buffer;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash.data && !init_stash(&stash))
		return (NULL);
	buffer = malloc(BUFFER_SIZE);
	if (!buffer)
		return (NULL);
	if (!fill_stash(fd, &stash, buffer))
		return (cleanup_stash(&stash, buffer));
	if (stash.len == 0)
	{
		free(buffer);
		return (NULL);
	}
	line = extract_line(&stash);
	free(buffer);
	return (line);
}
