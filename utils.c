/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 18:33:55 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/22 15:53:24 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_time(void)
{
	static struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	monitor(t_philo *philo, char *status)
{
	t_data	*data;

	data = _data();
	if (is_dead())
		return ;
	pthread_mutex_lock(&data->output);
	if (is_dead())
	{
		pthread_mutex_unlock(&data->output);
		return ;
	}
	printf("%d %d %s\n", get_time() - data->begin, philo->id, status);
	pthread_mutex_unlock(&data->output);
}

int	waiting(int time)
{
	int	begin;

	begin = get_time() * 1000;
	while (get_time() * 1000 - begin < time)
	{
		if (is_dead())
			return (0);
		usleep(50);
	}
	return (1);
}
