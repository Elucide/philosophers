/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 21:10:40 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/22 13:08:31 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_fork
{
	int	is_used;
}	t_fork;

typedef struct s_philo
{
	int				id;
	int				n_meals;
	pthread_mutex_t	blackhole_mutex;
	int				blackhole;
	pthread_mutex_t	has_eaten_mutex;
	int				has_eaten;
	pthread_t		*thread;
	pthread_mutex_t	l_fork;
	pthread_mutex_t	r_fork;

}	t_philo;

typedef struct s_data
{
	int				begin;
	int				n_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				max_meal;
	int				n_philo_has_eaten;
	pthread_mutex_t	died_mutex;
	pthread_mutex_t	n_eaten_mutex;
	int				died;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	output;
}	t_data;

t_data	*_data(void);
void	*routine(void *arg);
int		init_fork(int i);
int		init_philo(void);
int		ft_exit(void);
int		get_time(void);
int		ft_atoi(char *str);
int		mold_forks(void);
int		is_dead(void);
int		set_table(void);
void	monitor(t_philo philo, char *status);
int		deathchecker(void);
int		arg_init(int ac, char **av);
int		philosophers_init(int ac, char **av);
int		philo_birth(t_philo *philo, int i);
int		loop(void);

#endif
