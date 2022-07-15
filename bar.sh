#!/bin/bash

# ^c$var^ = fg color
# ^b$var^ = bg color

# load colors
black="#1E1D2D"
green="#ABE9B3"
white="#D9E0EE"
grey="#282737"
blue="#96CDFB"
red="#F28FAD"
darkblue="#83bae8"
purple="#cba6f7"
pink="#f5c2e7"
yellow="#f9e2af"

cpu() {
    cpu_val=$[100-$(vmstat 1 2|tail -1|awk '{print $15}')]
    echo -ne "^c$green^ CPU $cpu_val%"
}

battery() {
    get_capacity="$(cat /sys/class/power_supply/BAT0/capacity)"
    printf "^c$blue^   $get_capacity"
}

brightness() {
    printf "^c$red^   "
    printf "^c$red^%.0f\n" $(xbacklight -get)
}

mem() {
    printf "^c$blue^^b$black^  "
    printf "^c$blue^ $(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)"
}

wlan() {
	case "$(cat /sys/class/net/wl*/operstate 2>/dev/null)" in
	up) printf "^c$black^ ^b$yellow^ 󰤨 ^d^%s" " ^c$yellow^Connected" ;;
	down) printf "^c$black^ ^b$yellow^ 󰤭 ^d^%s" " ^c$yellow^Disconnected" ;;
	esac
}

clock() {
	printf "^c$black^ ^b$darkblue^ 󱑆 "
	printf "^c$black^^b$blue^ $(date '+%H:%M')  "
}

vol(){
    volume=$(pactl get-sink-volume @DEFAULT_SINK@ | tail -n 2 | sed -e 's,.* \([0-9][0-9]*\)%.*,\1,' | head -n 1)
    printf "^c$black^ ^b$purple^  "
    echo -ne "^c$purple^^b$black^ $volume%"
}

while true; do
    xsetroot -name "$(battery) $(brightness) $(cpu) $(mem) $(wlan) $(vol) $(clock)"
    sleep 1
done

