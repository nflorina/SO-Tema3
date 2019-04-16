/*
 * Loader Implementation
 *
 * 2018, Operating Systems
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

#include "exec_parser.h"

static so_exec_t *exec;

static int fd;
static struct sigaction old_action;

static void usr2_handler(int signum, siginfo_t *info) {
    /* actions that should be taken when the signal signum is received */
	char *addr = (char *)info->si_addr;
	int case1 = 0, case2 = 0;
	//int pageSize = getpagesize();
	for (int i = 0; i < exec->segments_no; i ++) {
		uintptr_t vaddr = exec->segments[i].vaddr;
		unsigned int mem_size =  exec->segments[i].mem_size;
		if (addr > vaddr && addr < vaddr + mem_size) {
			case2 = 1;
			break;
		}
	}
	if (case2 == 0) 
		case1 = 1;   
}

int so_init_loader(void)
{
	/* TODO: initialize on-demand loader */
	struct sigaction sa;
 
    memset(&sa, 0, sizeof(sa));
    memset(&old_action, 0, sizeof(old_action));
 
    sa.sa_flags   = SA_RESETHAND | SA_SIGINFO;
    sa.sa_handler = usr2_handler;
    sigaction(SIGSEGV , &sa, &old_action);

	return -1;
}

int so_execute(char *path, char *argv[])
{
	fd = open(path, O_RDONLY);
	exec = so_parse_exec(path);
	if (!exec)
		return -1;

	so_start_exec(exec, argv);

	return -1;
}
