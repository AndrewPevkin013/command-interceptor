#define _GNU_SOURCE
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#ifndef RLTD_NEXT
#define RLTD_NEXT ((void *) -1)
#endif

const char *blacklist[] = {
	"/usr/bin/rm",
	"/usr/bin/shutdown",
	"/usr/sbin/reboot",
	NULL
};

int is_blocked(const char *cmd) {
	const char **ptr = blacklist;
	while (*ptr) {
		if (strcmp(cmd, *ptr) == 0)
			return 1;
		ptr++;
	}
	return 0;
}

int execve(const char *filename, char *const argv[], char *const envp[]) {
	static int (*original_execve)(const char *, char *const[], char *const[]) = NULL;

	if (!original_execve) {
		original_execve = dlsym(RLTD_NEXT, "execve");
	}

	FILE *log = fopen("/tmp/execve_log.txt", "a");
	if (log) {
		fprintf(log, "[LOG] user tried to execute: ");
		for (int i = 0; argv[i] != NULL; ++i) {
			fprintf(log, " %s", argv[i]);
		}
		fprintf(log, "\n");
		fclose(log);
	}

	if (is_blocked(filename)) {
		fprintf(stderr, "[SECURITY] Command %s is blocked by policy. \n", filename);
		errno = EPERM;
		return -1;
	}

	return original_execve(filename, argv, envp);
}
