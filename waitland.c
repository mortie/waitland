#include <wayland-client.h>
#include <string.h>
#include <stdio.h>

static void usage(char *argv0) {
	printf("Usage: %s [options] -- Wait until the Wayland compositor exits.\n", argv0);
	puts("Options:");
	puts("  -h, --help:  Show this help message.");
	puts("  -q, --quiet: Disable log messages.");
}

int main(int argc, char **argv) {
	int quiet = 0;
	int i = 1;
	while (i < argc) {
		char *arg = argv[i];
		if (strcmp(arg, "--quiet") == 0 || strcmp(arg, "-q") == 0) {
			quiet = 1;
		} else if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
			usage(argv[0]);
			return 0;
		} else {
			fprintf(stderr, "Unknown argument: '%s'. Use '--help' for help.\n", arg);
			return 1;
		}
	}

	struct wl_display *disp = wl_display_connect(NULL);
	if (!disp) {
		perror("waitland: Failed to connect\n");
		return 1;
	}

	fputs("waitland: Connected.\n", stderr);
	while (wl_display_dispatch(disp));

	fputs("waitland: Connection to compositor closed, exiting.\n", stderr);
	return 0;
}
