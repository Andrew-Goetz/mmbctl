# mmbctl

Commandline multi-monitor brightness controller for Linux. Written in C with the [ddcutil](https://github.com/rockowitz/ddcutil) library.

## Motivation

This is basically just a simpler program to use to set monitor brightness compared to using the `ddcutil` program. Here you just have to specify brightness values in the right order, while with the `ddcutil` an equivalent command is more complex (requiring explicit setting of the feature code for brightness, for instance).

`ddcutil` is a more powerful tool of course, but most people probably just want to set their brightness and that's it. `mmbctl` addresses this use case.

Note that `mmbctl` is for external monitors only. `mmbctl` will not work for setting laptop brightness because laptop brightness is controlled by a protocol other than I2C.

## Build

`mmbctl` is a Linux-only program. It was written, built, and tested on an Arch Linux system with a Dell and an Asus monitor connected to it.

Requirements:
- ddcutil
- gcc
- make

To build, simply clone the repository, download required packages, navigate to the cloned directory in the terminal, and type `make`.

You may then type `./mmbctl` to launch the program, or move the compiled program into a directory included in your path (i.e `~/.local/bin/`).

You may have to run this program with root privileges. See this ArchWiki page on a way to get around this: <https://wiki.archlinux.org/title/Backlight#External_monitors>.

## Usage

```sh
Usage: mmbctl [brightness...]
```

All brightness values provided must be in the range of `[-1, 100]`. `[0, 100]` are valid brightness values. Specifying a brightness of `-1` will make `mmbctl` leave that monitor's brightness unmodified.

Try different brightness values until you see which of your monitors comes up as monitor `1`, `2`, etc.. The program should then become pretty straightforward to use, just supply the brightness values you want for the monitors you want to change the brightness of.

### Examples

```sh
# Set monitor 1 to 100% brightness
mmbctl 100

# Set monitor 2 to 0% brightness (lowest brightness, not completely off),
# but leave monitor 1 untouched
mmbctl -1 0

# Set monitor 1 to 50% brightness, skip monitor 2, and set monitor 3 to max brightness
mmbctl 50 -1 100
```

### Advanced Examples

It's easy to do interesting things with `mmbctl` without too much effort. Some examples of this I've thought of:

```sh
# Load stored brightness settings from a file
echo "25 0" > nighttime.txt
mmbctl $(cat nighttime.txt)

# Set brightness to random number for first 2 monitors
mmbctl $(shuf -i 0-100 -n 2)

# Open GUI window to set brightness for first 2 monitors (requires zenity)
mmbctl $(zenity --separator ' ' \
         --forms \
         --title "mmbctl" \
         --text "Input monitor brightness values in range [0-100]" \
         --add-entry="Monitor 1:" \
         --add-entry="Monitor 2:")
```

### Behavior on Failure

`mmbctl` tries its best to apply all brightness settings. If brightness fails to be set for the first monitor, an error message will be printed and `mmbctl` will move to the next specified monitor.

All output is printed to `stderr`. To suppress all output (not recommended), run `mmbctl` like this:

```sh
mmbctl 100 2> /dev/null
```

### Why is there an initial delay before the brightness gets set?

Checkout these links:

- <https://github.com/rockowitz/ddcutil/issues/240#issuecomment-991381421>
- <https://www.ddcutil.com/config_file/>

Tweaking the configuration file for `ddcutil` should allow you to minimize this delay if you so desire.

## License

GPLv3, see LICENSE file for details.
