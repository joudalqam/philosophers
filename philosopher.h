/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:32:02 by jalqam            #+#    #+#             */
/*   Updated: 2025/06/24 18:18:45 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>	
# include <sys/time.h>
# include <stdbool.h>
# include <string.h>
# include <semaphore.h>
# include <fcntl.h>
# include <limits.h>

typedef struct s_philo t_philo;

typedef struct s_data
{
	int philo_count;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int meals_required;
	int meals_eaten;
	int start_time;
	bool is_dead;
	pthread_mutex_t *forks;
	pthread_mutex_t print_mutex;
	t_philo *philo;
}	t_data;

typedef struct s_philo
{
	int id;
	int meals_eaten;
	int last_meal_time;
	t_data *data;
	pthread_t thread_id;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
}	t_philo;

int	ft_atoi(const char *str);
int is_positive_digit(char *argv);
int check_args(int argc, char *argv[]);
int valid_args(int argc, char **argv);
t_data *init_data(char **argv);


# endif