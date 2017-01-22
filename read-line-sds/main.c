#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "sds.h"

int read_line(sds *buf, sds *line, int fd)
{
	int i = 0;
	char tmp[1024];
	for(;;) {
		if( (*buf)[i] == '\0' ) {
			// reached end of input, read more
			int j = read( fd, tmp, 1024 );
			if( j == 0 ) {
				*line = sdscpy( *line, *buf );
				return 1;
			}
			*buf = sdscatlen( *buf, tmp, j );
		}
		if( (*buf)[i] == '\n' ) {
			sdscpylen( *line, *buf, i );
			int j = i + 1;
			memmove( *buf, *buf + j, strlen(*buf + j) + 1 );
			sdsupdatelen( *buf );
			return 0;
		}
		i++;
	}
}


int main()
{

	int fd[2];
	pipe( fd );
	const char *s = "hello\nworld\nbye\nbye";
	write( fd[1], s, strlen(s) );
	close( fd[1] );

	sds buf = sdsempty();
	sds line = sdsempty();

	int res = 0;
	while( !res ) {
		res = read_line(&buf, &line, fd[0] );
		printf( "got: %s\n", line );
	}

	return 0;
}

