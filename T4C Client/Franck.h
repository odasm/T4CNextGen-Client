//#define GRBX_NOMOUSE
#define GRBX_MOUSE
//#define GRBX_NOVK
#define GRBX_VK
//#define GRBX_NOCHAR
#define GRBX_CHAR

#ifndef GRBX_NOMOUSE
#ifndef GRBX_MOUSE
#error You must define GRBX_MOUSE to use the Mouse or GRBX_NOMOUSE to don't it.
#endif
#endif

#ifndef GRBX_NOVK
#ifndef GRBX_VK
#error You must define GRBX_VK to use the Virtual Key or GRBX_NOVK to don't use it.
#endif
#endif

#ifndef GRBX_NOCHAR
#ifndef GRBX_CHAR
#error You must define GRBX_CHAR to use the Character or GRBX_NOCHAR to don't use it.
#endif
#endif

#ifndef GRBX_NOMOUSE
#ifndef GRBX_MOUSE
#error You must only define GRBX_MOUSE or GRBX_NOMOUSE.
#endif
#endif

#ifndef GRBX_NOVK
#ifndef GRBX_VK
#error You must only define GRBX_VK or GRBX_NOVK.
#endif
#endif

#ifndef GRBX_NOCHAR
#ifndef GRBX_CHAR
#error You must only define GRBX_CHAR or GRBX_NOCHAR.
#endif
#endif

//#error All those error was just for fun!