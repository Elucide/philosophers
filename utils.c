/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 18:33:55 by yschecro          #+#    #+#             */
/*   Updated: 2022/08/02 15:19:31 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_time(void)
{
	static struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	monitor(t_philo philo, char *status)
{
	t_data	*data;

	data = _data();
	pthread_mutex_lock(data->output);
	printf("%d %d %s\n", get_time() - data->begin, philo.id, status);
	pthread_mutex_unlock(data->output);
}
