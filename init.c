/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jalqam <jalqam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:52:57 by jalqam            #+#    #+#             */
/*   Updated: 2025/06/26 18:10:40 by jalqam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
int	sleeping(t_philo *philo);
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
	data->is_dead = 0;
	int i=0;
	while(i<data->philo_count)
	{
		if(pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			free(data->forks);
			free(data);
			return (NULL);
		}
		i++;
	}
	if(pthread_mutex_init(&data->dead_mutex, NULL) != 0)
	{
		free(data->forks);
		free(data);
		return (NULL);
	}
	if(pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		free(data->forks);
		free(data);
		return (NULL);
	}
	return (data);
}

t_philo **init_philosophers(t_data *data, t_philo **philos)
{
	int i;

	i = 0;
	while (i < data->philo_count)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			return (NULL);
		philos[i]->id = i + 1;
		philos[i]->meals_eaten = 0;
		philos[i]->last_meal_time = 0;
		philos[i]->data = data;
		philos[i]->is_dead = &data->is_dead;
		philos[i]->left_fork = &data->forks[i];
		if (i == data->philo_count - 1)
			philos[i]->right_fork = &data->forks[0];
		else
			philos[i]->right_fork = &data->forks[i + 1];
		i++;
	}
	return (philos);
}

// void *philo_routine(void *arg)
// {
//     t_philo *philo = (t_philo *)arg;  
// 	long current_time;
// 	while(1)
// 	{
// 		current_time = timestamp_in_ms();
// 		printf("%ld Philo %d is thinking.\n", current_time, philo->id);
// 		printf("%ld Philo %d is eating.\n", current_time, philo->id);
// 		printf("%ld Philo %d is sleeping.\n", current_time, philo->id);
// 	}
// 	return NULL;
// }
void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    while (1)
	{
		if(is_dead(philo))
		{
			printf("hello\n");
			return (NULL);
		}
        if(philo_eating(philo))
			return (NULL);
		if(is_dead(philo))
		{
			printf("hello\n");
			return (NULL);
		}
		if (sleeping(philo))
		{
			printf("hello\n");
			return (NULL);
		}
		if(is_dead(philo))
		{
			printf("hello\n");
			return (NULL);
		}
		// if(is_dead(philo))
        print_action(philo, "is thinking.");
		// 	return (NULL);
		if(is_dead(philo))
		{
			// pthread_mutex_lock(&philo->data->print_mutex);
			printf("Philosopher %d is dead.\n", philo->id);	
			// pthread_mutex_unlock(&philo->data->print_mutex);
			return (NULL);
		}
		//pthread_mutex_lock(&philo->data->death_mutex);
    }
    return NULL;
}

void init_threads(t_data *data, t_philo **philos)
{
	pthread_t monitor_thread;
	int i;
	
	i = 0;
	
	while (i < data->philo_count)
	{
		if(pthread_create(&philos[i]->thread,NULL, philo_routine, philos[i]) != 0)
		{
			perror("Failed to create thread");
		}	
		i++;
	}
	i = 0;
	pthread_create(&monitor_thread, NULL, monitor_routine, data);
	while(i < data->philo_count)
	{
    	if(pthread_join(philos[i]->thread, NULL) != 0)
    	{
        	perror("Failed to join thread");
    	}
    	i++;
	}
    pthread_join(monitor_thread, NULL);
}
int	sleeping(t_philo *philo)
{
	if(is_dead(philo))
	{
		// printf("hello\n");
		return (1);
	}
	print_action(philo, "is sleeping\n");
	usleep(philo->data->time_to_sleep * 1000);//
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
	pthread_mutex_lock(&philo->data->print_mutex);
	if(*philo->is_dead)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		pthread_mutex_unlock(&philo->data->dead_mutex);
		return;
	}	
	printf("%s\n", action);
	current_time = timestamp_in_ms();
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
	if(is_dead(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

int philo_eating(t_philo *philo)
{
	long current_time;
	
	if(is_dead(philo))
	{
		return(1);
	}
	philo_taking_forks(philo);
	if(is_dead(philo))
	{
		return(1);
	}
	print_action(philo, "is eating.");
	pthread_mutex_lock(&philo->data->dead_mutex);
	current_time = timestamp_in_ms();
	philo->last_meal_time = current_time;
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->dead_mutex);
	usleep(philo->data->time_to_eat * 1000);//
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
	
}

int	is_dead(t_philo *philo)
{
	int	status;

	pthread_mutex_lock(&philo->data->dead_mutex);
	status = *(philo->is_dead);
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
    t_data *data = (t_data *)arg;
    int i = 0;
    long now;

    while (!is_dead(data->philo[0]))
    {
		i = 0;
        while (i < data->philo_count)
        {
			pthread_mutex_lock(&data->dead_mutex);
            now = timestamp_in_ms();
            if (now - data->philo[i]->last_meal_time >= data->time_to_die)
            {
                data->is_dead = 1;
                // print_action(data->philo[i], "has died");
                pthread_mutex_unlock(&data->dead_mutex);
                return NULL;
            }
            pthread_mutex_unlock(&data->dead_mutex);
            i++;
        }
        usleep(100);
    }
    return NULL;
}

// -----------------------------------





// void philo_sleep(t_philo *philo)
// {
//     long current_time = timestamp_in_ms();
//     printf("%ld %d is sleeping.\n", current_time, philo->id);
//     usleep(philo->data->time_to_sleep * 1000);
// }

// void *philo_routine(void *arg)
// {
//     t_philo *philo = (t_philo *)arg;
//     while(1)
//     {
//         philo_think(philo);
//         philo_take_forks(philo);
//         philo_eat(philo);
//         philo_put_forks(philo);
//         philo_sleep(philo);
//     }
//     return NULL;
// }

// void init_threads(t_data *data, t_philo **philos)
// {
// 	int i;
	
// 	i = 0;
// 	while (i < data->philo_count)
// 	{
// 		if(pthread_create(&philos[i]->thread,NULL, philo_routine, philos[i]) != 0)
// 		{
// 			perror("Failed to create thread");
// 			exit(EXIT_FAILURE);
// 		}	
// 		i++;
// 	}
// 	i = 0;
// 	while(i < data->philo_count)
// 	{
//     	if(pthread_join(philos[i]->thread, NULL) != 0)
//     	{
//         	perror("Failed to join thread");
//         	exit(EXIT_FAILURE);
//     	}
//     	i++;
// 	}
// }
