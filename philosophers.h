/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yschecro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 21:10:40 by yschecro          #+#    #+#             */
/*   Updated: 2022/09/16 04:09:02 by yschecro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	s_fork
{
	int	is_used;
} t_fork;

typedef struct s_philo 
{
	int				id;
	int				n_meals;
	int				last_meal;
	pthread_t		*thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;

} t_philo;

typedef struct s_data 
{
	int				begin;
	int				n_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				max_meal;
	pthread_mutex_t	*died_mutex;
	int				died;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*output;
} t_data;

t_data *_data(void);
void	*routine(void *arg);
int		init_fork(int i);
int		init_philo(void);
int		ft_exit(void);
int		get_time(void);
int		mold_forks(void);
int		set_table(void);
void	monitor(t_philo philo, char *status);
int		deathchecker(void);

#endif
