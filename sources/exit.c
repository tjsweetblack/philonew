/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <belmiro@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 17:27:50 by badriano          #+#    #+#             */
/*   Updated: 2024/11/08 06:58:24 by badriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	free_env(t_env *env)
{
	unsigned int	i;

	if (!env)
		return ;
	if (env->forks != NULL)
		free(env->forks);
	if (env->philos != NULL)
	{
		i = 0;
		while (i < env->nb_philos)
		{
			if (env->philos[i] != NULL)
				free(env->philos[i]);
			i++;
		}
		free(env->philos);
	}
	free(env);
	return ;
}

void	destroy_mutexes(t_env *env)
{
	unsigned int	i;

	i = 0;
	while (i < env->nb_philos)
	{
		pthread_mutex_destroy(&env->forks[i]);
		pthread_mutex_destroy(&env->philos[i]->meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&env->print_lock);
	pthread_mutex_destroy(&env->sim_stop_lock);
}

int	print_msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

int	close_safely(char *str, char *details, t_env *env)
{
	if (env != NULL)
		free_env(env);
	return (print_msg(str, details, 0));
}

void	*start_close_safely(char *str, char *details, t_env *env)
{
	if (env != NULL)
		free_env(env);
	print_msg(str, details, EXIT_FAILURE);
	return (NULL);
}
