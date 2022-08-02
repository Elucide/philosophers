/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 21:13:23 by yschecro          #+#    #+#             */
/*   Updated: 2022/08/02 16:55:30 by yschecro         ###   ########.fr       */
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
		data->philos[i].thread = malloc(sizeof(pthread_t));
		data->philos[i].n_meals = 0;
//		dprintf(1, "%d mother pregnant\n", i + 1);
		data->philos[i].id = i + 1;
		if (pthread_create(data->philos[i].thread, NULL, routine, &data->philos[i]))
			return (0);
//		dprintf(1, "%d philos born\n", i + 1);
		i++;
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

int main(int ac, char **av)
{
	int		i;
	t_data	*data;

	data = _data();
	if (ac < 5)
		return (printf("invalid arg\n"));
	data->n_philo = atoi(av[1]);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	if (ac == 5)
		data->max_meal = -1;
	else
		data->max_meal = atoi(av[5]);
	i = 0;
	data->output = malloc(sizeof(pthread_mutex_t));
	if (!data->output)
		return (0);
	if (!mold_forks())
		return (0);
	if (!init_philo())
		return (0);
	if (pthread_mutex_init(data->output, 0))
		return (0);
	while (i < data->n_philo)
	{
		pthread_join(*data->philos[i].thread, NULL);
		i++;
	}
	return(ft_exit());
}
