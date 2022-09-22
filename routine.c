/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 17:04:52 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/22 13:06:15 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		if (pthread_mutex_lock(&philo->l_fork) || is_dead())
			return (0);
		if (pthread_mutex_lock(&philo->r_fork))
		{
			pthread_mutex_unlock(&philo->l_fork);
			return (0);
		}
		monitor(*philo, "has taken a fork");
	}
	else
	{
		if (pthread_mutex_lock(&philo->r_fork) || is_dead())
			return (0);
		if (pthread_mutex_lock(&philo->l_fork))
		{
			pthread_mutex_unlock(&philo->r_fork);
			return (0);
		}
		monitor(*philo, "has taken a fork");
	}
	if (is_dead())
		return (0);
	dprintf(2, "philo %d forks locked\n", philo->id);
	return (1);
}

int	unlock_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->r_fork);
		pthread_mutex_unlock(&philo->l_fork);
	}
	else
	{
		pthread_mutex_unlock(&philo->l_fork);
		pthread_mutex_unlock(&philo->r_fork);
	}
	dprintf(2, "philo %d forks unlocked\n", philo->id);
	return (0);
}

int	is_dead(void)
{
	t_data	*data;

	data = _data();
	pthread_mutex_lock(&data->n_eaten_mutex);
	if (data->n_philo_has_eaten == data->n_philo)
	{
		pthread_mutex_unlock(&data->n_eaten_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->n_eaten_mutex);
	pthread_mutex_lock(&data->died_mutex);
	if (data->died)
	{
		pthread_mutex_unlock(&data->died_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->died_mutex);
	return (0);
}

int	waiting(int time)
{
	int	begin;

	begin = get_time() * 1000;
	while (get_time() * 1000 - begin < time)
	{
		if (is_dead())
			return (0);
		usleep(100);
	}
	return (1);
}

int	sleeping(t_philo *philo)
{
	t_data	*data;

	data = _data();
	monitor(*philo, "is sleeping");
	if (!waiting(data->time_to_sleep * 1000))
		return (0);
	return (1);
}

int	eating(t_philo *philo)
{
	t_data	*data;

	data = _data();
	pthread_mutex_lock(&philo->blackhole_mutex);
	philo->blackhole = (get_time() - data->begin) + data->time_to_die;
	monitor(*philo, "is eating");
	pthread_mutex_unlock(&philo->blackhole_mutex);
	if (!waiting(data->time_to_eat * 1000))
		return (unlock_forks(philo), 0);
	unlock_forks(philo);
	philo->n_meals++;
	pthread_mutex_lock(&philo->has_eaten_mutex);
	if (philo->n_meals == data->max_meal && philo->has_eaten == 0)
		philo->has_eaten = 1;
	pthread_mutex_unlock(&philo->has_eaten_mutex);
	if (is_dead())
		return (0);
	if (!waiting(200))
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
		if (is_dead())
			break ;
		monitor(*philo, "is thinking");
		if (is_dead())
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
