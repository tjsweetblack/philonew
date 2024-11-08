/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <belmiro@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:35:04 by badriano          #+#    #+#             */
/*   Updated: 2024/11/08 09:38:30 by badriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static pthread_mutex_t	*init_forks(t_env *env)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * env->nb_philos);
	if (!forks)
		return (start_close_safely(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < env->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (start_close_safely(STR_ERR_MUTEX, NULL, 0));
		i++;
	}
	return (forks);
}

static void	take_forks(t_philo *philo)
{
	philo->left_fork = philo->id;
	philo->right_fork = (philo->id + 1) % philo->env->nb_philos;
	if (philo->id % 2)
	{
		philo->left_fork = (philo->id + 1) % philo->env->nb_philos;
		philo->right_fork = philo->id;
	}
}

static t_philo	**init_philosophers(t_env *env)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * env->nb_philos);
	if (!philos)
		return (start_close_safely(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < env->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (start_close_safely(STR_ERR_MALLOC, NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (start_close_safely(STR_ERR_MUTEX, NULL, 0));
		philos[i]->env = env;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		philos[i]->last_meal = 0;
		philos[i]->total_eat = env->must_eat_count;
		take_forks(philos[i]);
		i++;
	}
	return (philos);
}

static bool	init_global_mutexes(t_env *env)
{
	env->forks = init_forks(env);
	if (!env->forks)
		return (false);
	if (pthread_mutex_init(&env->sim_stop_lock, 0) != 0)
		return (close_safely(STR_ERR_MUTEX, NULL, env));
	if (pthread_mutex_init(&env->print_lock, 0) != 0)
		return (close_safely(STR_ERR_MUTEX, NULL, env));
	return (true);
}

void	init_struct(int ac, char **av, int i, t_env *env)
{
	if (!env)
	{
		start_close_safely(STR_ERR_MALLOC, NULL, 0);
		return ;
	}
	env->nb_philos = atoi_digits(av[i++]);
	env->time_to_die = atoi_digits(av[i++]);
	env->time_to_eat = atoi_digits(av[i++]);
	env->time_to_sleep = atoi_digits(av[i++]);
	env->must_eat_count = -1;
	if (ac - 1 == 5)
		env->must_eat_count = atoi_digits(av[i]);
	env->philos = init_philosophers(env);
	if (!env->philos)
		return ;
	if (!init_global_mutexes(env))
		return ;
	env->sim_stop = false;
}
