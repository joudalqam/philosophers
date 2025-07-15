/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:52:57 by jalqam            #+#    #+#             */
/*   Updated: 2025/07/02 13:46:00 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (ft_free(NULL, data));
		i++;
	}
	if (pthread_mutex_init(&data->dead_mutex, NULL) != 0)
		return (ft_free(NULL, data));
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (ft_free(NULL, data));
	return (0);
}

int	init_data(t_data *data, char **argv)
{
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->meals_required = ft_atoi(argv[5]);
	else
		data->meals_required = -1;
	data->is_dead = 0;
	data->start_time = timestamp_in_ms();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->forks)
	{
		free(data);
		return (1);
	}
	if (init_mutexes(data) != 0)
		return (1);
	return (0);
}

int	init_philosophers(t_data *data, t_philo **philos)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{
			free(philos);
			return (1);
		}
		philos[i]->id = i + 1;
		philos[i]->meals_eaten = 0;
		philos[i]->last_meal_time = data->start_time;
		philos[i]->data = data;
		philos[i]->left_fork = &data->forks[i];
		if (i == data->philo_count - 1)
			philos[i]->right_fork = &data->forks[0];
		else
			philos[i]->right_fork = &data->forks[i + 1];
		i++;
	}
	return (0);
}

int	init_threads(t_data *data, t_philo **philos)
{
	pthread_t	monitor_thread;
	int			i;

	i = -1;
	pthread_create(&monitor_thread, NULL, monitor_routine, philos);
	while (++i < data->philo_count)
	{
		if (pthread_create(&philos[i]->thread
				, NULL, philo_routine, philos[i]) != 0)
		{
			ft_free(philos, data);
			return (1);
		}
	}
	pthread_join(monitor_thread, NULL);
	i = -1;
	while (++i < data->philo_count)
	{
		if (pthread_join(philos[i]->thread, NULL) != 0)
		{
			ft_free(philos, data);
			return (1);
		}
	}
	return (0);
}
