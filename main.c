/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:11:26 by jalqam            #+#    #+#             */
/*   Updated: 2025/06/26 16:59:43 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_atoi(const char *str)
{
	long	result = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		result = result * 10 + (*str - '0');
		if (result > INT_MAX)
			return (-1);
		str++;
	}
	return ((int)result);
}

int is_positive_digit(char *argv)
{
	int i;

	i = 0;
	if (!argv || !*argv)
		return 0;
	if (argv[i] == '+' || argv[i] != '-')
		i++;
	while(argv[i])
	{
		if (argv[i] < '0' || argv[i] > '9')
			return 0;
		i++;
	}
	return 1;
}

int check_args(int argc, char *argv[])
{
	int i;

	i = 1;
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments.\n");
		return (1);
	}
	while(i < argc)
	{
		if (!is_positive_digit(argv[i]))
		{
			printf("Error: Invalid Argument\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int valid_args(int argc, char **argv)
{
	int num_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int num_meals;
	
	if(!check_args(argc, argv))
		return (1);
	num_philosophers = ft_atoi(argv[1]);
	time_to_die = ft_atoi(argv[2]);
	time_to_eat = ft_atoi(argv[3]);
	time_to_sleep = ft_atoi(argv[4]);
	if(num_philosophers <= 0 || num_philosophers > 200)
		return (1);
	if (time_to_die < 60 || time_to_eat < 60 || time_to_sleep < 60)
		return (1);
	if (argc == 6)
	{
		num_meals = ft_atoi(argv[5]);
		if (num_meals <= 0)
			return (1);
	}
	return (0);
}




int main(int argc, char **argv)
{
	if (check_args(argc, argv) == 1)
		return (1);
	// pthread_t monitor_thread;
	t_data *data;
	t_philo **philos;
	data = init_data(argv);
	philos = malloc(sizeof(t_philo *) * data->philo_count);
	philos = init_philosophers(data, philos);
	data->philo = philos;
	init_threads(data, philos);
	
	if (!data || !philos)
		return (1);

}
