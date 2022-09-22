/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 18:22:44 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/21 17:12:40 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_fork(int i)
{
	t_data	*data;

	data = _data();
	if (i == 0)
	{
		if (!pthread_mutex_init(data->philos[i].l_fork, 0))
			return (0);
		if (!pthread_mutex_init(data->philos[i].r_fork, 0))
			return (0);
	}
	else if (i == data->n_philo - 1)
	{
		data->philos[i].l_fork = data->philos[i - 1].r_fork;
		data->philos[i].r_fork = data->philos[0].l_fork;
	}
	else
	{
		data->philos[i].l_fork = data->philos[i - 1].r_fork;
		if (!pthread_mutex_init(data->philos[i].r_fork, 0))
			return (0);
	}
	return (1);
}

int	mold_forks(void)
{
	t_data	*data;
	int		i;

	i = 0;
	data = _data();
	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philo);
	if (!data->forks)
		return (0);
	while (i < data->n_philo)
	{
		if (pthread_mutex_init(&data->forks[i], 0))
			return (0);
		i++;
	}
	return (1);
}

int	set_table(void)
{
	t_data	*data;
	int		i;

	i = 0;
	data = _data();
	while (i < data->n_philo)
	{
		data->philos[i].l_fork = &data->forks[i];
		i++;
	}
	i = 0;
	while (i < data->n_philo - 1)
	{
		data->philos[i].r_fork = data->philos[i + 1].l_fork;
		i++;
	}
	data->philos[i].r_fork = data->philos[0].l_fork;
	return (1);
}
