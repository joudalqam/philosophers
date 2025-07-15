/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:11:26 by jalqam            #+#    #+#             */
/*   Updated: 2025/07/02 13:47:23 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	**philos;

	if (valid_args(argc, argv) == 1)
	{
		write(2, "Error: Invalid arguments.\n", 26);
		return (1);
	}
	if (init_data(&data, argv))
		return (1);
	philos = malloc(sizeof(t_philo *) * data.philo_count);
	if (!philos)
	{
		ft_free(philos, &data);
		return (1);
	}
	if (init_philosophers(&data, philos))
	{
		ft_free(philos, &data);
		return (1);
	}
	init_threads(&data, philos);
	ft_free(philos, &data);
	return (0);
}
