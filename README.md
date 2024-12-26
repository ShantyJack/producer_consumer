# producer_consumer

## background
Multithreading is a powerful technique that allows you to, under the right situation, greatly speed up the execution of your program. If you have an application where there are distinct functions that could be executed in parallel, multithreading allows you to assign those functions to different threads, meaning that the operating system can assign execution to all of those functions simultaneously. This of course can lead to some pitfalls, most salient being the problem of shared resources. What happens if two threads attempt to access a shared resource at the same time? Well it wouldn't be good, and it's better to avoid it altogether. This problem is best illustrated with the producer-consumer problem. In this problem there are two seperate threads, the consumer and the producer. One puts numbers into a shared buffer while the other reads them out. It might not be the most practically useful problem, but it does a good job of illustrating the kinds of synchronization problems that can happen in larger programs. These are solved using a semaphore

## semaphore
A semaphore is a tool that aids in synchronization of multiple threads. Specifically, it allows you to make sure that no more than one thread is acessing the same resource at the same time. From a user perspective, this manifests as a series of calls to "wait" and "post". The "wait" call is used right before entering the critical section and it allows the thread to signal that it is accessing the shared resource, or it can stop a thread from accessing the shared resource if another is already doing so. The "post" call just signals that the thread is no longer accessing the critical section and lets in any waiting thread if there is any. 

## overview
The program takes 3 parameters from the command line: buffer size, producer sleep, and consumer sleep in that order. Buffer size is how many integers the buffer should hold, while the two 'sleep' values control how long the two threads sleep each time after producing or consuming a value. Each time they do put an integer into the buffer or take one out, they will print a message to the command line. As long as there is a difference between these sleep values, one of the threads will catch up to spot being used by the other thread, in which case they will stall. If you want to change the sleep values, there are a number of commands to do so

## commands
the commands can be entered any time by pressing the corresponding key:
a - adds .25 seconds to the producer's sleep time
z - subtracts .25 seconds from the producer's sleep time
s - adds .25 seconds to the consumer's sleep time
x - subtracts .25 seconds from the consumer's sleep time
q - quits out of the program
