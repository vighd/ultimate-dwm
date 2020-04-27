/* See LICENSE file for copyright and license details. */
#include "fibonacci.c"

/* appearance */
static const char *fonts[] = {
  "caneleb:size=9",
  "Waffle:size=12",
};
static const unsigned int borderpx    = 1;        /* border pixel of windows */
static const unsigned int snap        = 32;       /* snap pixel */
static const int showbar              = 1;        /* 0 means no bar */
static const int topbar               = 1;        /* 0 means bottom bar */
static const unsigned int taglinepx   = 1;        /* height of tag underline */
static const unsigned int gappx       = 0;        /* gaps between windows */
static const Bool showtitle           = True;     /* True means Show title in status bar */
static const Bool gaponeclient        = False;    /* Enable gap with one window only */
static const char dmenufont[]         = "caneleb:size=9";
static const char normbgcolor[]       = "#292D3E";
static const char normfgcolor[]       = "#D0D0D0";
static const char selbgcolor[]        = "#82AAFF";
static const char selfgcolor[]        = "#292D3E";
static const char *colors[][3]      = {
  /*                          fg         bg         border   */
  [SchemeNorm]        = { "#D0D0D0",   "#292D3E",   "#3E4452"   }, /* 01 - normal */
  [SchemeSel]         = { "#D0D0D0",   "#32424A",   "#425B80"   }, /* 02 - selected */
  [SchemeTitle]       = { "#C792EA",   "#292D3E",   "#292D3E"   }, /* 03 - active title */
  [SchemeUrgent]      = { "#F07178",   "#292D3E",   "#292D3E"   }, /* 04 - urgent */
  [SchemeOccupied]    = { "#D0D0D0",   "#292D3E",   "#425B80"   }, /* 05 - occupied */
  [SchemeWifi]        = { "#F07178",   "#292D3E",   "#292D3E"   }, /* 06 - Wifi */
  [SchemeBattery]     = { "#DDFFA7",   "#292D3E",   "#292D3E"   }, /* 07 - Battery */
  [SchemeSensor]      = { "#89DDFF",   "#292D3E",   "#292D3E"   }, /* 08 - Sensor */
  [SchemeRpm]         = { "#D0D0D0",   "#292D3E",   "#292D3E"   }, /* 09 - RPM */
  [SchemeProc]        = { "#FFCB6B",   "#292D3E",   "#292D3E"   }, /* 0A - PROC */
  [SchemeLayout]      = { "#F07178",   "#292D3E",   "#292D3E"   }, /* 0B - layout colours */
  [SchemeClock]       = { "#FFDC89",   "#292D3E",   "#292D3E"   }, /* 0C - Clock */
  [SchemeVolume]      = { "#C792EA",   "#292D3E",   "#292D3E"   }, /* 0D - Volume */
  [SchemeFreespc]     = { "#F78C6C",   "#292D3E",   "#292D3E"   }, /* 0E - Free Space */
  [SchemeMem]         = { "#C3E88d",   "#292D3E",   "#292D3E"   }, /* 0F - Mem free */
  [SchemeBrightness]  = { "#9B859D",   "#292D3E",   "#292D3E"   }, /* 10 - Brightness */
  [SchemeTagLine]     = { "#292D3E",   "#292D3E",   "#C792EA"   }, /* 11 - tagline */
  [SchemeKbdLang]     = { "#82AAFF",   "#292D3E",   "#292D3E"   }, /* 12 - kbd lang color */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

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
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 2;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "",        spiral      },    /* first entry is default */
  { "",        tile        },    
  { "",        NULL        },    /* no layout function means floating behavior */
  { "",        monocle     },
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
static const char *dmenucmd[]     = { "dmenu_run",            "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *nmcmd[]        = { "networkmanager_dmenu", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *btmenu[]       = { "btmenu", "::", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *calccmd[]      = { "=", "--", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *webb[]         = { "chromium", NULL };
static const char *termcmd[]      = { "termite", NULL };
static const char *file[]         = { "termite", "-e", "vifmrun", NULL };
static const char *alsamixer[]    = { "termite", "-e", "alsamixer -c 0", NULL };
static const char *upvol[]        = { "/bin/sh", "-c", "amixer set Master 3%+; pkill -f sleep", NULL };
static const char *downvol[]      = { "/bin/sh", "-c", "amixer set Master 3%-; pkill -f sleep", NULL };
static const char *mutevol[]      = { "/bin/sh", "-c", "amixer set Master toggle; pkill -f sleep", NULL };
static const char *mutemic[]      = { "/bin/sh", "-c", "amixer set Capture toggle; pkill -f sleep", NULL };
static const char *brup[]         = { "xbacklight", "-inc", "5", NULL };
static const char *brdown[]       = { "xbacklight", "-dec", "5", NULL };
static const char *scrlock[]      = { "sfplock", NULL };
static const char *screenshot[]   = { "/bin/sh", "-c", "maim -s ~/Pictures/$(date +%s).png", NULL };
static const char *extleft[]      = { "/bin/sh", "-c", "xrandr --output $(xrandr --listmonitors | grep -v '\\*' | grep -Po '[A-Z]+\\d' | head -1) --auto --left-of LVDS1 --output LVDS1 --auto && nitrogen --restore", NULL };
static const char *extright[]     = { "/bin/sh", "-c", "xrandr --output $(xrandr --listmonitors | grep -v '\\*' | grep -Po '[A-Z]+\\d' | head -1) --auto --right-of LVDS1 --output LVDS1 --auto && nitrogen --restore", NULL };
static const char *extabove[]     = { "/bin/sh", "-c", "xrandr --output $(xrandr --listmonitors | grep -v '\\*' | grep -Po '[A-Z]+\\d' | head -1) --auto --above LVDS1 --output LVDS1 --auto", NULL };
static const char *onedisp[]      = { "/bin/sh", "-c", "xrandr --output $(xrandr --listmonitors | grep -v '\\*' | grep -Po '[A-Z]+\\d' | head -1) --off --output LVDS1 --auto && nitrogen --restore", NULL };
static const char *togglemusic[]  = { "/bin/sh", "-c", "mpc toggle; pkill -f sleep", NULL };
static const char *stopmusic[]    = { "/bin/sh", "-c", "mpc stop; pkill -f sleep", NULL };
static const char *nextsong[]     = { "/bin/sh", "-c", "mpc next; pkill -f sleep", NULL };
static const char *prevsong[]     = { "/bin/sh", "-c", "mpc prev; pkill -f sleep", NULL };
static const char *ncmpcpp[]      = { "termite", "-e", "spotifycli", NULL };
static const char *enpass[]       = { "/opt/enpass/Enpass", "showassistant", NULL };
static const char *refbar[]       = { "/bin/sh", "-c", "pkill -f sleep", NULL };
static const char *chlang[]       = { "/bin/sh", "-c", "(setxkbmap -query | grep -q 'layout:     hu' && setxkbmap en_US || setxkbmap hu); pkill -f sleep", NULL };

static Key keys[] = {
  /* modifier                     key          function      argument */
  { MODKEY,                       XK_Shift_L,  spawn,        {.v = refbar       }   },
  { MODKEY,                       XK_p,        spawn,        {.v = dmenucmd     }   },
  { MODKEY|ShiftMask,             XK_n,        spawn,        {.v = nmcmd        }   },
  { MODKEY|ShiftMask,             XK_Return,   spawn,        {.v = termcmd      }   },
  { MODKEY|ShiftMask,             XK_w,        spawn,        {.v = webb         }   },
  { MODKEY|ShiftMask,             XK_f,        spawn,        {.v = file         }   },
  { MODKEY|ShiftMask,             XK_s,        spawn,        {.v = alsamixer    }   },
  { MODKEY|ShiftMask,             XK_e,        spawn,        {.v = enpass       }   },
  { MODKEY|ShiftMask,             XK_7,        spawn,        {.v = calccmd      }   },
  { MODKEY|ShiftMask,             XK_b,        spawn,        {.v = btmenu       }   },
  { 0,                            0x1008ff13,  spawn,        {.v = upvol        }   },
  { 0,                            0x1008ff11,  spawn,        {.v = downvol      }   },
  { 0,                            0x1008ff12,  spawn,        {.v = mutevol      }   },
  { 0,                            0x1008ff2d,  spawn,        {.v = scrlock      }   },
  { 0,                            0xff61,      spawn,        {.v = screenshot   }   },
  { 0,                            0x1008ffb2,  spawn,        {.v = mutemic      }   },
  { 0,                            0x1008ff03,  spawn,        {.v = brdown       }   },
  { 0,                            0x1008ff02,  spawn,        {.v = brup         }   },
  { 0,                            0x1008ff81,  spawn,        {.v = ncmpcpp      }   },
  { 0,                            0x1008ff14,  spawn,        {.v = togglemusic  }   },
  { 0,                            0x1008ff15,  spawn,        {.v = stopmusic    }   },
  { 0,                            0x1008ff17,  spawn,        {.v = nextsong     }   },
  { 0,                            0x1008ff16,  spawn,        {.v = prevsong     }   },
  { WINDOWSKEY,                   XK_Left,     spawn,        {.v = extleft      }   },
  { WINDOWSKEY,                   XK_Right,    spawn,        {.v = extright     }   },
  { WINDOWSKEY,                   XK_Up,       spawn,        {.v = extabove     }   },
  { WINDOWSKEY,                   XK_Down,     spawn,        {.v = onedisp      }   },
  { WINDOWSKEY,                   XK_l,        spawn,        {.v = chlang       }   },
  { MODKEY|ShiftMask,             XK_t,        togglebar,    {0                 }   },
  { MODKEY,                       XK_j,        focusstack,   {.i = +1           }   },
  { MODKEY,                       XK_k,        focusstack,   {.i = -1           }   },
  { MODKEY,                       XK_i,        incnmaster,   {.i = +1           }   },
  { MODKEY,                       XK_d,        incnmaster,   {.i = -1           }   },
  { MODKEY,                       XK_h,        setmfact,     {.f = -0.05        }   },
  { MODKEY,                       XK_l,        setmfact,     {.f = +0.05        }   },
  { MODKEY,                       XK_Return,   zoom,         {0                 }   },
  { MODKEY,                       XK_Tab,      view,         {0                 }   },
  { MODKEY|ShiftMask,             XK_c,        killclient,   {0                 }   },
  { MODKEY,                       XK_s,        setlayout,    {.v = &layouts[0]  }   },
  { MODKEY,                       XK_t,        setlayout,    {.v = &layouts[1]  }   },
  { MODKEY,                       XK_f,        setlayout,    {.v = &layouts[2]  }   },
  { MODKEY,                       XK_m,        setlayout,    {.v = &layouts[3]  }   },
  { MODKEY,                       XK_0,        view,         {.ui = ~0          }   },
  { MODKEY|ShiftMask,             XK_0,        tag,          {.ui = ~0          }   },
  { MODKEY,                       XK_comma,    focusmon,     {.i  = -1          }   },
  { MODKEY,                       XK_period,   focusmon,     {.i  = +1          }   },
  { MODKEY|ShiftMask,             XK_comma,    tagmon,       {.i  = -1          }   },
  { MODKEY|ShiftMask,             XK_period,   tagmon,       {.i  = +1          }   },
  { MODKEY|ShiftMask,             XK_q,        quit,         {0                 }   },
  { MODKEY|ControlMask|ShiftMask, XK_q,        quit,         {1                 }   },
  TAGKEYS(                     XK_1,                                             0)
  TAGKEYS(                     XK_2,                                             1)
  TAGKEYS(                     XK_3,                                             2)
  TAGKEYS(                     XK_4,                                             3)
  TAGKEYS(                     XK_5,                                             4)
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
