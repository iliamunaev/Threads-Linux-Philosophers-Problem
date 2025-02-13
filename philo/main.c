#include "philo.h"

int main(int ac, char **av)
{
    if (!is_input_valid(ac, av))
        return (EXIT_FAILURE);

    t_sim *sim = init_sim(av);
    if (!sim)
    {
        // Error message already printed in init_sim
        return (EXIT_FAILURE);
    }

    t_ph *ph = init_ph(sim, av);
    if (!ph)
    {
        cleanup(sim, NULL);
        return (EXIT_FAILURE);
    }

    /*
    ** Create a thread for each philosopher.
    */
    for (long i = 0; i < sim->ph_count; i++)
    {
        if (pthread_create(&sim->ph_threads[i], NULL, &philosopher, &ph[i]) != 0)
        {
            print_err("Error: main() pthread_create philosopher failed.");

            /* Optional: join/cleanup partially created threads if you want full correctness. */
            for (long j = 0; j < i; j++)
                pthread_join(sim->ph_threads[j], NULL);

            cleanup(sim, ph);
            return (EXIT_FAILURE);
        }
    }

    /*
    ** Join all philosopher threads.
    */
    for (long i = 0; i < sim->ph_count; i++)
    {
        if (pthread_join(sim->ph_threads[i], NULL) != 0)
        {
            print_err("Error: main() pthread_join failed.");
            // We’ll still proceed to clean up below.
        }
    }

    /*
    ** Clean up and exit
    */
    cleanup(sim, ph);
    return (EXIT_SUCCESS);
}
