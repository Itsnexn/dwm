#!/bin/sh

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

xrdb merge ~/.Xresources
picom & # Compositor
/usr/lib/polkit-kde-authentication-agent-1 & # polkit
/usr/bin/mpd > /dev/null 2>&1 & # Start music player daemon
xset -dpms & # Disable DPMS

$SCRIPT_DIR/bar.sh &
nitrogen --restore

# Start dwm
while type dwm >/dev/null; do
    dwm && continue || continue
done
