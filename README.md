# DwmStatus 
Display the common stauts for `dwm` status bar
## Platform Surpport
Only Linux
> Windows doesn't have these command
## Dependency
Any fonts with [Nerd Font](https://www.nerdfonts.com) patch
## Usage
1. Download or Compile it
2. Add it to your `autostart`
## Config
It's doesn't have any configuration file,  so you need to modify it by yourself.
### Change the command for get status
```c
//Battery
const char* GET_BATTERY_STAT = "cat /sys/class/power_supply/BAT1/status";
const char* GET_BATTERY_FULL = "cat /sys/class/power_supply/BAT1/energy_full";
const char* GET_BATTERY_NOW = "cat /sys/class/power_supply/BAT1/energy_now";

//Brightness
const char* GET_BRIGHTNESS = "xrandr --verbose | grep Brightness";

//Volume
const char* GET_VOLUME = "amixer get -c 0 Master | grep '[[0-9%]]'";
const char* VOLUME_UP = "pactl set-sink-volume 0 +10%";
const char* VOLUME_DOWM = "pactl set-sink-volume 0 -10%";
```
### Add & Remove any functions by yourself
