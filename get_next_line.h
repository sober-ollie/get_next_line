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

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

#endif