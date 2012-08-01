#include	<sys/types.h>
#include	<sys/socket.h>		/* struct msghdr */
#include	<sys/uio.h>			/* struct iovec */
#include	<stddef.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#define 	MAXLINE 4096


static struct cmsghdr	*cmptr = NULL;		/* malloc'ed first time */

#define	CONTROLLEN	(sizeof(struct cmsghdr) + sizeof(int))
int send_fd(int , int );
int recv_fd(int );