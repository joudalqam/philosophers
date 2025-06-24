/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:52:57 by jalqam            #+#    #+#             */
/*   Updated: 2025/06/24 17:37:45 by jalqam           ###   ########.fr       */
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