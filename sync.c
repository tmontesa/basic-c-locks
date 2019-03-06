#define _REENTRANT

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include "sync.h"

/*******************************************/
/* SPINLOCK ROUTINES                       */
/*******************************************/

int my_spinlock_init(my_spinlock_t *lock) {
	lock->owner = -1;
	lock->locked = 0;
	lock->unlock = 0;
	return 0;
}

int my_spinlock_destroy(my_spinlock_t *lock) {
	lock->owner = -1;
	lock->locked = -1;
	lock->unlock = -1;
	return 0;
}

int my_spinlock_unlock(my_spinlock_t *lock) {
	pthread_t this_thread = pthread_self();

	if (pthread_equal(lock->owner, this_thread)) {
		lock->unlock--;

		if (lock->unlock != 1) {
			lock->owner = -1;
			lock->locked = 0;
		}

		return 0;

	} else {
		return -1;
	}	
}

int my_spinlock_lockTAS(my_spinlock_t *lock) {
	pthread_t this_thread = pthread_self();

	if (pthread_equal(lock->owner, this_thread)) {
		lock->unlock++;

	} else {
		while (tas(&lock->locked) == 1) { }
		// Block until lock is unlocked.
		lock->owner = this_thread;
		lock->unlock = 1;
	}

	return 0;
}


int my_spinlock_lockTTAS(my_spinlock_t *lock) {
	pthread_t this_thread = pthread_self();

	if (pthread_equal(lock->owner, this_thread)) {
		lock->unlock++;
	} else {
		while (1) {
			while (lock->locked == 1) { }
			// Block until lock "looks" unlocked, then attempt to unlock.	
			if (tas(&lock->locked) == 0) {
				lock->owner = this_thread;
				lock->unlock = 1;
				return 0;
			} 
		}
	}

	return 0;		
}

int my_spinlock_trylock(my_spinlock_t *lock) {
	pthread_t this_thread = pthread_self();

	if (pthread_equal(lock->owner, this_thread)) {
		lock->unlock++;

	} else {
		// Lock lock if lock is unlocked.
		if (tas(&lock->locked) == 0) { 
			lock->owner = this_thread;
			lock->unlock = 1;

		} else {
			return -1;
		}
	}

	return 0;	
}

/*******************************************/
/* MUTEX ROUTINES                          */
/*******************************************/
// Exponential backoff formula from Chapter 5 lecture slides! 
// http://199.60.17.135/cmpt-300/wp-content/uploads/2015/12/chapter5.pdf, slide 117.

int my_mutex_init(my_mutex_t *lock) {
	lock->owner = -1;
	lock->locked = 0;
	lock->unlock = 0;
	return 0;
}

int my_mutex_destroy(my_mutex_t *lock) {
	lock->owner = -1;
	lock->locked = -1;
	lock->unlock = -1;
	return 0;
}

int my_mutex_unlock(my_mutex_t *lock) {
	pthread_t this_thread = pthread_self();

	if (pthread_equal(lock->owner, this_thread)) {
		lock->unlock--;

		if (lock->unlock == 0) {
			lock->owner = -1;
			lock->locked = 0;

		} else {
			return -1;
		}
	}

	return 0;
}

int my_mutex_lock(my_mutex_t *lock) {	// TTAS
	pthread_t this_thread = pthread_self();

	if (pthread_equal(lock->owner, this_thread)) {
		lock->unlock++;

	} else {
		useconds_t delay = DELAY_MIN;
		while (1) {

			while (lock->locked == 1) { }
			// Block until lock looks unlocked, then attempt to unlock.

			if (tas(&lock->locked) == 0) {
				lock->owner = this_thread;
				lock->unlock = 1;
				return 0;

			} else {
				usleep(rand() % delay);
				if (delay < DELAY_MAX) {
					delay *= 2;
				}
			}
		}
	}

	return 0;
}

int my_mutex_trylock(my_mutex_t *lock) {
	pthread_t this_thread = pthread_self();

	if (pthread_equal(lock->owner, this_thread)) {
		lock->unlock++;

	} else {
		// Lock lock if lock is unlocked.
		if (tas(&lock->locked) == 0) { 
			lock->owner = this_thread;
			lock->unlock = 1;

		} else {
			return -1;
		}
	}

	return 0;
}

/*******************************************/
/* QUEUELOCK ROUTINES                      */
/*******************************************/
// Base code taken from Wikipedia!
// https://en.wikipedia.org/wiki/Ticket_lock.

int my_queuelock_init(my_queuelock_t *lock) {
printf(".");
	lock->now_serving = 0;
	lock->next_ticket = 0;
	return 0;
}

int my_queuelock_destroy(my_queuelock_t *lock) {
	lock->now_serving = -1;
	lock->next_ticket = -1;
	return 0;
}

int my_queuelock_unlock(my_queuelock_t *lock) {
		lock->now_serving++;
		return 0;
}

int my_queuelock_lock(my_queuelock_t *lock) {
	unsigned long this_ticket = cas(&lock->next_ticket, lock->next_ticket, (lock->next_ticket + 1));
	
	while (lock->now_serving != this_ticket) { }
	// Block until owner's ticket comes up.
	return 0;
}

int my_queuelock_trylock(my_queuelock_t *lock) {
	unsigned long this_ticket = cas(&lock->next_ticket, lock->next_ticket, (lock->next_ticket + 1));

	// Check if owner's ticket comes up.
	if (lock->now_serving == this_ticket) {
		return 0;
	} else {
		return -1;
	}

}

