/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 21:13:23 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/16 05:20:47 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_data *_data(void)
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
	return (1);
}

int	philo_birth(t_philo *philo, int i)
{
	philo->thread = malloc(sizeof(pthread_t));
	if (!philo->thread)
		return (0);
	philo->n_meals = 0;
	philo->last_meal= 0;
	philo->id = i + 1;
//	dprintf(2, "----id of the philo is %d\n", i + 1);
	if (pthread_create(philo->thread, NULL, &routine, philo))
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
	if (!data->philos)
		return (0);
	if (!set_table())
		return (0);
	data->begin = get_time();
	while (i < data->n_philo)
	{
		usleep(300);
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

int	ft_exit(void)
{
	t_data	*data;
	int		i;

	i = 0;
	data = _data();
	while (i < data->n_philo)
	{
		free(data->philos[i].thread);
		i++;
	}
	free(data->forks);
	free(data->philos);
	free(data->output);
	return (1);
}

int	arg_init(int ac, char **av)
{
	t_data	*data;

	data = _data();
	if (ac < 5)
		return (printf("invalid arg\n"), 0);
	data->n_philo = atoi(av[1]);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	if (ac == 5)
		data->max_meal = -1;
	else
		data->max_meal = atoi(av[5]);
	return (1);
}

int	philosophers_init(int ac, char **av)
{
	t_data	*data;

	data = _data();
	if (!arg_init(ac, av))
		return (0);
	data->output = malloc(sizeof(pthread_mutex_t));
	data->died_mutex= malloc(sizeof(pthread_mutex_t));
	if (pthread_mutex_init(data->output, 0))
		return (0);
	if (pthread_mutex_init(data->died_mutex, 0))
		return (0);
	if (!data->output)
		return (0);
	if (!mold_forks())
		return (0);
	if (!init_philo())
		return (0);
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

int main(int ac, char **av)
{
	int		i;
	t_data	*data;

	data = _data();
	if (!philosophers_init(ac, av))
		return (0);
	while (1)
	{
		i = 0;
		while (i < data->n_philo)
		{
			if (data->philos[i].last_meal - data->begin >= data->time_to_die)
			{
				pthread_mutex_lock(data->died_mutex);
				data->died = 1;
				pthread_mutex_unlock(data->died_mutex);
//				pthread_mutex_unlock(data->output);
				monitor(data->philos[i], "died");
				pthread_mutex_lock(data->output);
				break ;
			}
			i++;
		}
		usleep(200);
	}
	join_philo();
	return(ft_exit());
}
