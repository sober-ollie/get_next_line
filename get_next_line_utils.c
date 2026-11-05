/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waihein <waihein@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 17:39:47 by waihein           #+#    #+#             */
/*   Updated: 2026/11/05 20:55:09 by waihein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	init_stash(t_stash *stash)
{
	stash->data = malloc(8);
	if (!stash->data)
		return (0);
	stash->data[0] = '\0';
	stash->len = 0;
	stash->capacity = 8;
	return (1);
}

int	expand_stash(t_stash *stash, size_t required)
{
	char	*new_data;
	size_t	i;
	size_t	new_capacity;

	i = 0;
	if (stash->capacity == 0)
		return (0);
	if (required <= stash->capacity)
		return (1);
	new_capacity = stash->capacity;
	while (required > new_capacity)
		new_capacity *= 2;
	new_data = malloc(new_capacity);
	if (!new_data)
		return (0);
	while (i < stash->len)
	{
		new_data[i] = stash->data[i];
		i++;
	}
	new_data[i] = '\0';
	free(stash->data);
	stash->data = new_data;
	stash->capacity = new_capacity;
	return (1);
}

int	append_stash(t_stash *stash, char *buffer, size_t bytes_read)
{
	size_t	required;
	size_t	i;

	i = 0;
	required = stash->len + bytes_read + 1;
	if (!expand_stash(stash, required))
		return (0);
	while (i < bytes_read)
	{
		stash->data[stash->len + i] = buffer[i];
		i++;
	}
	stash->data[stash->len + i] = '\0';
	stash->len = stash->len + bytes_read;
	return (1);
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

char	*extract_line(t_stash *stash)
{
	char	*line;
	size_t	i;
	size_t	line_len;
	int		newline_pos;

	i = 0;
	newline_pos = find_newline(stash);
	if (newline_pos == -1)
		line_len = stash->len;
	else
		line_len = newline_pos + 1;
	line = malloc(line_len + 1);
	if (!line)
		return (NULL);
	while (i < line_len)
	{
		line[i] = stash->data[i];
		i++;
	}
	line[line_len] = '\0';
}