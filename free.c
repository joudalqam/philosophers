/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:39:18 by jalqam            #+#    #+#             */
/*   Updated: 2025/07/01 17:40:00 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int ft_free(t_philo **philos, t_data *data)
{
	int i;

	i = -1;
	if (philos)
	{
		while (++i < data->philo_count)
			if (philos[i])
				free(philos[i]);
		free(philos);
	}
	i = -1;
	if (data->forks)
	{
		while (++i < data->philo_count)
			if (&data->forks[i])
				pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}
	pthread_mutex_destroy(&data->dead_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	return (1);
}
