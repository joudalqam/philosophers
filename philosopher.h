/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:32:02 by jalqam            #+#    #+#             */
/*   Updated: 2025/06/28 17:10:10 by jalqam           ###   ########.fr       */
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
#include <sys/time.h>

typedef struct s_philo t_philo;

typedef struct s_data
{
	int philo_count;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int meals_required;
	int is_dead;
	long long start_time;
	
	pthread_mutex_t *forks;
	pthread_mutex_t dead_mutex;
	pthread_mutex_t print_mutex;
	int full_count;
}	t_data;

typedef struct s_philo
{
	int id;
	int meals_eaten;
	long long last_meal_time;
	t_data *data;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	pthread_t       thread; 
}	t_philo;

int	ft_atoi(const char *str);
int is_positive_digit(char *argv);
int check_args(int argc, char *argv[]);
int valid_args(int argc, char **argv);
int init_data(t_data *data, char **argv);
void *philo_routine(void *arg);
int init_threads(t_data *data, t_philo **philos);
int init_philosophers(t_data *data, t_philo **philos);
long timestamp_in_ms(void);
void print_action(t_philo *philo, char *action);
void philo_taking_forks(t_philo *philo);
int	is_dead(t_philo *philo);
int philo_eating(t_philo *philo);
void philo_put_forks(t_philo *philo);
void *monitor_routine(void *arg);
int	sleeping(t_philo *philo);
void ft_usleep(long long time, t_philo *philo);
int ft_free(t_philo **philos, t_data *data);
# endif