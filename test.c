#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define N 5  // Number of philosophers and forks

typedef enum { THINKING, HUNGRY, EATING } State;

State state[N];  // Track philosopher states
pthread_mutex_t mutex;  // Protects critical sections
pthread_mutex_t forks[N];  // Mutex for each fork

size_t left(size_t i)
{
	return (i + N - 1) % N;
}
size_t right(size_t i)
{
	return (i + 1) % N;
}

long get_timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);  // Convert to milliseconds
}

void print_state(size_t i, const char *msg)
{
    printf("%ld %zu %s\n", get_timestamp(), i, msg);
}

void test(size_t i)
{
    if (state[i] == HUNGRY && state[left(i)] != EATING && state[right(i)] != EATING) {
        state[i] = EATING;
        pthread_mutex_unlock(&forks[i]);  // Signal philosopher to proceed
    }
}

void think(size_t i)
{
    print_state(i, "is thinking");
    usleep((rand() % 400 + 400) * 1000);
}

void take_forks(size_t i)
{
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    print_state(i, "is hungry");
    test(i);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_lock(&forks[i]);  // Block if no forks available
}

void eat(size_t i)
{
    print_state(i, "is eating");
    usleep((rand() % 400 + 400) * 1000);
}

void put_forks(size_t i)
{
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    test(left(i));
    test(right(i));
    pthread_mutex_unlock(&mutex);
}

void *philosopher(void *arg)
{
    size_t i = (size_t)arg;
    while (1)
	{
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
    }
    return NULL;
}

int main()
{
    pthread_t threads[N];
    srand(time(NULL));

    memset(state, THINKING, sizeof(state));

    pthread_mutex_init(&mutex, NULL);

    for (size_t i = 0; i < N; i++)
	{
        pthread_mutex_init(&forks[i], NULL);
        pthread_mutex_lock(&forks[i]);  // Locked initially
    }

    for (size_t i = 0; i < N; i++)
	{
        pthread_create(&threads[i], NULL, philosopher, (void *)i);
    }

    for (size_t i = 0; i < N; i++)
	{
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    for (size_t i = 0; i < N; i++)
	{
        pthread_mutex_destroy(&forks[i]);
    }
    return 0;
}
