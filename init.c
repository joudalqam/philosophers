/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:52:57 by jalqam            #+#    #+#             */
/*   Updated: 2025/06/28 18:11:33 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int init_data(t_data *data, char **argv)
{
	int i;

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
	data->full_count = 0;
    data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_count);
    if (!data->forks)
		return (1);
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


int	init_philosophers(t_data *data, t_philo **philos)
{
	int i;

	i = 0;
	while (i < data->philo_count)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			return (1);
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

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    if (philo->id % 2 == 0)
        usleep(100);
    while (!is_dead(philo))
    {
		if (philo->data->meals_required != -1 &&
            philo->meals_eaten >= philo->data->meals_required)
            	break;
        if(philo_eating(philo))
            break;
        if (sleeping(philo))
            break;
        print_action(philo, "is thinking.");
    }
    return NULL;
}

int init_threads(t_data *data, t_philo **philos)
{
    pthread_t monitor_thread;
    int i = 0;

    while (i < data->philo_count)
    {
        if(pthread_create(&philos[i]->thread, NULL, philo_routine, philos[i]) != 0)
        {
            perror("Failed to create thread");
            return (1);
        }
        i++;
    }
    pthread_create(&monitor_thread, NULL, monitor_routine, philos);
    i = 0;
    while(i < data->philo_count)
    {
        if(pthread_join(philos[i]->thread, NULL) != 0)
        {
            perror("Failed to join thread");
            return (1);
        }
        i++;
    }
    pthread_join(monitor_thread, NULL);
    return (0);
}


int	sleeping(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep, philo);
	return (0);
}
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

int philo_eating(t_philo *philo)
{
	philo_taking_forks(philo);
	if(is_dead(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return(1);
	}
	pthread_mutex_lock(&philo->data->dead_mutex);
	philo->last_meal_time = timestamp_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	print_action(philo, "is eating.");
	ft_usleep(philo->data->time_to_eat, philo);
	if(philo->id % 2 == 1)
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

int	is_dead(t_philo *philo)
{
	int	status;

	pthread_mutex_lock(&philo->data->dead_mutex);
	status = philo->data->is_dead;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (status);
}

void philo_put_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void *monitor_routine(void *arg)
{
    t_philo **philos = (t_philo **)arg;
    t_data *data = philos[0]->data;
    int i;
    long now;
    int full_count;

    while (!is_dead(philos[0]))
    {
        i = 0;
        full_count = 0;
        while (i < data->philo_count)
        {
            pthread_mutex_lock(&data->dead_mutex);
            now = timestamp_in_ms();
            if (now - philos[i]->last_meal_time > data->time_to_die)
            {
                data->is_dead = 1;
                pthread_mutex_unlock(&data->dead_mutex);
                pthread_mutex_lock(&data->print_mutex);
                printf("%lld %d died\n", now - data->start_time, philos[i]->id);
                pthread_mutex_unlock(&data->print_mutex);
                return NULL;
            }
            if (data->meals_required != -1 && philos[i]->meals_eaten >= data->meals_required)
                full_count++;
            pthread_mutex_unlock(&data->dead_mutex);
            i++;
        }
        if (data->meals_required != -1 && full_count == data->philo_count)
        {
            pthread_mutex_lock(&data->dead_mutex);
            data->is_dead = 1;
            pthread_mutex_unlock(&data->dead_mutex);
            return NULL;
        }
        usleep(100);
    }
    return NULL;
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



