/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:58:02 by jalqam            #+#    #+#             */
/*   Updated: 2025/07/02 13:26:29 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	check_philo_death(t_philo *philo)
{
	t_data		*data;
	long long	now;

	data = philo->data;
	now = timestamp_in_ms();
	pthread_mutex_lock(&data->dead_mutex);
	if (now - philo->last_meal_time > data->time_to_die)
	{
		data->is_dead = 1;
		pthread_mutex_unlock(&data->dead_mutex);
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %d died\n", now - data->start_time, philo->id);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->dead_mutex);
	return (0);
}

static int	check_meals_required(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (data->meals_required != -1
		&& philo->meals_eaten >= data->meals_required)
		return (1);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	**philos;
	t_data	*data;
	int		i;

	philos = (t_philo **)arg;
	data = philos[0]->data;
	while (!is_dead(philos[0]))
	{
		i = 0;
		while (i < data->philo_count)
		{
			if (check_philo_death(philos[i]))
				return (NULL);
			if (check_meals_required(philos[i]))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}
