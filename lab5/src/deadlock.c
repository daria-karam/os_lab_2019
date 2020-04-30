/*
As an example, consider the classical dining philocophers problem:

Five silent philosophers sit at a round table with bowls of spaghetti.
Forks are placed between each pair of adjacent philosophers.
Each philosopher must alternately think and eat.
However, a philosopher can only eat spaghetti when they have both left and right forks.
Each fork can be held by only one philosopher and so a philosopher can use the fork only
if it is not being used by another philosopher.
After an individual philosopher finishes eating,
he needs to put down both forks so that the forks become available to others.
A philosopher can only take the fork on their right or the one on their left
as they become available and they cannot start eating before getting both forks.

Eating is not limited by the remaining amounts of spaghetti or stomach space;
an infinite supply and an infinite demand are assumed.

Algorythm for each philosopher:
1. think until the left fork is available; when it is, pick it up;
2. think until the right fork is available; when it is, pick it up;
3. when both forks are held, eat for a fixed amount of time;
4. then, put the right fork down;
5. then, put the left fork down;
6. repeat from the beginning.
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
 
#define PHT_SIZE 5

/* declare the "philocopher" structure, which will store
the name of the philosopher and the numbers of forks that it can take */
typedef struct philosopher_tag
{
    const char *name;
    unsigned left_fork;
    unsigned right_fork;
} philosopher_t;

/* the table structure consists of an array of forks.
The fork will be a mutex. Locking the mutex means "take the fork"
and unlocking it means "put it down" */
typedef struct table_tag
{
    pthread_mutex_t forks[PHT_SIZE];
} table_t;

/* to pass these parameters to the thread, combine them into this structure */
typedef struct philosopher_args_tag
{
    const philosopher_t *philosopher;
    const table_t *table;
} philosopher_args_t;

/* it is necessary not to let anyone take the fork
while the one philosopher takes the fork */
pthread_mutex_t entry_point = PTHREAD_MUTEX_INITIALIZER;

/* auxiliary initialization function */
void init_philosopher(philosopher_t *philosopher, const char *name,
unsigned left_fork, unsigned right_fork)
{
    philosopher->name = name;
    philosopher->left_fork = left_fork;
    philosopher->right_fork = right_fork;
}

/* auxiliary initialization function */
void init_table(table_t *table)
{
    size_t i;
    for (i = 0; i < PHT_SIZE; i++)
    {
        pthread_mutex_init(&table->forks[i], NULL);
    }
}

/* the main function, which simulates dinner */
void* eat(void *args)
{
    philosopher_args_t *arg = (philosopher_args_t*) args;
    const philosopher_t *philosopher = arg->philosopher;
    const table_t *table = arg->table;
    unsigned rand_time;  
    srand(NULL);
    do {
        printf("%s started dinner\n", philosopher->name);
 
        pthread_mutex_lock(&entry_point); //philosopher wants to take fork
        /* take the left fork, because it is available */
        pthread_mutex_lock(&table->forks[philosopher->left_fork]);
        pthread_mutex_unlock(&entry_point); //he picked up the fork
        /* think for a random time(wait):
        this is necessary to demonstrate deadlock,
        because initialization of threads need some time */
        rand_time =rand()%10;
        sleep(rand_time);
        /* take the left fork (try) */
        pthread_mutex_lock(&entry_point); //philosopher wants to take fork
        pthread_mutex_lock(&table->forks[philosopher->right_fork]);
        pthread_mutex_unlock(&entry_point); //he picked up the fork
        /* it turned out to take both forks, philoshopher eats */
        printf("%s is eating after %d s sleep\n", philosopher->name, rand_time);
        /* put the right fork down */
        pthread_mutex_unlock(&table->forks[philosopher->right_fork]);
        /* put the left fork down */
        pthread_mutex_unlock(&table->forks[philosopher->left_fork]);
        /* philoshopher finished dinner */
        printf("%s finished dinner\n", philosopher->name);
    } while (1); /* repeat */
}
 
void main()
{
    pthread_t threads[PHT_SIZE];
    philosopher_t philosophers[PHT_SIZE];
    philosopher_args_t arguments[PHT_SIZE];
    table_t table;
    size_t i;
 
    init_table(&table);
 
    init_philosopher(&philosophers[0], "Apollonius", 0, 1);
    init_philosopher(&philosophers[1], "Socrates",   1, 2);
    init_philosopher(&philosophers[2], "Parmenides", 2, 3);
    init_philosopher(&philosophers[3], "Heraclitus", 3, 4);
    init_philosopher(&philosophers[4], "Aristotle", 4, 0);
 
    for (i = 0; i < PHT_SIZE; i++)
    {
        arguments[i].philosopher = &philosophers[i];
        arguments[i].table = &table;
    }
 
    for (i = 0; i < PHT_SIZE; i++)
    {
        pthread_create(&threads[i], NULL, eat, &arguments[i]);
    }
 
    for (i = 0; i < PHT_SIZE; i++)
    {
        pthread_join(threads[i], NULL);
    }
}