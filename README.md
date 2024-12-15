# mmbctl

Commandline multi-monitor brightness controller for Linux. Written in C with the ddcutil library.

This is basically just a simpler program to use to set monitor brightness compared to using the `ddcutil` program. Here you just have to specify brightness values in the right order, with ddcutil an equivalent command is more complex (requiring explicit setting of the feature code for brightness, for instance).

Note that this program is for external monitors, this program will not work for setting laptop brightness because laptop brightness is controlled by a protocol other than I2C.

## Build

`mmbctl` is a Linux-only program. It was written, built, and tested on an Arch Linux system with a Dell and an Asus monitor connected to it.

Requirements:
- gcc
- make
- ddcutil

To build, simply clone the repository and type `make`.

You may have to run this program with root privileges. See this ArchWiki page on a way to get around this: <https://wiki.archlinux.org/title/Backlight#External_monitors>.

## Usage

All brightness values provided must be in the range of `[-1, 100]`. `[0, 100]` are valid brightness values. Specifying a brightness of `-1` will make `mmbctl` leave that monitor's brightness unmodified.

Try different brightness values until you see which of your monitors comes up as monitor `1`, `2`, etc.. The program should then become pretty straightforward to use, just supply the brightness values you want for the monitors you want to change the brightness of.

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

### Why is there an initial delay before the brightness gets set?

Checkout these links:

- <https://github.com/rockowitz/ddcutil/issues/240#issuecomment-991381421>
- <https://www.ddcutil.com/config_file/>

Tweaking the config file for ddcutil should allow you to minimize this delay if you so desire.

## License

GPLv3, see LICENSE file for details.
