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


#include <glib/gstdio.h>    /* necessary for printf */
#include <pthread.h>        /* necessary for macros ENTER; LEAVE; etc. due to multi threading */
#include <stdlib.h>         /* necessary for system */
//#include <wiringPi.h>

#include "eject.h"


static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define ENTER pthread_mutex_lock (& mutex)
#define LEAVE pthread_mutex_unlock (& mutex)





EXPORT void aud_drct_eject ()
{
	int ret;
	char cmd[] = "/usr/bin/eject";

    ENTER;
    set_playing_locked (nullptr, false); /* do stop playlist actions before ejecting */
    /* eject */
    ret = system (cmd);
    printf("schlizbäda: exit code of '%s': %d\n", cmd, ret);
    LEAVE;
}


