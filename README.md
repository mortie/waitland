# Waitland: wait until the Wayland compositor exits

Waitland is the simplest possible Wayland client program.
Its only purpose is to create a connection to the Wayland compositor,
then do absolutely nothing until the connection closes,
at which point waitland will exit.

The indeded use-case is for scriptch which need to clean up after themselves
when the Wayland compositor exits.

## Compile

Simply run `make`.

The only dependency is `wayland-client`. It can be installed with:

* Fedora: `sudo dnf install wayland-devel`
* Debian/Ubuntu: `sudo apt install libwayland-dev`
* Arch Linux: `sudo apt install wayland` (though you probably have this installed already)

## Usage

```
Usage: waitland [options] -- Wait until the Wayland compositor exits.
Options:
  -h, --help:  Show this help message.
  -q, --quiet: Disable log messages.
```
