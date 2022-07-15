```
                        ██
                        ██
                        ██
                        ██
                ██████████     ██    ██████████████
                ██      ██     ██    ██    ██    ██
                ██      ██     ██    ██    ██    ██
                ███████████████████████    ██    ██

|=------------------------------------------------------------------=|
|=------=[ 0xNexn Build of suckless dynamic window manager ]=-------=|
|=------------------------------------------------------------------=|
|=-----------------------=[    Itsnexn   ]=-------------------------=|
|=---------------------=[ 0xnexn@proton.me ]=-----------------------=|
|=------------------------------------------------------------------=|

--[ Table of contents

0 - Introduction
1 - Installation
2 - Hacking guides
3 - Other Links

--[ 0 - Introduction

dwm is a dynamic window manager for X. it means it can manage windows
in tiled, monocle and floating layouts. dwm is customized through
editing its source code so if you wan't to learn how it works best way
is reading the source. dwm is written in c which makes it fast as f.

--[ 1 - Installation

before you install the window manager you have to build it. you can use
gnu make to build the dwm.

enter:
$ sudo make install
this will install dwm for you.

you can either run it with startx or using your login manager.

to start with startx:
$ startx ~/path/to/repo/run.sh

and to run it from your login manager you have to create a *.desktop
file in the /usr/share/xsessions/

your desktop file should be something like this:
----------------------------------------------------------------------
[Desktop Entry]
Name=dwm
Comment=suckless dynamic window manager
Exec=/home/user/path/to/repo/run.sh
Type=Application
----------------------------------------------------------------------

now login and enjoy from your new window manager.

--[ 2 - Hacking guides


configuring this wm is really simple due to all the options in config.def.h
is explained with comments.

because this build is heavily patched you probably will have some problem
patching the source but you can easily read through the patch file and
apply the customizations.

--[ 3 - Other Links

[0]: 0xNexn Dotfiles - https://github.com/itsnexn/dotfiles
[1]: Catppuccin ColorScheme - https://github.com/catppuccin/catppuccin
[2]: Dwm Main page - https://dwm.suckless.org/
[3]: Dwm tutorials - https://dwm.suckless.org/tutorial/
[4]: Chadwm - https://github.com/siduck/chadwm/

|=[ EOF ]=----------------------------------------------------------=|
```
