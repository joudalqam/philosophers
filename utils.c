/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:56:12 by jalqam            #+#    #+#             */
/*   Updated: 2025/07/01 17:39:52 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long timestamp_in_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void print_action(t_philo *philo, char *action)
{
    long current_time;

    pthread_mutex_lock(&philo->data->dead_mutex);
    if (philo->data->is_dead)
    {
        pthread_mutex_unlock(&philo->data->dead_mutex);
        return;
    }
    pthread_mutex_lock(&philo->data->print_mutex);
    current_time = timestamp_in_ms() - philo->data->start_time;
    printf("%ld %d %s\n", current_time, philo->id, action);
    pthread_mutex_unlock(&philo->data->print_mutex);
    pthread_mutex_unlock(&philo->data->dead_mutex);
}

void philo_taking_forks(t_philo *philo)
{
	if(philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork.");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork.");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork.");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork.");
	}
}

int	is_dead(t_philo *philo)
{
	int	status;

	pthread_mutex_lock(&philo->data->dead_mutex);
	status = philo->data->is_dead;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (status);
}

void ft_usleep(long long time, t_philo *philo)
{
	long long start ;
	start = timestamp_in_ms();
	while (timestamp_in_ms() - start < time)
	{
		if (is_dead(philo))
            break;
		usleep(100);
	}
}

