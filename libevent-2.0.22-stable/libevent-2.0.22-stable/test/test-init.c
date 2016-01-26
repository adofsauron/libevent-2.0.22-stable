/*
 * Compile with:
 * cc -I/usr/local/include -o time-test time-test.c -L/usr/local/lib -levent
 */
#ifdef HAVE_CONFIG_H
//#include "event-config.h"
#endif


#include <sys/types.h>
#include <sys/stat.h>

# ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
# endif

#ifdef _EVENT_HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <event.h>

 static void
write_cb(int fd, short event, void *arg)
{
	const char *test = "test call back";
	int len;

	len = write(fd, test, strlen(test) + 1);

	printf("%s  %d: write %d -> %s\n",__FILE__,__LINE__,
		 len, test);

}

 static void
write_cb2(int fd, short event, void *arg)
 {
	 const char *test = "test call back 2";
	 int len;

	 len = write(fd, test, strlen(test) + 1);

	 printf("%s %d: write %d -> %s\n", __FILE__, __LINE__,
		 len, test);

 }

 static void
 write_cb3(int fd, short event, void *arg)
 {
	 const char *test = "test call back 3";
	 int len;

	 len = write(fd, test, strlen(test) + 1);

	 printf("%s %d: write %d -> %s\n", __FILE__, __LINE__,
		 len, test);

 }

 static void
read_cb(int fd, short event, void *arg)
{
	const int length = 256;
	char test[256] ;
	int len;

	len = read(fd, test, length);

	printf("%s %d: read %d -> %s\n",__FILE__,__LINE__,
		 len, test);

}

 static void
 add_event(int fd, short EV_FLAGS, void* (*callback) (int , short , void *))
 {
 	struct event ev;
 	event_set(&ev, fd, EV_FLAGS, callback, &ev);
	event_add(&ev, NULL);
 }

int
main(int argc, char **argv)
{
	struct event ev0, ev1, ev2, ev3;
	int pair[2];
	
	if (pipe(pair) == -1) 
	{
		if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair) == -1)
		 {
			perror("pipe");
			exit(1);		
		}
	}


	struct event_base *base = event_init();

	event_set(&ev0, pair[0], EV_READ, read_cb, &ev0);
	event_set(&ev1, pair[1], EV_WRITE, write_cb, &ev1);
	event_set(&ev2, pair[1], EV_WRITE, write_cb2, &ev2);
	event_set(&ev3, pair[1], EV_WRITE, write_cb3, &ev3);

	event_add(&ev0, NULL);
	event_add(&ev1, NULL);
	event_add(&ev2, NULL);
	event_add(&ev3, NULL);


	// add_event(pair[1], EV_WRITE, write_cb);
	// add_event(pair[0], EV_READ, read_cb);


	event_base_dispatch(base);

	return (0);
}

