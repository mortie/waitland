# Waitland: Wait until the Wayland compositor exits

Waitland is the simplest possible Wayland client.
Its only purpose is to create a connection to the Wayland compositor,
then do absolutely nothing until the connection closes,
at which point waitland will exit.

Optionally, you can also make it spawn a child process after connecting to the
wayland compositor and then kill that child process when the connection closes.

The indeded use-case is for scriptch which need to clean up after themselves
when the Wayland compositor exits.

Waitland is licensed under the MIT license.

## Packages

* Fedora (Copr): [xmort/waitland](https://copr.fedorainfracloud.org/coprs/xmort/waitland/) (Official)

## Compile

To build Waitland, simply run `make`.

To install, run `make && sudo make install`.
This will simply copy waitland into `/usr/local/bin`.

The only dependency is `wayland-client`. It can be installed with:

* Fedora: `sudo dnf install wayland-devel`
* Debian/Ubuntu: `sudo apt install libwayland-dev`
* Arch Linux: `sudo pacman -S wayland` (though you probably have this installed already)

## Usage

```
Usage: waitland [options] [command...].
Options:
  -h, --help:  Print this help message and exit.
  --version:   Print version and exit.
  -q, --quiet: Disable log messages.
```

## Example: Sway scratchpad script

The way I use Sway, I like to have exactly one terminal window in a scratchpad at all times.
This makes Sway's scratchpad functionality function as a pop-up terminal.
For this to work nicely, the scratchpad terminal needs to be automatically re-opened
if it exits for any reason.

A naive `scratchpad.sh` script to automatically run a terminal is:

```
#!/bin/sh
while :; do
    kitty --class scratchpad
    echo "Kitty exited with code $?, restarting" >&2
done
```

Combined with these two lines in `~/.config/sway/config`, the scratchpad more or less works:

```
exec /path/to/scratchpad.sh
for_window [app_id="scratchpad"] move scratchpad
```

However, if you exit sway, the `scratchpad.sh` script doesn't get killed.
Moreover, if you start Sway again, you will end up with *two* `scratchpad.sh` scripts
running at the same time.
Restart Sway a bunch more times and you'll suddenly have dozens of unkillable kitties.
Disaster!

Waitland to the rescue! We just need to modify `~/.config/sway/config` to
run the scratchpad.sh script under waitland.

Replace:

```
exec /path/to/scratchpad.sh
```

with:

```
exec waitland /path/to/scratchpad.sh
```

and the kitty hordes will cease.

## Example: Making the script itself smarter with waitland

In the previous example, the script remained naive, and we fixed it by
running it with putting `waitland` in the Sway config.
That works well, but in some cases, it would be nice if the script itself was
smart enough to exit when the compositor dies.

We can modify the `scratchpad.sh` script like this:

```
#!/bin/sh
# Start the kitty spawner in the background
while :; do
    kitty --class scratchpad
    echo "Kitty exited with code $?, restarting" >&2
done &
kitty_spawner=$!

# Wait for the compositor to die
waitland

# Kill the kitty spawner to ensure no more kitties will spawn
kill $kitty_spawner
```

With this smart `scratchpad.sh` script, we can keep the Sway config simple:

```
exec /path/to/scratchpad.sh
```

and yet we won't be overrun by hordes of unkillable kitties.
