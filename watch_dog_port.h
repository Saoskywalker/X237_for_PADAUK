#ifndef _WATCH_DOG_PORT_H
#define _WATCH_DOG_PORT_H

#define MTF_watch_dog_init() wdreset; clkmd.1 = 1 //enable wdt, about 128ms, use ILRC
#define MTF_watch_dog_exit() wdreset; clkmd.1 = 0 //disable wdt
#define MTF_watch_dog_feed() WDRESET //feed WDOG

#define MTF_sys_stop() STOPSYS //进入掉电模式, 只能通过IO变化唤醒, 所有振荡器关闭, LVR无效

#endif
