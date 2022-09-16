/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 17:04:52 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/16 05:20:37 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_forks(t_philo *philo)
{
	//	dprintf(2, "id of the philo that is trying to get a fork  %d\n", philo->id);
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->l_fork);
	else
		pthread_mutex_lock(philo->r_fork);
	monitor(*philo, "has taken a fork");
	if (philo->id % 2 == 0)
	{
		if (pthread_mutex_lock(philo->r_fork))
		{
			pthread_mutex_unlock(philo->l_fork);
			return (0);
		}
		monitor(*philo, "has taken a fork");
	}
	else
	{
		if (pthread_mutex_lock(philo->l_fork))
		{
			pthread_mutex_unlock(philo->r_fork);
			return (0);
		}
		monitor(*philo, "has taken a fork");
	}
	return (1);
}

int	is_dead(void)
{
	t_data	*data;

	data = _data();
	//	return (0);
	pthread_mutex_lock(data->died_mutex);
	if (data->died)
	{
		pthread_mutex_unlock(data->died_mutex);
		return (1);
	}
	pthread_mutex_unlock(data->died_mutex);
	return (0);
}

int	waiting(int	time)
{
	int	i;

	i = 0;
	while (i < time)
	{
		if (is_dead())
			return (0);
		usleep(10);
		i += 10;
	}
	return (1);
}

void sleeping(t_philo *philo)
{
	t_data	*data;

	data = _data();
	monitor(*philo, "is sleeping");
	waiting(data->time_to_sleep * 1000);
}

int	eating(t_philo *philo)
{
	t_data	*data;

	if (get_forks(philo))
	{
		data = _data();
		if (is_dead())	
			return (0);
		philo->last_meal = get_time();
		monitor(*philo, "is eating");
		waiting(data->time_to_eat * 1000);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		philo->n_meals++;
		if (is_dead())	
			return 0 ;
		sleeping(philo);
		return (1);
	}
	return (0);
}

void	*routine(void *param)
{
	t_philo	*philo;
	t_data	*data;

	data = _data();
	philo = param;
	philo->last_meal = get_time();
	//	printf("philo %d born\n", philo->id);
	while (1)
	{
		if (is_dead())	
			break ;
		if (philo->n_meals == data->max_meal)
			break ;
		monitor(*philo, "is thinking");
		if (!eating(philo))
			break;
	}
	pthread_exit(NULL);
	return (NULL);
}
