#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include "sync.h"

/*******************************************/
/* TEST VALUES                             */
/*******************************************/

int num_thread;
int num_iter;
int num_workOut;
int num_workIn;
int num_ID;
int num_custom;

long long count_out = 0;
long long count_in = 0;

/*******************************************/
/* LOCKS                                   */
/*******************************************/

pthread_mutex_t lock_mutex;
pthread_spinlock_t lock_spin;
my_spinlock_t lock_my_spin_TAS;
my_spinlock_t lock_my_spin_TTAS;
my_mutex_t lock_my_mutex;
my_queuelock_t lock_my_queue;

/*******************************************/
/* TIMER STRUCTS & FUNCTIONS               */
/*******************************************/

struct timespec start;
struct timespec stop;
unsigned long long result; 

unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

