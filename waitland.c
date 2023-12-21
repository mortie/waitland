#include <wayland-client.h>
#include <string.h>
#include <stdio.h>
#include <spawn.h>
#include <signal.h>

extern char **environ;

static void usage(char *argv0) {
	printf("Usage: %s [options] [command...].\n", argv0);
	puts("Options:");
	puts("  -h, --help:  Show this help message.");
	puts("  -q, --quiet: Disable log messages.");
}

int main(int argc, char **argv) {
	int quiet = 0;
	int i = 1;
	char **cmd = NULL;
	while (i < argc) {
		char *arg = argv[i];
		if (strcmp(arg, "--quiet") == 0 || strcmp(arg, "-q") == 0) {
			quiet = 1;
		} else if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
			usage(argv[0]);
			return 0;
		} else if (strcmp(arg, "--") == 0) {
			cmd = &argv[i] + 1;
			break;
		} else if (arg[0] == '-') {
			fprintf(stderr, "Unknown argument: '%s'. Use '--help' for help.\n", arg);
			return 1;
		} else {
			cmd = &argv[i];
			break;
		}
	}

	struct wl_display *disp = wl_display_connect(NULL);
	if (!disp) {
		perror("waitland: Failed to connect\n");
		return 1;
	}

	fputs("waitland: Connected.\n", stderr);

	pid_t child = -1;
	if (cmd) {
		int ret = posix_spawnp(&child, cmd[0], NULL, NULL, cmd, environ);
		if (ret != 0) {
			perror("waitland: Failed to spawn child process");
			return 1;
		}
	}

	// Wait for the compositor connection to close
	while (wl_display_dispatch(disp) > 0);

	fputs("waitland: Connection to compositor closed. Exiting.\n", stderr);
	if (child >= 0) {
		if (kill(child, SIGTERM) < 0) {
			perror("waitland: Killing child process failed");
			return 1;
		}
	}

	return 0;
}
