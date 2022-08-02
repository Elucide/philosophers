/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 17:04:52 by yschecro          #+#    #+#             */
/*   Updated: 2022/08/02 17:11:01 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


int	get_forks(t_philo philo)
{
	if (!deathchecker())
		return (0);
	pthread_mutex_lock(philo.l_fork);
	monitor(philo, "has taken a fork");
	pthread_mutex_lock(philo.r_fork);
	monitor(philo, "has taken a fork");
	return (1);
}

void	*routine(void *param)
{
	t_philo	*philo;
	t_data	*data;

	data = _data();
	philo = param;
	philo->last_meal = get_time();
	while (1)
	{
//		dprintf(2, "\nphilo : %d   trying first deathcheck\n\n", philo->id);
		if (!deathchecker())
			break ;
//		dprintf(2, "\nphilo : %d   first deathcheck passed\n\n", philo->id);
		if (philo->n_meals == data->max_meal)
			break ;
		monitor(*philo, "is thinking");
		if (get_forks(*philo))
		{
			if (!deathchecker())
				break ;
			monitor(*philo, "is eating");
			usleep(data->time_to_eat);
			if (!deathchecker())
				break ;
			pthread_mutex_unlock(philo->l_fork);
			pthread_mutex_unlock(philo->r_fork);
			philo->n_meals++;
			philo->last_meal = get_time();
			if (!deathchecker())
				break ;
			monitor(*philo, "is sleeping");
			usleep(data->time_to_sleep);
			if (!deathchecker())
				break ;
		}
	}
	pthread_exit(NULL);
	return (NULL);
}
