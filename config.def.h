/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
#include <stdlib.h>

/* appearance */

// border sz of windows
static const unsigned int borderpx  = 2;

// snap pixel
static const unsigned int snap      = 32;

// inner gap between windows
static const unsigned int gappih    = 10; // horizontal
static const unsigned int gappiv    = 10; // vertical

// outer gap between win and screen
static const unsigned int gappoh    = 10; // horizontal
static const unsigned int gappov    = 10; // vertical

// smartgaps: no gap when there is only one window
static const int smartgaps          = 1;

// 0: sloppy systray follows selected monitor, >0: pin systray to monitor X
static const unsigned int systraypinning = 0;
// systray spacing
static const unsigned int systrayspacing = 10;

// systray icon size in px
static const unsigned int systrayiconsize = 16;
// 1: if pinning fails,display systray on the 1st monitor,False: display systray on last monitor*/
static const int systraypinningfailfirst = 1;
static const int showsystray        = 1; // 0: means no systray

static const int showbar            = 1; // 0 means no bar
// showtab_never, showtab_auto, showtab_nmodes, showtab_always
static const int showtab            = showtab_never;
// 0 means bottom tab
static const int toptab             = 1;
// 0 means bottom bar
static const int topbar             = 1;

// bar padding
static const int horizpadbar        = 5;
static const int vertpadbar         = 11;

// tab padding
static const int vertpadtab         = 33;
static const int horizpadtabi       = 15;
static const int horizpadtabo       = 15;
static const int scalepreview       = 4;

// 1 means enable, 0 is off
static const int tag_preview        = 1;
// 0 means use SchemeSel for selected non vacant tag
static const int colorfultag        = 0;

static const char *fonts[]          = { "JetBrainsMono Nerd Font:style:medium:size=10",
                                        "Material Design Icons Desktop:size=10" };

// theme
static const char black[]       = "#1E1D2D";
static const char gray2[]       = "#282737"; // unfocused window border
static const char gray3[]       = "#585767";
static const char gray4[]       = "#282737";
static const char blue[]        = "#96CDFB";  // focused window border
static const char green[]       = "#ABE9B3";
static const char red[]         = "#F28FAD";
static const char orange[]      = "#F8BD96";
static const char yellow[]      = "#FAE3B0";
static const char pink[]        = "#d5aeea";
static const char col_borderbar[]  = "#1E1D2D"; // inner border

static const char *colors[][3]      = {
    /*                     fg       bg      border */
    [SchemeNorm]       = { gray3,   black,  gray2 },
    [SchemeSel]        = { blue,    black,  blue  },
    [TabSel]           = { blue,    gray2,  black },
    [TabNorm]          = { gray3,   black,  black },
    [SchemeTag]        = { gray3,   black,  black },
    [SchemeLayout]     = { green,   black,  black },
    [SchemeBtnPrev]    = { green,   black,  black },
    [SchemeBtnNext]    = { yellow,  black,  black },
    [SchemeBtnClose]   = { red,     black,  black },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const char* eww[] = { "echo", "hello", NULL };

static const Launcher launchers[] = {
    /* command     name to display */
    { eww,         "" },
};

// color schemes array for colorful tags
static const int tagschemes[] = {
  SchemeTag,
};

static const unsigned int ulinepad      = 5; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke   = 2; /* thickness / height of the underline */
static const unsigned int ulinevoffset  = 0; /* how far above the bottom of the bar the line should appear */
static const int ulineall               = 0; /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     iscentered   isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            0,           1,           -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,       0,           0,           -1 },
    /* { "eww",      NULL,       NULL,       0,            0,           1,           -1 }, */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "functions.h"


static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "[M]",      monocle },
    { "[@]",      spiral },
    { "[\\]",     dwindle },
    { "H[]",      deck },
    { "TTT",      bstack },
    { "===",      bstackhoriz },
    { "HHH",      grid },
    { "###",      nrowgrid },
    { "---",      horizgrid },
    { ":::",      gaplessgrid },
    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */

static Key keys[] = {
    /* modifier                         key         function        argument */

    // brightness and audio
    {0, XF86XK_AudioMute,          spawn, SHCMD("amixer set Master 1+ toggle")},
    {0, XF86XK_AudioRaiseVolume,   spawn, SHCMD("amixer -q set Master 10%+")},
    {0, XF86XK_AudioLowerVolume,   spawn, SHCMD("amixer -q set Master 10%-")},
    {0, XF86XK_MonBrightnessDown,  spawn, SHCMD("xbacklight -dec 5")},
    {0, XF86XK_MonBrightnessUp,    spawn, SHCMD("xbacklight -inc 5")},

    // screenshot fullscreen and cropped
    {0,                    XK_Print,   spawn, SHCMD("flameshot gui")},


    { MODKEY,              XK_i,       spawn,  SHCMD("rofi -show drun") },
    { MODKEY,              XK_Return,  spawn,  SHCMD("kitty")},
    { MODKEY,              XK_m,       spawn,  SHCMD("kitty sh -c ncmpcpp")},
    { MODKEY,              XK_e,       spawn,  SHCMD("kitty sh -c ranger")},
    { MODKEY,              XK_w,       spawn,  SHCMD("brave")},
    { MODKEY|ShiftMask,    XK_w,       spawn,  SHCMD("kitty sh -c nmtui")},
    { MODKEY,              XK_n,       spawn,  SHCMD("kitty sh -c nmtui")},
    { MODKEY,              XK_F1,      spawn,  SHCMD("kitty sh -c pulsemixer")},
    { MODKEY,              XK_r,       spawn,  SHCMD("kitty sh -c htop")},


    // toggle stuff
    { MODKEY,              XK_b,       togglebar,      {0} },
    { MODKEY|ControlMask,  XK_t,       togglegaps,     {0} },
    { MODKEY,              XK_f,       togglefloating, {0} },
    { MODKEY|ShiftMask,    XK_f,    togglefullscr,  {0} },

    { MODKEY|ControlMask,  XK_w,       tabmode,        { -1 } },
    { MODKEY,              XK_j,       focusstack,     {.i = +1 } },
    { MODKEY,              XK_k,       focusstack,     {.i = -1 } },
    { MODKEY,              XK_equal,   incnmaster,     {.i = +1 } },
    { MODKEY,              XK_plus,    incnmaster,     {.i = -1 } },

    // change m,cfact sizes
    { MODKEY,              XK_h,       setmfact,       {.f = -0.01} },
    { MODKEY,              XK_l,       setmfact,       {.f = +0.01} },
    { MODKEY|ShiftMask,    XK_h,       setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,    XK_l,       setcfact,       {.f = -0.25} },
    { MODKEY|ShiftMask,    XK_o,       setcfact,       {.f =  0.00} },


    { MODKEY|ShiftMask,    XK_j,       movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,    XK_k,       movestack,      {.i = -1 } },
    { MODKEY|ShiftMask,    XK_Return,  zoom,           {0} },
    { MODKEY,              XK_Tab,     view,           {0} },

    // overall gaps
    // { MODKEY|ControlMask,  XK_i,       incrgaps,       {.i = +1 } },
    // { MODKEY|ControlMask,  XK_d,       incrgaps,       {.i = -1 } },
    // inner gaps
    // { MODKEY|ShiftMask,                 XK_i,       incrigaps,      {.i = +1 } },
    // { MODKEY|ControlMask|ShiftMask,     XK_i,       incrigaps,      {.i = -1 } },

    // outer gaps
    // { MODKEY|ControlMask,               XK_o,       incrogaps,      {.i = +1 } },
    // { MODKEY|ControlMask|ShiftMask,     XK_o,       incrogaps,      {.i = -1 } },

    // inner+outer hori, vert gaps
    // { MODKEY|ControlMask,               XK_6,       incrihgaps,     {.i = +1 } },
    // { MODKEY|ControlMask|ShiftMask,     XK_6,       incrihgaps,     {.i = -1 } },
    // { MODKEY|ControlMask,               XK_7,       incrivgaps,     {.i = +1 } },
    // { MODKEY|ControlMask|ShiftMask,     XK_7,       incrivgaps,     {.i = -1 } },
    // { MODKEY|ControlMask,               XK_8,       incrohgaps,     {.i = +1 } },
    // { MODKEY|ControlMask|ShiftMask,     XK_8,       incrohgaps,     {.i = -1 } },
    // { MODKEY|ControlMask,               XK_9,       incrovgaps,     {.i = +1 } },
    // { MODKEY|ControlMask|ShiftMask,     XK_9,       incrovgaps,     {.i = -1 } },

    // { MODKEY|ControlMask|ShiftMask,     XK_d,       defaultgaps,    {0} },


    // layout
    { MODKEY|ControlMask,               XK_space,   setlayout,      {.v = &layouts[0]} },
    { MODKEY,                           XK_space,   cyclelayout,    {.i = -1 } },
    { MODKEY|ShiftMask,                 XK_space,   cyclelayout,    {.i = +1 } },
    { MODKEY,                           XK_0,       view,           {.ui = ~0 } },
    { MODKEY,                           XK_comma,   focusmon,       {.i = -1 } },
    { MODKEY,                           XK_period,  focusmon,       {.i = +1 } },

    // change border size
    { MODKEY|ShiftMask,                 XK_minus,   setborderpx,    {.i = -1 } },
    { MODKEY|ShiftMask,                 XK_p,       setborderpx,    {.i = +1 } },
    { MODKEY|ShiftMask,                 XK_w,       setborderpx,    {.i = borderpx} },

    // kill dwm
    { MODKEY|ShiftMask,                 XK_q,       spawn,        SHCMD("killall bar.sh dwm") },

    // kill window
    { MODKEY,                           XK_q,       killclient,     {0} },

    // restart
    { MODKEY|ShiftMask,                 XK_r,       restart,           {0} },

    // hide & restore windows
    { MODKEY,                           XK_m,       hidewin,        {0} },
    { MODKEY|ShiftMask,                 XK_m,       restorewin,     {0} },

    TAGKEYS(XK_1,0)
    TAGKEYS(XK_2,1)
    TAGKEYS(XK_3,2)
    TAGKEYS(XK_4,3)
    TAGKEYS(XK_5,4)
    TAGKEYS(XK_6,5)
    TAGKEYS(XK_7,6)
    TAGKEYS(XK_8,7)
    TAGKEYS(XK_9,8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          SHCMD("st") },

    /* Keep movemouse? */
    /* { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} }, */

    /* placemouse options, choose which feels more natural:
    *    0 - tiled position is relative to mouse cursor
    *    1 - tiled position is relative to window center
    *    2 - mouse pointer warps to window center
    *
    * The moveorplace uses movemouse or placemouse depending on the floating state
    * of the selected client. Set up individual keybindings for the two if you want
    * to control these separately (i.e. to retain the feature to move a tiled window
    * into a floating position).
    */
    { ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,         ControlMask,    Button1,        dragmfact,      {0} },
    { ClkClientWin,         ControlMask,    Button3,        dragcfact,      {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTabPrev,           0,              Button1,        movestack,      { .i = -1 } },
    { ClkTabNext,           0,              Button1,        movestack,      { .i = +1 } },
    { ClkTabClose,          0,              Button1,        killclient,     {0} },
};
