/*
 * eject.cc
 * Copyright 2018 schlizbäda
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    provided with the distribution.
 *
 * This software is provided "as is" and without any warranty, express or
 * implied. In no event shall the authors be liable for any damages arising from
 * the use of this software.
 */

#define RASPBERRYPI

#include <glib/gstdio.h>    /* necessary for printf */
#include <pthread.h>        /* necessary for macros ENTER; LEAVE; etc. due to multi threading */
#include <stdlib.h>         /* necessary for system */
#ifdef RASPBERRYPI
//#include <pigpio.h>       /* wrong way, slows down the PWM playback speed */
//#include <wiringPi.h>     /* unnecessary due to the usage of the "gpio" shell command which is part of the wiringPi project */
#include <cstring>
#define GPIO_LOCK_EJECT 4
#endif

#include "eject.h"


static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define ENTER pthread_mutex_lock (& mutex)
#define LEAVE pthread_mutex_unlock (& mutex)


EXPORT void aud_drct_eject ()
{
	int ret;
	char cmd [] = "/usr/bin/eject";

    ENTER;
    set_playing_locked (nullptr, false); /* do stop playlist actions before ejecting */
    /* eject */
    ret = system (cmd);
    printf("schlizbäda: exit code of '%s': %d\n", cmd, ret);
    LEAVE;
}



/* Raspberry Pi
 * especially on schlizbäda's raspiblaster
 *
 * Functions to lock and unlock the CDROM drive's eject push button via GPIO 4
 */

EXPORT int lock_eject_pushbutton ()
{
    int ret;
#ifdef RASPBERRYPI
    ret = lock_eject_raspberrypi (true);
#else
    printf ("schlizbäda: lock_eject_pushbutton\n");
    ret = 0;
#endif
    return ret;
}

EXPORT int unlock_eject_pushbutton ()
{
    int ret;
#ifdef RASPBERRYPI
    ret = lock_eject_raspberrypi (false);
#else
    printf ("schlizbäda: unlock_eject_pushbutton\n");
    ret = 0;
#endif
    return ret;
}


#ifdef RASPBERRYPI
int lock_eject_raspberrypi (bool lock)
{
    int ret {0};

    unsigned level {0};    /* UNLOCK */
    char prefix [] = "un"; /* UNLOCK */
    if (lock)
    {
        level = 1;     /* LOCK: high-level     */
        prefix[0] = 0; /* LOCK: no prefix "un" */
    }

#define WIRINGPI
#ifdef WIRINGPI
    /* better take ye olde (scnr) wiringPi Library instead of PiGPIO */
    static int cnt {0};
    char cmd_mode [32];
    char cmd_write [32];
    char cmd_gpio [4];

    printf("schlizbäda: wiringPi(call %d) lock_eject_raspberry (%slock): GPIO%d=%d\n", cnt, prefix, GPIO_LOCK_EJECT, level);
    sprintf (cmd_gpio, "%d ", GPIO_LOCK_EJECT);
    if (!cnt)
    {
        strcpy (cmd_mode, "gpio -g mode ");
        strcat (cmd_mode, cmd_gpio); /* concatenate the value of GPIO_LOCK_EJECT as string */
        strcat (cmd_mode, "out");
        printf ("            set mode with command:        %s\n", cmd_mode);
        ret = system (cmd_mode);
    }
    if (!ret) /* initialisation of pin GPIO_LOCK_EJECT was successful resp. skipped */
    {
        strcpy (cmd_write, "gpio -g write ");
        strcat (cmd_write, cmd_gpio); /* concatenate the value of GPIO_LOCK_EJECT as string */
        strcat (cmd_write, level ? "1" : "0");
        printf ("            set lock/unlock with command: %s\n", cmd_write);
        ret = system (cmd_write);
    }
    cnt++;
#endif

/* don't take PiGPIO or PiGPIO_IF2 because this massively slows down the playback speed! */
#ifdef PIGPIO
    printf("schlizbäda: SUDO lock_eject_raspberry (%slock): GPIO%d=%d\n", prefix, GPIO_LOCK_EJECT, level);

//    ret = gpioInitialise ();
    if (ret < 0)
    {
        // pigpio initialisation failed.
        printf ("  %slock_eject_raspberrypi: gpioInitialise() failed with error code %d.\n", prefix, ret);
    }
    else
    {
        // pigpio initialised okay.
//        gpioTerminate ();
    }
#endif
#ifdef PIGPIOD_IF2
    printf("schlizbäda: PiGPIOD: lock_eject_raspberry (%slock): GPIO%d=%d\n", prefix, GPIO_LOCK_EJECT, level);

    int gpiohandle = pigpio_start (NULL, NULL);
    if (gpiohandle < 0)
    {
        // pigpio initialisation failed.
        printf ("  %slock_eject_raspberrypi: pigpio_start() failed with error code %d.\n", prefix, gpiohandle);
        ret = gpiohandle;
    }
    else
    {
        // pigpio initialised okay.
        ret = set_mode (gpiohandle, GPIO_LOCK_EJECT, PI_OUTPUT);
        if (0 == ret)
        {
             // set_mode passed
             ret = gpio_write (gpiohandle, GPIO_LOCK_EJECT, level);
             // TODO
        }
        else if (PI_BAD_GPIO == ret)
        {
             printf ("  %slock_eject_pushbutton: pigpio set_mode(gpiohandle=%d, gpio=%d): PI_BAD_GPIO.\n", prefix, gpiohandle, GPIO_LOCK_EJECT);
        }
        else if (PI_BAD_MODE == ret)
        {
             printf ("  %slock_eject_pushbutton: pigpio set_mode(gpiohandle=%d, gpio=%d): PI_BAD_MODE.\n", prefix, gpiohandle, GPIO_LOCK_EJECT);
        }
        else if (PI_NOT_PERMITTED == ret)
        {
             printf ("  %slock_eject_pushbutton: pigpio set_mode(gpiohandle=%d, gpio=%d): PI_NOT_PERMITTED.\n", prefix, gpiohandle, GPIO_LOCK_EJECT);
        }
        else
        {
             printf ("  %slock_eject_pushbutton: pigpio set_mode(gpiohandle=%d, gpio=%d): <unknown error>.\n", prefix, gpiohandle, GPIO_LOCK_EJECT);
        }
        pigpio_stop (gpiohandle);
    }
#endif // SEPPERL
    return ret;
}
#endif
