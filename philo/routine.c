/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:32:20 by jalqam            #+#    #+#             */
/*   Updated: 2025/07/09 13:10:49 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->philo_count == 1)
	{
		one_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(50);
	while (!is_dead(philo))
	{
		if (philo_eating(philo))
			break ;
		if (philo->data->meals_required != -1
			&& philo->meals_eaten >= philo->data->meals_required)
			break ;
		if (philo_sleeping(philo))
			break ;
		philo_thinking(philo);
		usleep(1);
	}
	return (NULL);
}

int	one_philo(t_philo *philo)
{
	print_action(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die + 1, philo);
	pthread_mutex_lock(&philo->data->dead_mutex);
	philo->data->is_dead = 1;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}

int	philo_eating(t_philo *philo)
{
	philo_taking_forks(philo);
	if (is_dead(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	// pthread_mutex_lock(&philo->data->dead_mutex);
	philo->last_meal_time = timestamp_in_ms();
	philo->meals_eaten++;
	// pthread_mutex_unlock(&philo->data->dead_mutex);
	print_action(philo, "is eating.");
	ft_usleep(philo->data->time_to_eat, philo);
	if (philo->id % 2 == 1)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	return (0);
}

int	philo_sleeping(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep, philo);
	return (0);
}

void	philo_thinking(t_philo *philo)
{
	print_action(philo, "is thinking.");
	usleep(1);
}
