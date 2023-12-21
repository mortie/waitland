VERSION = 1.0.0

CFLAGS := $(shell pkg-config --cflags wayland-client) $(CFLAGS) -DWAITLAND_VERSION='"$(VERSION)"'
LDFLAGS := $(shell pkg-config --libs wayland-client) $(LDFLAGS)

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
DESTDIR ?=

waitland: waitland.c

.PHONY: install
install: waitland
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 waitland $(DESTDIR)$(BINDIR)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/waitland

.PHONY: clean
clean:
	rm -f waitland
