#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>


void *philosopher(void *arg)
{
    uint32_t i = (uintptr_t)arg;  // Safe conversion
	int h = 1000;
	int j = 0;
    while (j < h)
    {
        printf("Thread: %u, time: %ld\n", i, time(NULL));
        // sleep(1);  // Simulate philosopher thinking
		j++;
    }
    return NULL;
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", av[0]);
        return EXIT_FAILURE;
    }

    uint32_t t = atoi(av[1]);
    if (t == 0)
    {
        fprintf(stderr, "Invalid thread count\n");
        return EXIT_FAILURE;
    }

    pthread_t threads[t];
    uint32_t i = 0;

    while (i < t)
    {
        if (pthread_create(&threads[i], NULL, philosopher, (void *)(uintptr_t)i) != 0)
        {
            fprintf(stderr, "Error creating thread %u\n", i);
            return EXIT_FAILURE;
        }
        i++;
    }

    i = 0;
    while (i < t)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "Error joining thread %u\n", i);
            return EXIT_FAILURE;
        }
        i++;
    }

    return EXIT_SUCCESS;
}
