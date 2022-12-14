/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 16:30:10 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/22 16:15:40 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philosophers_init(int ac, char **av)
{
	t_data	*data;

	data = _data();
	if (!arg_init(ac, av))
		return (0);
	if (data->n_philo == 1)
		return (1);
	if (pthread_mutex_init(&data->output, 0))
		return (0);
	if (pthread_mutex_init(&data->died_mutex, 0))
		return (0);
	if (pthread_mutex_init(&data->n_eaten_mutex, 0))
		return (0);
	if (pthread_mutex_init(&data->blackhole_mutex, 0))
		return (0);
	if (pthread_mutex_init(&data->has_eaten_mutex, 0))
		return (0);
	if (!mold_forks())
		return (0);
	if (!init_philo())
		return (0);
	return (1);
}

int	init_philo(void)
{
	t_data	*data;
	int		i;

	data = _data();
	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->n_philo);
	if (!data->philos || !set_table())
		return (0);
	data->begin = get_time();
	while (i < data->n_philo)
	{
		usleep(100);
		if (!philo_birth(&data->philos[i], i))
			return (0);
		i += 2;
	}
	i = 1;
	while (i < data->n_philo)
	{
		if (!philo_birth(&data->philos[i], i))
			return (0);
		i += 2;
	}
	return (1);
}

int	philo_birth(t_philo *philo, int i)
{
	philo->thread = malloc(sizeof(pthread_t));
	if (!philo->thread)
		return (0);
	philo->n_meals = 0;
	philo->has_eaten = 0;
	philo->blackhole = _data()->time_to_die;
	philo->id = i + 1;
	if (pthread_create(philo->thread, NULL, &routine, philo))
		return (0);
	return (1);
}

int	get_time_to_sleep(void)
{
	t_data	*data;

	data = _data();
	if (data->time_to_sleep < data->time_to_eat && data->n_philo % 2)
		return ((data->time_to_eat - data->time_to_sleep) * 1000 + 1);
	else
		return (1);
}
