*This project has been created as part of the 42 curriculum by waihein.*

# get_next_line

## Description

`get_next_line` is a C function that returns one line per call from a file
descriptor, including the terminating newline when present. It was built as
part of the 42 curriculum to practise low-level I/O, persistent state, and
dynamic memory management.

The goal is to read any readable file descriptor without knowing the total
file size or line length in advance. The function returns `NULL` at end of file
or on error, and the caller must free each returned line.

Key properties:

- Works with regular files, standard input, and any other readable descriptor.
- Supports any positive `BUFFER_SIZE`, defined at compile time.
- Handles a final line without a trailing newline.
- Keeps only the unread remainder buffered between calls.
- Uses a function-local `static` stash instead of a global variable.

## Instructions

Compile the sources with your own `main.c`:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    get_next_line.c get_next_line_utils.c main.c -o gnl
```

`BUFFER_SIZE` is optional because the header defaults it to `42`. To test
another value, override it:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 \
    get_next_line.c get_next_line_utils.c main.c -o gnl
```

Useful test cases include `BUFFER_SIZE=1`, `BUFFER_SIZE=42`, a very large
buffer size, an empty file, and a file whose last line has no newline.

A minimal usage example:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc != 2)
		return (1);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

Run it with:

```sh
./gnl file.txt
```

To check for leaks and invalid accesses:

```sh
valgrind --leak-check=full --show-leak-kinds=all ./gnl file.txt
```

## Algorithm and design choices

The implementation keeps a persistent buffer called the **stash**, represented
by a dynamically allocated, NUL-terminated byte array with its current length
and capacity. It is stored in a `static` variable inside `get_next_line` and
holds bytes that were read but not yet returned as part of a line.

The algorithm proceeds as follows:

1. Validate `fd` and `BUFFER_SIZE`; return `NULL` if either is invalid.
2. Initialise the stash on the first call.
3. Read in chunks of `BUFFER_SIZE`, appending each chunk to the stash, until the
   stash contains `'\n'` or `read` returns `0` (end of file).
4. On a read or allocation error, free the stash and return `NULL`. If the stash
   is empty at end of file, return `NULL`.
5. Find the first newline. Allocate exactly `line_length + 1` bytes, copy the
   line including `'\n'` when present, and add the NUL terminator.
6. Shift any bytes after the newline to the front of the stash, so they become
   the start of the next line.
7. Return the allocated line; the caller frees it. The next call continues from
   the remaining stash contents.

**Why this design:**

- **Persistent stash:** `read` is not line-aware, and one read may contain the
  end of one line and the beginning of several others. Keeping the remainder is
  therefore necessary to avoid losing or re-reading data.
- **Capacity doubling:** The stash grows geometrically. Copying the whole stash
  after every read would cost quadratic time in the line length; doubling makes
  appending `L` bytes cost `O(L)` amortised.
- **Stop at the first newline:** This keeps memory proportional to the current
  line instead of buffering the whole file.
- **Contiguous stash and in-place compaction:** This avoids a complicated
  multi-chunk structure and keeps cleanup and error handling simple. The
  trade-off is that the unread remainder is shifted after each extraction,
  which is cheap for normal text but can cost up to about `BUFFER_SIZE` bytes
  per line for very short lines.
- **Single static stash:** This satisfies the mandatory single-descriptor
  requirement without globals. Supporting several descriptors simultaneously
  would require one stash per `fd`.

**Complexity:** For a line of length `L` and buffer size `B`, memory use is
`O(L + B)` and appending is `O(L)` amortised. Newline extraction is `O(L)`.
The read loop rescans the stash after each chunk to check for a newline, so one
enormous line without a newline has a worst case of `O(L² / B)`; for ordinary
text where line lengths are close to `B`, the work is effectively linear in the
number of bytes read.

## Resources

- 42 curriculum: the *Get Next Line* subject and evaluation sheet.
- `read(2)` — <https://man7.org/linux/man-pages/man2/read.2.html>
- `open(2)` / `close(2)` — <https://man7.org/linux/man-pages/man2/open.2.html>
- `malloc(3)` / `free(3)` — <https://man7.org/linux/man-pages/man3/malloc.3.html>
- GNU C Library Manual, “Low-Level Input/Output” —
  <https://www.gnu.org/software/libc/manual/html_node/Low_002dLevel-I_002fO.html>
- Valgrind User Manual — <https://valgrind.org/docs/manual/manual.html>
- Norminette — <https://github.com/42School/norminette>
- 42 community tutorials on file descriptors, static variables, and
  `get_next_line`.

### AI usage

AI was used as a supporting study and documentation aid:

- **Subject clarification:** edge cases such as empty files, missing trailing
  newlines, invalid file descriptors, `BUFFER_SIZE <= 0`, and repeated calls
  after end of file.
- **Algorithm review:** checking the explanation of stash growth, newline
  detection, extraction, compaction, and complexity.
- **Documentation:** drafting, structuring, and editing this README, including
  the compilation and usage examples.

All AI-assisted text and reasoning were reviewed by the student before being
included. The final implementation remains the student’s responsibility.
