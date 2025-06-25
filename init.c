/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:52:57 by jalqam            #+#    #+#             */
/*   Updated: 2025/06/25 16:15:03 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

t_data *init_data(char **argv)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * ft_atoi(argv[1]));
	if (!data->forks)
	{
		free(data);
		return (NULL);
	}
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->meals_required = ft_atoi(argv[5]);
	else
		data->meals_required = -1;
	data->meals_eaten = 0;
	data->start_time = 0;
	data->is_dead = false;

	return (data);
}

t_philo **init_philosophers(t_data *data, t_philo **philos)
{
	int i;

	i = 0;
	while (i < data->philo_count)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			return (NULL);
		philos[i]->id = i + 1;
		philos[i]->meals_eaten = 0;
		philos[i]->last_meal_time = 0;
		philos[i]->data = data;
		philos[i]->left_fork = &data->forks[i];
		if (i == data->philo_count - 1)
			philos[i]->right_fork = &data->forks[0];
		else
			philos[i]->right_fork = &data->forks[i + 1];
		i++;
	}
	return (philos);
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;  
	long current_time;

	current_time = timestamp_in_ms();
    printf("%ld %d is thinking.\n",current_time, philo->id);
	printf("%ld %d is eating.\n",current_time, philo->id);
	printf("%ld %d is sleeping.\n",current_time, philo->id);
    return NULL;
}

void init_threads(t_data *data, t_philo **philos)
{
	int i;

	i = 0;
	while (i < data->philo_count)
	{
		if(pthread_create(&philos[i]->thread,NULL, philo_routine, philos[i]) != 0)
		{
			perror("Failed to create thread");
			exit(EXIT_FAILURE);
		}
			
		i++;
	}
	i = 0;
	while(i < data->philo_count)
	{
		if(pthread_join(philos[i]->thread, NULL) != 0)
		{
			perror("Failed to join thread");
			exit(EXIT_FAILURE);
		}
		else
			pthread_join(philos[i]->thread, NULL);
		i++;
	}
}

long timestamp_in_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
