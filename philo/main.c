/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:37:17 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/11 21:50:14 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	log_action(t_philo *philo, const char *action)
{
	pthread_mutex_lock(&philo->sim->log_mutex);
	printf("%ldms Philosopher %u %s\n", get_timestamp(), philo->index, action);
	pthread_mutex_unlock(&philo->sim->log_mutex);
}


// void *thread_function(void *arg);

void	*philosopher(void *arg)
{
	uint32_t	i;

	i = (uint32_t)arg;
	printf("Thread: %d, time: %d\n", );

	while (1)
	{
		think(i);
		take_forks(i);
		eat(i);
		put_forks(i);
		sleep(i);
	}
}

int	*init_threads(t_phls *phls, uint32_t p)
{
	while (phls->id < p)
	{
		if (pthread_create(phls->id, NULL, philosopher, phls->id) != 0)
			return (EXIT_FAILURE);
		phls->id++;
	}
	return (EXIT_SUCCESS);
}

t_phls	*init_phls(uint32_t p)
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


int	main(int ac, char **av)
{
	// if (!is_input_valid(ac, av))
	// {
	// 	return (EXIT_FAILURE);
	// }
	// init treads

	uint32_t	p;
	p = atoi(av[1]);



	// alloc memory for phils structures
	t_phls	*phls;
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
