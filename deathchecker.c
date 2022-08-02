/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deathchecker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 15:31:51 by yschecro          #+#    #+#             */
/*   Updated: 2022/08/02 16:55:58 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	deathchecker(void)
{
	int	i;
	t_data	*data;

	data = _data();
	i = 0;
	while (i < data->n_philo)
	{
		if (data->died == 1)
			return (0);
//		dprintf(2, "philo number %d, lastmeal time = %d,	 time to die = %d \n", i + 1, data->begin - data->philos[i].last_meal, data->time_to_die);
		if (data->begin - data->philos[i].last_meal > data->time_to_die)
		{
			monitor(data->philos[i], "died");
			data->died = 1;
			return (0);
		}
		i++;
	}
	return (1);
}
