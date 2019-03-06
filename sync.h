#ifndef _my_SYNC1_H_
#define _my_SYNC1_H_

#define DELAY_MIN 1
#define DELAY_MAX 4096

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include "atomic_ops.h"

/*******************************************/
/* SPINLOCK FUNCTIONS & STRUCT             */
/*******************************************/

struct my_spinlock_struct {
	volatile pthread_t owner;
	volatile unsigned long locked;
	volatile unsigned long unlock;	
};

typedef struct my_spinlock_struct my_spinlock_t;

int my_spinlock_init(my_spinlock_t *lock);
int my_spinlock_destroy(my_spinlock_t *lock);
int my_spinlock_unlock(my_spinlock_t *lock);

int my_spinlock_lockTAS(my_spinlock_t *lock);
int my_spinlock_lockTTAS(my_spinlock_t *lock);
int my_spinlock_trylock(my_spinlock_t *lock);

/*******************************************/
/* MUTEX FUNCTIONS & STRUCT                */
/*******************************************/

struct my_mutex_struct {
	volatile pthread_t owner;
	volatile unsigned long locked;
	volatile unsigned long unlock;
};

typedef struct my_mutex_struct my_mutex_t;

int my_mutex_init(my_mutex_t *lock);
int my_mutex_unlock(my_mutex_t *lock);
int my_mutex_destroy(my_mutex_t *lock);

int my_mutex_lock(my_mutex_t *lock);
int my_mutex_trylock(my_mutex_t *lock);

/*******************************************/
/* QUEUELOCK FUNCTIONS & STRUCT            */
/*******************************************/

struct my_queuelock_struct {
	volatile unsigned long now_serving;
	volatile unsigned long next_ticket;
};

typedef struct my_queuelock_struct my_queuelock_t;

int my_queuelock_init(my_queuelock_t *lock);
int my_queuelock_destroy(my_queuelock_t *lock);
int my_queuelock_unlock(my_queuelock_t *lock);

int my_queuelock_lock(my_queuelock_t *lock);
int my_queuelock_trylock(my_queuelock_t *lock);


#endif
