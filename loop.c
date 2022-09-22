/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 16:37:45 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/22 15:50:55 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_food(t_philo *philo)
{
	t_data	*data;

	data = _data();
	pthread_mutex_lock(&data->has_eaten_mutex);
	if (philo->has_eaten == 1)
	{
		data->n_philo_has_eaten++;
		if (data->n_philo_has_eaten == data->n_philo)
		{
			pthread_mutex_unlock(&data->has_eaten_mutex);
			return (0);
		}
		philo->has_eaten = -1;
	}
	pthread_mutex_unlock(&data->has_eaten_mutex);
	return (1);
}

int	check_death(t_philo *philo)
{
	t_data	*data;

	data = _data();
	pthread_mutex_lock(&data->blackhole_mutex);
	if ((get_time() - data->begin) > philo->blackhole)
	{
		pthread_mutex_unlock(&data->blackhole_mutex);
		monitor(philo, "died");
		pthread_mutex_lock(&data->died_mutex);
		data->died = 1;
		pthread_mutex_unlock(&data->died_mutex);
		return (0);
	}
	pthread_mutex_unlock(&data->blackhole_mutex);
	return (1);
}

int	loop(void)
{
	int		i;
	t_data	*data;

	data = _data();
	i = 0;
	while (1)
	{
		i = 0;
		while (i < data->n_philo)
		{
			if (!check_food(&data->philos[i]))
				return (pthread_mutex_lock(&data->output), 0);
			if (!check_death(&data->philos[i]))
				return (pthread_mutex_lock(&data->output), 0);
			i++;
		}
	}
}
