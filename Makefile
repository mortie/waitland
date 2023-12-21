CFLAGS := $(shell pkg-config --cflags wayland-client) $(CFLAGS)
LDFLAGS := $(shell pkg-config --libs wayland-client) $(LDFLAGS)

waitland: waitland.c

.PHONY: clean
clean:
	rm -f waitland
