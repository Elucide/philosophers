/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 20:38:19 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/22 19:02:57 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_atoi(char *str)
{
	int	i;
	int	out;
	int	signe;

	i = 0;
	out = 0;
	signe = 1;
	if (str[i] == '-')
	{
		signe = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		out = out * 10 + str[i] - '0';
		i++;
	}
	while (str[i])
		return (0);
	return (out * signe);
}

int	check_arg(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] < '0' || str[i] > '9') || str[i] != '-' || str[i] != '+')
			return (0);
		i++;
	}
	return (1);
}

int	arg_init(int ac, char **av)
{
	t_data	*data;

	data = _data();
	if (ac < 5 || ac > 6)
		return (printf("invalid arg\n"), 0);
	data->n_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->max_meal = 1;
	if (ac == 6)
		data->max_meal = ft_atoi(av[5]);
	if (data->time_to_die >= 1000000 || data->time_to_sleep >= 1000000 || \
			data->time_to_eat >= 1000000 || data->n_philo <= 0 || \
			data->max_meal <= 0 || data->time_to_die <= 60 || \
			data->time_to_eat <= 60 || data->time_to_sleep <= 60 || \
			data->n_philo > 200)
		return (printf("invalid arg\n"), 0);
	if (ac == 5)
		data->max_meal = -1;
	data->time_to_wait = get_time_to_sleep();
	return (1);
}
