# mmbctl

Commandline multi-monitor brightness controller for Linux. Written in C with the ddcutil library.

This is basically just a simpler program to use to set monitor brightness compared to using the `ddcutil` program. Here you just have to specify brightness values in the right order, with ddcutil an equivalent command is more complex (requiring explicit setting of the feature code for brightness, for instance).

Note that this program is for external monitors, this program will not work for setting laptop brightness, as that uses a different protocol than I2C to control.

## Build

`mmbctl` is a Linux-only program. It was written, built, and tested on an Arch Linux system with a Dell and an Asus monitor connected to it.

Requirements:
- gcc
- make
- ddcutil

To build, simply clone the repository and type `make`.

You may have to run this program with root privileges. See this ArchWiki page on a way to get around this: <https://wiki.archlinux.org/title/Backlight#External_monitors>.

## Usage

All brightness values provided must be in the range of `[0, 100]`. To skip setting a monitor's brightness, use any negative value (I recommend using `-1` though).

Try different brightness values until you see which of your monitors comes up as monitor `1`, `2`, etc.. The program should then become pretty straightforward to use, just supply the brightness values you want to the monitors you want to set.

Examples:

```sh
# Set monitor 1 to 100% brightness
mmbctl 100

# Set monitor 2 to 0% brightness (lowest brightness, not completely off),
# but leave monitor 1 untouched
mmbctl -1 0

# Set monitor 1 to 50% brightness, skip monitor 2, and set monitor 3 to max brightness
mmbctl 50 -1 100
```

## License

GPLv3, see LICENSE file for details.
