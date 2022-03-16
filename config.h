/* See LICENSE file for copyright and license details. */
#include "fibonacci.c"

/* appearance */
static const char *fonts[] = {"UbuntuMonoNerdFont:size=10:antialias=true"};
static const unsigned int borderpx    = 1;        /* border pixel of windows */
static const unsigned int snap        = 32;       /* snap pixel */
static const int showbar              = 1;        /* 0 means no bar */
static const int topbar               = 1;        /* 0 means bottom bar */
static const unsigned int taglinepx   = 2;        /* height of tag underline */
static const unsigned int gappx       = 0;        /* gaps between windows */
static const Bool showtitle           = True;     /* True means Show title in status bar */
static const Bool gaponeclient        = False;    /* Enable gap with one window only */
static const int lockfullscreen       = 1;        /* 1 will force focus on the fullscreen window */
static const char dmenufont[]         = "UbuntuMonoNerdFont:size=10:antialias=true";
static const char normbgcolor[]       = "#242b38";
static const char normfgcolor[]       = "#ABB2BF";
static const char selbgcolor[]        = "#4fa6ed";
static const char selfgcolor[]        = "#242b38";
static const char *colors[][3]      = {
  /*                          fg         bg         border   */
  [SchemeNorm]        = { "#666e83",   "#242b38",   "#425B80"   },
  [SchemeSel]         = { "#242b38",   "#4fa6ed",   "#425B80"   },
  [SchemeTitle]       = { "#bf68d9",   "#242b38",   "#242b38"   },
  [SchemeUrgent]      = { "#e55561",   "#242b38",   "#242b38"   },
  [SchemeOccupied]    = { "#666e83",   "#242b38",   "#425B80"   },
  [SchemeLayout]      = { "#e2b86b",   "#242b38",   "#242b38"   },
  [SchemeTagLine]     = { "#bf68d9",   "#242b38",   "#bf68d9"   },
};

static const char *tagsel[][2] = {
  { "#48b0bd", "#242b38" },
  { "#4fa6ed", "#242b38" },
  { "#8ebd6b", "#242b38" },
  { "#e2b86b", "#242b38" },
  { "#bf68d9", "#242b38" },
  { "#e55561", "#242b38" },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "" };

static const Rule rules[] = {
  /* xprop(1):
   *	WM_CLASS(STRING) = instance, class
   *	WM_NAME(STRING) = title
   */
  /* class                        instance                      title                           tags mask     iscentered    isfloating   monitor */
  { "Gimp",                       NULL,                         NULL,                           0,            0,            1,           -1 },
  { "Nitrogen",                   NULL,                         NULL,                           0,            1,            1,           -1 },
  { "Steam",                      NULL,                         NULL,                           0,            0,            1,           -1 },
  { "Feh",                        NULL,                         NULL,                           0,            0,            1,           -1 },
  { "Libreoffice",                NULL,                         NULL,                           0,            0,            1,           -1 },
  { "Enpass",                     NULL,                         "Enpass Assistant",             0,            1,            1,           -1 },
  { "mpv",                        NULL,                         NULL,                           0,            0,            1,           -1 },
  { "Microsoft Teams - Preview",  "microsoft teams - preview",  "Microsoft Teams Notification", 0,            0,            1,           -1 },
  { "Lxrandr",                    "lxrandr",                    "Display Settings",             0,            1,            1,           -1 },
  { "wow.exe",                    "wow.exe",                    "World of Warcraft",            0,            0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 2;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "",        spiral      },    /* first entry is default */
  { "",        tile        },    
  { "",        NULL        },    /* no layout function means floating behavior */
  { "",        monocle     },
};

/* key definitions */
#define MODKEY Mod1Mask
#define WINDOWSKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]       = { "dmenu_run",    "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *passmenucmd[]    = { "passm",        "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *passmenuotpcmd[] = { "passm", "otp", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *btmenu[]         = { "btmenu", "::", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };

static Key keys[] = {
  /* modifier                     key             function      argument */
  { MODKEY,                       XK_p,           spawn,        {.v = dmenucmd        }                                                                                                   },
  { MODKEY|ShiftMask,             XK_b,           spawn,        {.v = btmenu          }                                                                                                   },
  { MODKEY,                       XK_i,           spawn,        {.v = passmenucmd     }                                                                                                   },
  { MODKEY,                       XK_o,           spawn,        {.v = passmenuotpcmd  }                                                                                                   },
  { MODKEY|ShiftMask,             XK_n,           spawn,        SHCMD("alacritty -e iwctl")                                                                                               },
  { MODKEY|ShiftMask,             XK_Return,      spawn,        SHCMD("alacritty")                                                                                                        },
  { MODKEY|ShiftMask,             XK_w,           spawn,        SHCMD("chromium")                                                                                                         },
  { MODKEY|ShiftMask,             XK_f,           spawn,        SHCMD("alacritty -e vifmrun")                                                                                             },
  { MODKEY|ShiftMask,             XK_s,           spawn,        SHCMD("alacritty -e pulsemixer")                                                                                          },
  { 0,                            0x1008ff13,     spawn,        SHCMD("pulsemixer --max-volume 100 --change-volume +5")                                                                   },
  { 0,                            0x1008ff11,     spawn,        SHCMD("pulsemixer --max-volume 100 --change-volume -5")                                                                   },
  { 0,                            0x1008ff12,     spawn,        SHCMD("pulsemixer --max-volume 100 --toggle-mute")                                                                        },
  { 0,                            0x1008ffb2,     spawn,        SHCMD("pulsemixer --max-volume 100 --id source-1 --toggle-mute")                                                          },
  { 0,                            0xff61,         spawn,        SHCMD("maim -s ~/Pictures/$(date +%s).png")                                                                               },
  { 0,                            0x1008ff03,     spawn,        SHCMD("xbacklight -dec 5")                                                                                                },
  { 0,                            0x1008ff02,     spawn,        SHCMD("xbacklight -inc 5")                                                                                                },
  { 0,                            0x1008ff14,     spawn,        SHCMD("mocp --toggle-pause")                                                                                              },
  { 0,                            0x1008ff15,     spawn,        SHCMD("mocp --stop")                                                                                                      },
  { 0,                            0x1008ff17,     spawn,        SHCMD("mocp --next")                                                                                                      },
  { 0,                            0x1008ff16,     spawn,        SHCMD("mocp --previous")                                                                                                  },
  { MODKEY|ShiftMask,             XK_m,           spawn,        SHCMD("alacritty -e mocp && mocp -x")                                                                                     },
  { WINDOWSKEY,                   XK_p,           spawn,        SHCMD("xlayoutdisplay -p HDMI2 -d 96 && nitrogen --restore")                                                              },
  { WINDOWSKEY,                   XK_Left,        spawn,        SHCMD("/home/vighd/.screenlayout/Home.sh && nitrogen --restore")                                                          },
  { WINDOWSKEY,                   XK_g,           spawn,        SHCMD("grep performance /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor > /dev/null && sudo tlp bat || sudo tlp ac")},
  { WINDOWSKEY,                   XK_l,           spawn,        SHCMD("(setxkbmap -query | grep -q 'layout:     hu' && setxkbmap en_US || setxkbmap hu)")                                 },
  { MODKEY,                       XK_Shift_L,     spawn,        SHCMD("dwm-statusbar kill")                                                                                               },
  { MODKEY,                       XK_g,           togglefullscr,{0                    }                                                                                                   },
  { MODKEY|ShiftMask,             XK_t,           togglebar,    {0                    }                                                                                                   },
  { MODKEY,                       XK_j,           focusstack,   {.i = +1              }                                                                                                   },
  { MODKEY,                       XK_k,           focusstack,   {.i = -1              }                                                                                                   },
  { MODKEY,                       XK_i,           incnmaster,   {.i = +1              }                                                                                                   },
  { MODKEY,                       XK_d,           incnmaster,   {.i = -1              }                                                                                                   },
  { MODKEY,                       XK_h,           setmfact,     {.f = -0.05           }                                                                                                   },
  { MODKEY,                       XK_l,           setmfact,     {.f = +0.05           }                                                                                                   },
  { MODKEY,                       XK_Return,      zoom,         {0                    }                                                                                                   },
  { MODKEY,                       XK_Tab,         view,         {0                    }                                                                                                   },
  { MODKEY|ShiftMask,             XK_c,           killclient,   {0                    }                                                                                                   },
  { MODKEY,                       XK_s,           setlayout,    {.v = &layouts[0]     }                                                                                                   },
  { MODKEY,                       XK_t,           setlayout,    {.v = &layouts[1]     }                                                                                                   },
  { MODKEY,                       XK_f,           setlayout,    {.v = &layouts[2]     }                                                                                                   },
  { MODKEY,                       XK_m,           setlayout,    {.v = &layouts[3]     }                                                                                                   },
  { MODKEY,                       XK_0,           view,         {.ui = ~0             }                                                                                                   },
  { MODKEY|ShiftMask,             XK_0,           tag,          {.ui = ~0             }                                                                                                   },
  { MODKEY,                       XK_comma,       focusmon,     {.i  = -1             }                                                                                                   },
  { MODKEY,                       XK_period,      focusmon,     {.i  = +1             }                                                                                                   },
  { MODKEY|ShiftMask,             XK_comma,       tagmon,       {.i  = -1             }                                                                                                   },
  { MODKEY|ShiftMask,             XK_period,      tagmon,       {.i  = +1             }                                                                                                   },
  { MODKEY|ShiftMask,             XK_q,           quit,         {0                    }                                                                                                   },
  { MODKEY|ControlMask|ShiftMask, XK_q,           quit,         {1                    }                                                                                                   },
  TAGKEYS(                        XK_1,                         0                                                                                                                         )
  TAGKEYS(                        XK_2,                         1                                                                                                                         )
  TAGKEYS(                        XK_3,                         2                                                                                                                         )
  TAGKEYS(                        XK_4,                         3                                                                                                                         )
  TAGKEYS(                        XK_5,                         4                                                                                                                         )
  TAGKEYS(                        XK_6,                         5                                                                                                                         )
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0}    },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0}    },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0}    },
  { ClkTagBar,            0,              Button1,        view,           {0}    },
  { ClkTagBar,            0,              Button3,        toggleview,     {0}    },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0}    },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0}    },
};
