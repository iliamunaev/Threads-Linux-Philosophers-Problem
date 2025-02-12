/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:37:17 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/12 13:20:30 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	log_action(t_philo *philo, const char *action)
{
	pthread_mutex_lock(&philo->sim->log_mutex);
	printf("%ldms Philosopher %u %s\n", get_timestamp(), philo->index, action);
	pthread_mutex_unlock(&philo->sim->log_mutex);
}




int	*init_threads(t_phls *phls, size_t p)
{
	while (phls->id < p)
	{
		if (pthread_create(phls->id, NULL, simulation, phls->id) != 0)
			return (EXIT_FAILURE);
		phls->id++;
	}
	return (EXIT_SUCCESS);
}

t_phls	*init_phls(size_t p)
{
	t_phls	*phls;

	phls = malloc(p * sizeof(t_phls));
	if(!phls)
	{
		printf("Error:  init_phls() malloc fail.\n");
		return (NULL);
	}
	return (phls);
}

int	init_mutex(t_phls *phls)
{
	int	i;

	while(i < phls->id)
	{
		if(pthread_mutex_init(phls->fork, NULL) != 0)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}



void	simulation(void *arg)
{
	size_t	i;
	t_sim	*sim;

	i = (size_t)arg;	
	sim = init_simulation(i);
	if (!sim)
	{
		// err msg
		// stop simulation
		return ;
	}

	while (1)
	{
		think(sim, i);
		
		if(is_free_forks(sim, i))
		{
			take_forks(sim, i);
			eat(sim, i);
			put_forks(sim, i);			
		}
		
		sleep(sim, i);
	}
}

int	main(int ac, char **av)
{
	// if (!is_input_valid(ac, av))
	// {
	// 	return (EXIT_FAILURE);
	// }

	simulation();
	

    memset(state, thinking, sizeof(t_state));


	// alloc memory for phils structures
	t_philo	*phls;
	phls = init_phls(p);
	if (!phls)
	{
		// do smth
		return (EXIT_FAILURE);
	}

	// start threads
	pthread_t	*threads;
	threads = init_threads(phls, p);
	if (threads != 0)
	{
		// do smth
		return (EXIT_FAILURE);
	}

	// start mutex
	pthread_mutex_t	*mutex;
	mutex = init_mutex(phls);
	if (mutex != EXIT_SUCCESS)
	{
		// do smth
		return (EXIT_FAILURE);
	}


	//starts a new threads in the calling process

	i = 0;
	while (i < t)
	{
		if (pthread_join(&threads[i], NULL) != 0)
			return (EXIT_FAILURE);
		i++;
	}

	return (EXIT_SUCCESS);
}
