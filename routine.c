/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 17:04:52 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/22 15:54:01 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_dead(void)
{
	t_data	*data;

	data = _data();
	pthread_mutex_lock(&data->has_eaten_mutex);
	if (data->n_philo_has_eaten == data->n_philo)
	{
		pthread_mutex_unlock(&data->has_eaten_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->has_eaten_mutex);
	pthread_mutex_lock(&data->died_mutex);
	if (data->died)
	{
		pthread_mutex_unlock(&data->died_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->died_mutex);
	return (0);
}

int	sleeping(t_philo *philo)
{
	t_data	*data;

	data = _data();
	if (is_dead())
		return (0);
	monitor(philo, "is sleeping");
	if (!waiting(data->time_to_sleep * 1000))
		return (0);
	return (1);
}

int	eating(t_philo *philo)
{
	t_data	*data;

	data = _data();
	pthread_mutex_lock(&data->blackhole_mutex);
	philo->blackhole = (get_time() - data->begin) + data->time_to_die;
	pthread_mutex_unlock(&data->blackhole_mutex);
	philo->n_meals++;
	monitor(philo, "is eating");
	pthread_mutex_lock(&data->has_eaten_mutex);
	if (philo->n_meals == data->max_meal && philo->has_eaten == 0)
		philo->has_eaten = 1;
	pthread_mutex_unlock(&data->has_eaten_mutex);
	if (!waiting(data->time_to_eat * 1000))
		return (unlock_forks(philo), 0);
	unlock_forks(philo);
	if (is_dead())
		return (0);
	if (!waiting(200))
		return (0);
	return (1);
}

int	thinking(t_philo *philo)
{
	t_data	*data;

	data = _data();
	if (is_dead())
		return (0);
	monitor(philo, "is thinking");
	if (data->time_to_sleep < data->time_to_eat)
	{
		waiting((data->time_to_eat - data->time_to_sleep) * 1000 + 1);
	}
	else
		waiting(1);
	if (is_dead())
		return (0);
	return (1);
}

void	*routine(void *param)
{
	t_philo	*philo;
	t_data	*data;

	data = _data();
	philo = param;
	philo->blackhole = data->time_to_die;
	while (!is_dead())
	{
		if (!thinking(philo))
			break ;
		while (!get_forks(philo) && !is_dead())
			usleep(10);
		if (is_dead())
		{
			unlock_forks(philo);
			break ;
		}
		if (!eating(philo))
			break ;
		if (!sleeping(philo))
			break ;
	}
	pthread_exit(NULL);
	return (NULL);
}
