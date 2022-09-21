/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 21:13:23 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/21 17:19:55 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_data	*_data(void)
{
	static t_data	d;

	return (&d);
}

int	init_data(void)
{
	t_data	*data;

	data = _data();
	data->n_philo = 0;
	data->max_meal = 0;
	data->died = 0;
	data->n_philo_has_eaten = 0;
	return (1);
}

int	ft_exit(void)
{
	t_data	*data;
	int		i;

	i = 0;
	data = _data();
	while (i < data->n_philo)
	{
		free(data->philos[i].thread);
		pthread_mutex_destroy(data->philos[i].blackhole_mutex);
		pthread_mutex_destroy(data->philos[i].has_eaten_mutex);
		free(data->philos[i].blackhole_mutex);
		free(data->philos[i].has_eaten_mutex);
		i++;
	}
	pthread_mutex_destroy(data->forks);
	free(data->forks);
	free(data->philos);
	free(data->output);
	pthread_mutex_destroy(data->n_eaten_mutex);
	pthread_mutex_destroy(data->died_mutex);
	free(data->died_mutex);
	free(data->n_eaten_mutex);
	return (1);
}

int	join_philo(void)
{
	t_data	*data;
	int		i;

	i = 0;
	data = _data();
	while (i < data->n_philo)
	{
		pthread_join(*data->philos[i].thread, NULL);
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	if (!philosophers_init(ac, av))
		return (0);
	if (_data()->n_philo == 1)
		return (printf("0 1 is thinking\n0 1 died\n"), 0);
	loop();
	join_philo();
	return (ft_exit());
}
