#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "main.h"

/*******************************************/
/* INSTRUCTION PRINT                       */
/*******************************************/

void print_commands() {
	printf("+==================================================+\n");
	printf("| Assignment 3 - Locks & Synchronization           |\n");
	printf("+==================================================+\n");
	printf("| Timothy James Montesa - 301261623                |\n");
	printf("| tmontesa@sfu.ca                                  |\n");
	printf("| for CMPT 300 - E100                              |\n");
	printf("+==================================================+\n\n");
	usleep(250000);

	printf("+==========================+=======================+\n");
	printf("| List of commands:        | List of test IDs:     |\n");
	printf("+==========================+=======================+\n");
	printf("| -d = test ID number      | 0 = all               |\n");
	printf("| -i = iteration number    | 1 = Mutex pthread     |\n");
	printf("| -t = thread number       | 2 = SpinLock pthread  |\n");
	printf("| -o = operations outside  | 3 = mySpinLock TAS    |\n");
	printf("|      crtical section     | 4 = mySpinLock TTAS   |\n");
	printf("| -c = operations inside   | 5 = myMutex (TAS)     |\n");
	printf("|      crtical section     | 6 = myQueueLock       |\n");
	printf("+==========================+=======================+\n\n");
	usleep(250000);

	printf("+==================================================+\n");
	printf("| Custom Tests (set -u to 1)                       |\n");
	printf("+==================================================+\n");
	printf("|  0 = all tests except for myQueueLock            |\n");
	printf("+--------------------------------------------------+\n");
	printf("|  1 = Mutex with exponential workIn               |\n");
	printf("|  2 = Spin with exponential workIn                |\n");
	printf("|  3 = mySpin TAS & TTAS with exponential workIn   |\n");
	printf("|  4 = myMutex with exponential workIn             |\n");
	printf("|  5 = myQueue with exponential workIn             |\n");
	printf("+--------------------------------------------------+\n");
	printf("|  6 = Mutex with exponential workOut              |\n");
	printf("|  7 = Spin with exponential workOut               |\n");
	printf("|  8 = mySpin TAS & TTAS with exponential workOut  |\n");
	printf("|  9 = myMutex with exponential workOut            |\n");
	printf("| 10 = myQueue with exponential workOut            |\n");
	printf("+--------------------------------------------------+\n");
	printf("| 11 = Mutex with exponential iteration            |\n");
	printf("| 12 = Spin with exponential iteration             |\n");
	printf("| 13 = mySpin TAS & TTAS with exponential iter.    |\n");
	printf("| 14 = myMutex with exponential iteration          |\n");
	printf("| 15 = myQueue with exponential iteration          |\n");
	printf("+--------------------------------------------------+\n");
	printf("| 21 = all tests with 0 workIn & workOut           |\n");
	printf("| 22 = all tests with 0 workIn then 0 workOut      |\n");
	printf("| 23 = all tests with 1/4/8/9 threads              |\n");
	printf("+==================================================+\n\n");
}

/*******************************************/
/* SETS DEFAULT VALUES                     */
/*******************************************/

void set_default_values() {
	num_thread  = 4;
	num_iter    = 1000000;
	num_ID      = 0;
	num_workOut = 0;
	num_workIn  = 1;
	num_custom = 0;
}

/*******************************************/
/* THREAD FUNCTIONS                        */
/*******************************************/

void *test_mutex() {
	int i = 0; 
	int out, in = 0;
	count_out, count_in = 0;
	
	for (i = 0; i < num_iter; i++) {
		for (out = 0; out < num_workOut; out++) {
			count_out++;
		}

		pthread_mutex_lock(&lock_mutex);
		for (in = 0; in < num_workIn; in++) {
			count_in++;
		}
		pthread_mutex_unlock(&lock_mutex);
	}
}

void *test_spin() {
	int i = 0; 
	int out, in = 0;
	count_out, count_in = 0;
	
	for (i = 0; i < num_iter; i++) {
		for (out = 0; out < num_workOut; out++) {
			count_out++;
		}

		pthread_spin_lock(&lock_spin);
		for (in = 0; in < num_workIn; in++) {
			count_in++;

		}
		pthread_spin_unlock(&lock_spin);
	}
}

void *test_my_spin_TAS() {
	int i = 0; 
	int out, in = 0;
	count_out = 0;
	
	for (i = 0; i < num_iter; i++) {
		for (out = 0; out < num_workOut; out++) {
			count_out++;
		}

		my_spinlock_lockTAS(&lock_my_spin_TAS);
		for (in = 0; in < num_workIn; in++) {
			count_in++;
		}
		my_spinlock_unlock(&lock_my_spin_TAS);
	}
}

void *test_my_spin_TTAS() {
	int i = 0; 
	int out, in = 0;
	count_out, count_in = 0;
	
	for (i = 0; i < num_iter; i++) {
		for (out = 0; out < num_workOut; out++) {
			count_out++;
		}

		my_spinlock_lockTTAS(&lock_my_spin_TTAS);
		for (in = 0; in < num_workIn; in++) {
			count_in++;
		}
		my_spinlock_unlock(&lock_my_spin_TTAS);
	}
}

void *test_my_mutex() {
	int i = 0; 
	int out, in = 0;
	count_out, count_in = 0;
	
	for (i = 0; i < num_iter; i++) {
		for (out = 0; out < num_workOut; out++) {
			count_out++;
		}

		my_mutex_lock(&lock_my_mutex);
		for (in = 0; in < num_workIn; in++) {
			count_in++;
		}
		my_mutex_unlock(&lock_my_mutex);
	}
}

void *test_my_queue() {
	int i = 0; 
	int out, in = 0;
	count_out, count_in = 0;
	
	for (i = 0; i < num_iter; i++) {
		for (out = 0; out < num_workOut; out++) {
			count_out++;
		}

		my_queuelock_lock(&lock_my_queue);
		for (in = 0; in < num_workIn; in++) {
			count_in++;
		}
		my_queuelock_unlock(&lock_my_queue);
	}
}

/*******************************************/
/* TEST FUNCTIONS                          */
/*******************************************/

int run_mutex() {
	int t, err = 0;
	count_in = 0;
	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * num_thread);

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (t = 0; t < num_thread; t++) {
		err = pthread_create(threads + t, NULL, &test_mutex, NULL);
		if (err != 0) {
			printf("Error: Thread creation failed: %d.\n", err);
			return -1;
		}
	}

	for (t = 0; t < num_thread; t++) {
		pthread_join(threads[t], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &stop);
	pthread_mutex_destroy(&lock_mutex);

	result = timespecDiff(&stop,&start);
	printf("RESULTS - Mutex:\n");	
        printf("Total Count: %lld\n", count_in);
        printf("Time (ms): %llu\n\n",result/1000000);

        return 0;
}

int run_spin() {

	int t, err = 0;
	count_in = 0;
	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * num_thread);
	pthread_spin_init(&lock_spin, PTHREAD_PROCESS_PRIVATE);
	clock_gettime(CLOCK_MONOTONIC, &start);

	for (t = 0; t < num_thread; t++) {
		err = pthread_create(threads + t, NULL, &test_spin, NULL);
		if (err != 0) {
			printf("Error: Thread creation failed: %d.\n", err);
			return -1;
		}
	}

	for (t = 0; t < num_thread; t++) {
		pthread_join(threads[t], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &stop);
	pthread_spin_destroy(&lock_spin);

	result = timespecDiff(&stop,&start);
	printf("RESULTS - Spinlock:\n");
        printf("Total Count: %lld\n", count_in);
        printf("Time (ms): %llu\n\n",result/1000000);
        return 0;
}

int run_my_spin_TAS() {
	int t, err = 0;
	count_in = 0;
	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * num_thread);
	my_spinlock_init(&lock_my_spin_TAS);

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (t = 0; t < num_thread; t++) {
		err = pthread_create(threads + t, NULL, &test_my_spin_TAS, NULL);
		if (err != 0) {
			printf("Error: Thread creation failed: %d.\n", err);
			return -1;
		}
	}

	for (t = 0; t < num_thread; t++) {
		pthread_join(threads[t], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &stop);
	my_spinlock_destroy(&lock_my_spin_TAS);

	result = timespecDiff(&stop,&start);
	printf("RESULTS - My Spinlock TAS:\n");
        printf("Total Count: %lld\n", count_in);
        printf("Time (ms): %llu\n\n",result/1000000);
}

int run_my_spin_TTAS() {
	int t, err = 0;
	count_in = 0;
	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * num_thread);
	my_spinlock_init(&lock_my_spin_TTAS);

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (t = 0; t < num_thread; t++) {
		err = pthread_create(threads + t, NULL, &test_my_spin_TTAS, NULL);
		if (err != 0) {
			printf("Error: Thread creation failed: %d.\n", err);
			return -1;
		}
	}

	for (t = 0; t < num_thread; t++) {
		pthread_join(threads[t], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &stop);
	my_spinlock_destroy(&lock_my_spin_TTAS);

	result = timespecDiff(&stop,&start);
	printf("RESULTS - My Spinlock TTAS:\n");
        printf("Total Count: %lld\n", count_in);
        printf("Time (ms): %llu\n\n",result/1000000);
}

int run_my_mutex() {
	int t, err = 0;
	count_in = 0;
	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * num_thread);
	my_mutex_init(&lock_my_mutex);

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (t = 0; t < num_thread; t++) {
		err = pthread_create(threads + t, NULL, &test_my_mutex, NULL);
		if (err != 0) {
			printf("Error: Thread creation failed: %d.\n", err);
			return -1;
		}
	}

	for (t = 0; t < num_thread; t++) {
		pthread_join(threads[t], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &stop);
	my_mutex_destroy(&lock_my_mutex);

	result = timespecDiff(&stop,&start);
	printf("RESULTS - My Mutex:\n");
        printf("Total Count: %lld\n", count_in);
        printf("Time (ms): %llu\n\n",result/1000000);
}

int run_my_queue() {
	int t, err = 0;
	count_in = 0;
	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * num_thread);
	my_queuelock_init(&lock_my_queue);

	clock_gettime(CLOCK_MONOTONIC, &start);

	for (t = 0; t < num_thread; t++) {
		err = pthread_create(threads + t, NULL, &test_my_queue, NULL);
		if (err != 0) {
			printf("Error: Thread creation failed: %d.\n", err);
			return -1;
		}
	}

	for (t = 0; t < num_thread; t++) {
		pthread_join(threads[t], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &stop);
	my_queuelock_destroy(&lock_my_queue);

	result = timespecDiff(&stop,&start);
	printf("RESULTS - My Queuelock:\n");
        printf("Total Count: %lld\n", count_in);
        printf("Time (ms): %llu\n\n",result/1000000);
}

void run_test() {
	if (num_ID == 0) {
		run_mutex();
		sleep(1);

		run_spin();
		sleep(1);

		run_my_spin_TAS();
		sleep(1);

		run_my_spin_TTAS();
		sleep(1);

		run_my_mutex();
		sleep(1);

		run_my_queue();
		sleep(1);
	} else if (num_ID == 1) {
		run_mutex();
	} else if (num_ID == 2) {
		run_spin();
	} else if (num_ID == 3) {
		run_my_spin_TAS();
	} else if (num_ID == 4) {
		run_my_spin_TTAS();
	} else if (num_ID == 5) {
		run_my_mutex();
	} else if (num_ID == 6) {
		run_my_queue();
	} else {
		printf("Error: Invalid test ID!\n");
	}
}

void run_test_custom() {
	if (num_ID == 0) {
		printf("Doing all tests except for myQueue!\n\n");
		run_mutex();
		sleep(1);

		run_spin();
		sleep(1);

		run_my_spin_TAS();
		sleep(1);

		run_my_spin_TTAS();
		sleep(1);

		run_my_mutex();
		sleep(1);

	} else if (num_ID == 1) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workIn = k;
			printf("Inside CS = %5i **********\n\n", k);
			run_mutex();
			sleep(1);	
		}

	} else if (num_ID == 2) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workIn = k;
			printf("Inside CS = %5i **********\n\n", k);
			run_spin();
			sleep(1);	
		}

	} else if (num_ID == 3) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workIn = k;
			printf("Inside CS = %5i **********\n\n", k);
			run_my_spin_TAS();
			run_my_spin_TTAS();
			sleep(1);	
		}

	} else if (num_ID == 4) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workIn = k;
			printf("Inside CS = %5i **********\n\n", k);
			run_my_mutex();
			sleep(1);	
		}

	} else if (num_ID == 5) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workIn = k;
			printf("Inside CS = %5i **********\n\n", k);
			run_my_queue();
			sleep(1);	
		}
	} else if (num_ID == 6) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workOut = k;
			printf("Outside CS = %5i *********\n\n", k);
			run_mutex();
			sleep(1);	
		}

	} else if (num_ID == 7) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workOut = k;
			printf("Outside CS = %5i *********\n\n", k);
			run_spin();
			sleep(1);	
		}

	} else if (num_ID == 8) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workOut = k;
			printf("Outside CS = %5i *********\n\n", k);
			run_my_spin_TAS();
			run_my_spin_TTAS();
			sleep(1);	
		}

	} else if (num_ID == 9) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workOut = k;
			printf("Outside CS = %5i *********\n\n", k);
			run_my_mutex();
			sleep(1);	
		}

	} else if (num_ID == 10) {
		int k;
		for (k = 1; k <= 65536; k *= 2) {
			num_workOut = k;
			printf("Outside CS = %5i *********\n\n", k);
			run_my_queue();
			sleep(1);	
		}

	} else if (num_ID == 11) {
		int k;
		for (k = 1; k <= 67108864; k *= 2) {
			num_iter = k;
			printf("Iteration = %7i ********\n\n", k);
			run_mutex();
			sleep(1);	
		}

	} else if (num_ID == 12) {
		int k;
		for (k = 1; k <= 67108864; k *= 2) {
			num_iter = k;
			printf("Iteration = %7i ********\n\n", k);
			run_spin();
			sleep(1);	
		}

	} else if (num_ID == 13) {
		int k;
		for (k = 1; k <= 467108864; k *= 2) {
			num_iter = k;
			printf("Iteration = %7i ********\n\n", k);
			run_my_spin_TAS();
			run_my_spin_TTAS();
			sleep(1);	
		}

	} else if (num_ID == 14) {
		int k;
		for (k = 1; k <= 67108864; k *= 2) {
			num_iter = k;
			printf("Iteration = %7i ********\n\n", k);
			run_my_mutex();
			sleep(1);	
		}

	} else if (num_ID == 15) {
		int k;
		for (k = 1; k <= 67108864; k *= 2) {
			num_iter = k;
			printf("Iteration = %7i ********\n\n", k);
			run_my_queue();
			sleep(1);	
		}

	} else if (num_ID == 21) {
		printf("Doing all tests without workIn/workOut!\n\n");
		num_workIn = 0;
		num_workOut = 0;

		run_mutex();
		sleep(1);

		run_spin();
		sleep(1);

		run_my_spin_TAS();
		sleep(1);

		run_my_spin_TTAS();
		sleep(1);

		run_my_mutex();
		sleep(1);

		run_my_queue();
		sleep(1);

	} else if (num_ID == 22) {
		int save_workIn = num_workIn;
		int save_workOut = num_workOut;

		printf("Mutex ******************\n\n");
		num_workOut = 0;
		num_workIn = save_workIn;
		printf("0 out\n");
		run_mutex();
		num_workIn = 0;
		num_workOut = save_workOut;
		printf("0 in\n");
		run_mutex();
		sleep(1);

		printf("Spin *******************\n\n");
		num_workOut = 0;
		num_workIn = save_workIn;
		printf("0 out\n");
		run_spin();
		num_workIn = 0;
		num_workOut = save_workOut;
		printf("0 in\n");
		run_spin();
		sleep(1);

		printf("mySpin TAS *************\n\n");
		num_workOut = 0;
		num_workIn = save_workIn;
		printf("0 out\n");
		run_my_spin_TAS();
		num_workIn = 0;
		num_workOut = save_workOut;
		printf("0 in\n");
		run_my_spin_TAS();
		sleep(1);

		printf("mySpin TTAS ************\n\n");
		num_workOut = 0;
		num_workIn = save_workIn;
		printf("0 out\n");
		run_my_spin_TTAS();
		num_workIn = 0;
		num_workOut = save_workOut;
		printf("0 in\n");
		run_my_spin_TTAS();
		sleep(1);

		printf("myMutex ****************\n\n");
		num_workOut = 0;
		num_workIn = save_workIn;
		printf("0 out\n");
		run_my_mutex();
		num_workIn = 0;
		num_workOut = save_workOut;
		printf("0 in\n");
		run_my_mutex();
		sleep(1);

		
		printf("myQueue ****************\n\n");
		num_workOut = 0;
		num_workIn = save_workIn;
		printf("0 out\n");
		run_my_queue();
		num_workIn = 0;
		num_workOut = save_workOut;
		printf("0 in\n");
		run_my_queue();
		sleep(1);
		

	} else if (num_ID == 23) {
		int k;
		int threadtests[5] = {1, 4, 8, 9, 32};
		for (k = 0; k < 5; k++) {
			printf("Threads = %i *************\n\n", threadtests[k]);
			num_thread = threadtests[k];
			run_mutex();
			sleep(1);

			run_spin();
			sleep(1);

			run_my_spin_TAS();
			sleep(1);

			run_my_spin_TTAS();
			sleep(1);

			run_my_mutex();
			sleep(1);

			run_my_queue();
			sleep(1);
		}
		
	} else {
		printf("Error: Invalid test ID!\n");
	}
}

/*num_ID: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock*/
	/*You must write how to parse input from the command line here, your software should default to the values given below if no input is given*/

/*******************************************/
/* INPUT PROCESSING                        */
/*******************************************/

void process_input(int argc, char *argv[]) {
	int i = 1;

	while (i < argc && argv[i]) {
		if (!argv[i]) {
			printf("Error: Invalid input. Exiting...\n");
			exit(0);

		} else if (strcmp(argv[i], "-t") == 0) {
			i++;
			num_thread = atoi(argv[i]);

		} else if (strcmp(argv[i], "-i") == 0) {
			i++;
			num_iter = atoi(argv[i]);

		} else if (strcmp(argv[i], "-d") == 0) {
			i++;
			num_ID = atoi(argv[i]);

		} else if (strcmp(argv[i], "-o") == 0) {
			i++;
			num_workOut = atoi(argv[i]);

		} else if (strcmp(argv[i], "-c") == 0) {
			i++;
			num_workIn = atoi(argv[i]);

		} else if (strcmp(argv[i], "-u") == 0) {
			i++;
			num_custom = atoi(argv[i]);

		} else {
			printf("Error: Unrecognized input. Exiting...\n");
			exit(0);
		}


		i++;	
	}

	if (num_custom == 1) {
		printf("USING CUSTOM TESTS\n");
	} else {
		printf("USING STANDARD TESTS\n");
	}

	printf("Test ID number is:   %i\n", num_ID);
	printf("Thread number is:    %i\n", num_thread);
	printf("Iteration number is: %i\n", num_iter);
	printf("Outside CS ops is:   %i\n", num_workOut);
	printf("Inside CS ops is:    %i\n\n", num_workIn);
	sleep(1);
}

/*******************************************/
/* MAIN                                    */
/*******************************************/

int main(int argc, char *argv[]) {
	print_commands();
	
	set_default_values();
	process_input(argc,argv);
	if (num_custom == 0) {
		run_test();
	} else {
		run_test_custom();
	}
	
	return 0;
}
