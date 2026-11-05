#include "get_next_line.h"

int	find_newline(t_stash *stash)
{
	int	i;

	i = 0;
	while (i < stash->len)
	{
		if (stash->data[i] == '\n')
			return (i);
		i++;
	}

	return (-1);
}