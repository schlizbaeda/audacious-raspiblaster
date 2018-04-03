/*
 * tinylock.h
 * Copyright 2013 John Lindgren
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

#ifndef LIBAUDCORE_TINYLOCK_H
#define LIBAUDCORE_TINYLOCK_H

#if 0 /* VALGRIND_FRIENDLY */

#include <pthread.h>

typedef pthread_mutex_t TinyLock;
typedef pthread_rwlock_t TinyRWLock;

#define tiny_lock pthread_mutex_lock
#define tiny_unlock pthread_mutex_unlock
#define tiny_lock_read pthread_rwlock_rdlock
#define tiny_unlock_read pthread_rwlock_unlock
#define tiny_lock_write pthread_rwlock_wrlock
#define tiny_unlock_write pthread_rwlock_unlock

#else /* ! VALGRIND_FRIENDLY */

/* TinyLock is an extremely low-overhead lock object (in terms of speed and
 * memory usage).  It makes no guarantees of fair scheduling, however. */

typedef char TinyLock;

void tiny_lock (TinyLock * lock);
void tiny_unlock (TinyLock * lock);

typedef unsigned short TinyRWLock;

void tiny_lock_read (TinyRWLock * lock);
void tiny_unlock_read (TinyRWLock * lock);
void tiny_lock_write (TinyRWLock * lock);
void tiny_unlock_write (TinyRWLock * lock);

#endif /* ! VALGRIND_FRIENDLY */

#endif /* LIBAUDCORE_TINYLOCK_H */
