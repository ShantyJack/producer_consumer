#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/param.h>

//setting up variables for the buffer to be written to and the semaphore reference
int buffer[500];
int buffer_size, producer_sleep, consumer_sleep, producer_index, consumer_index;
sem_t mtx;

//command loop handles commands and spawining the other threads
void command_loop();
void* producer();
void* consumer();

int main (int argc, char* argv[]){


	char command;
    char string[1];

	/* this block is used for error checking in command input

	printf("enter anything: \n");
	if (fgets(string, 1, stdin) != NULL){
		printf("you said: %c\n", string[0]);
	}
	perror("");
	*/

	//format and print out the command line parameters
	sscanf(argv[1], "%d", &buffer_size);
	sscanf(argv[2], "%d", &producer_sleep);
	sscanf(argv[3], "%d", &consumer_sleep);

	printf("the buffer size is: %d\n", buffer_size);
	printf("the producer sleep interval is: %d\n", producer_sleep);
	printf("the consumer sleep interval is: %d\n", consumer_sleep);

	//initialize the semaphore 
	sem_init(&mtx, 0, 1);

	//create threads for producer and consumer loops
	pthread_t producer_thread, consumer_thread;
	pthread_create(&producer_thread, NULL, producer, NULL);
	pthread_create(&consumer_thread, NULL, consumer, NULL);

	//execute command loop until user inputs exit command
	command_loop();

	//rejoin with the threads and destroy the semaphore before exiting
	int rc1, rc2;
	pthread_join(producer_thread, (void **) &rc1);
	pthread_join(consumer_thread, (void **) &rc2);
	sem_destroy(&mtx);

	return 0;
}


//writes to the shared buffer 
void* producer(){

	while (true){

		//checks if producer sleep is negative before sleeping
		if (producer_sleep < 0){

			break;

		}else{

			usleep(producer_sleep);
		}

		//checks if consumer has caught up to producer
		if (consumer_index == (producer_index + 1)%buffer_size){

			continue;
		}

		//semaphore wait checks to mak sure that the producer isn't accessing the critical section at the same time as the consumer
		sem_wait(&mtx);

		//puts a random value in the buffer and then increments in the index
		buffer[producer_index] = (rand() % (9000 + 1 - 1000)) + 1000;
		printf("Put %d into bin %d\n", buffer[producer_index], producer_index);
		producer_index = (producer_index + 1)%buffer_size;

		//posts to the semaphore, allowing the producer to access it if it was waiting
		sem_post(&mtx);
	}
	return 0;
}


//reads values from the shared buffer
void* consumer(){

        while (true){

				//checks if consumer sleep is negative before sleeping
                if (consumer_sleep < 0){

                        break;

                }else{

                        usleep(consumer_sleep);
                }

				//checks if consumer has caught up to producer
                if (producer_index == (consumer_index + 1)%buffer_size){

                        continue;
                }

				//semaphore wait checks to mak sure that the consumer isn't accessing the critical section at the same time as the producer
                sem_wait(&mtx);

				//takes the value from the buffer and then increments the index
                printf("	got %d from bin %d\n", buffer[consumer_index], consumer_index);
				buffer[consumer_index] = 0;
				consumer_index = (consumer_index + 1)%buffer_size;

				//posts to the semaphore, allowing the producer to access it if it was waiting
                sem_post(&mtx);
        }

	//after finishing, the consumer must finish all leftover values
	while (true){

		//breaks the loop if it gets to the end
		if (buffer[consumer_index] == 0){

			break;

		}else{

			//no need for a semaphore since it's the only one now
			printf("	got %d from bin %d\n", buffer[consumer_index], consumer_index);
            buffer[consumer_index] = 0;
            consumer_index = (consumer_index + 1)%buffer_size;
		}
	}
	return 0;
}

//command loop takes commands to change values in the producer/consumer, and is responsible for ending the program
void command_loop(){

	char command;
	char string[1];

	while (true){

		//takes a command from stdin
		sleep(1);
		sem_wait(&mtx);
		fgets(string, 1, stdin);
		command = string[0];
		printf("RECEIVED COMMAND: %c\n", command);
		sem_post(&mtx);

		//if command is a, add .25 seconds to producer's sleep timer
		if (command == 'a'){

			producer_sleep = MAX(0, producer_sleep + 250);
			continue;
		}

		//if command is a, subtract .25 seconds to producer's sleep timer
		if (command == 'z'){

                        producer_sleep = MAX(0, producer_sleep - 250);
                        continue;
                }

		//if command is a, add .25 seconds to consumer's sleep timer
		if (command == 's'){

                        consumer_sleep = MAX(0, consumer_sleep + 250);
                        continue;
                }

		//if command is a, subtract .25 seconds to consumer's sleep timer
		if (command == 'x'){

                        consumer_sleep = MAX(0, consumer_sleep - 250);
                        continue;
                }

		//if command is q, set the producer and consumer sleep to -1, signaling their threads to end
		if (command == 'q'){

                        printf("now ending\n");
			consumer_sleep = -1;
			producer_sleep = -1;
                        break;
                }
	}
}

