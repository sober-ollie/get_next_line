/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waihein <waihein@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 15:50:11 by waihein           #+#    #+#             */
/*   Updated: 2026/11/09 15:50:58 by waihein          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

typedef struct s_stash
{
	char	*data;
	size_t	len;
	size_t	capacity;
}	t_stash;

int		init_stash(t_stash *stash);
int		expand_stash(t_stash *stash, size_t required);
int		append_stash(t_stash *stash, char *buffer, size_t bytes_read);
char	*extract_line(t_stash *stash);
void	update_stash(t_stash *stash, size_t line_len);
int		find_newline(t_stash *stash);
int		has_newline(t_stash *stash);
char	*get_next_line(int fd);
char	*cleanup_stash(t_stash *stash, char *buffer);
int		fill_stash(int fd, t_stash *stash, char *buffer);

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

#endif