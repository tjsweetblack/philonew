/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <belmiro@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 11:46:09 by badriano          #+#    #+#             */
/*   Updated: 2024/11/08 06:58:43 by badriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static bool	start_simulation(t_env *env)
{
	unsigned int	i;

	env->start_time = get_time_in_ms() + (env->nb_philos * 2 * 10);
	i = 0;
	while (i < env->nb_philos)
	{
		if (pthread_create(&env->philos[i]->thread, NULL,
				&philosopher, env->philos[i]) != 0)
			return (close_safely(STR_ERR_THREAD, NULL, env));
		i++;
	}
	if (env->nb_philos > 1)
	{
		if (pthread_create(&env->monitor, NULL,
				&monitor_thread, env) != 0)
			return (close_safely(STR_ERR_THREAD, NULL, env));
	}
	return (true);
}

static void	end_simulation(t_env	*env)
{
	unsigned int	i;

	i = 0;
	while (i < env->nb_philos)
	{
		pthread_join(env->philos[i]->thread, NULL);
		i++;
	}
	if (env->nb_philos > 1)
		pthread_join(env->monitor, NULL);
	destroy_mutexes(env);
	free_env(env);
}

int	main(int ac, char **av)
{
	t_env	*env;

	env = malloc(sizeof(t_env) * 1);
	if (ac - 1 < 4 || ac - 1 > 5)
		return (print_msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!check_arguments(ac, av))
	{
		free(env);
		return (EXIT_FAILURE);
	}
	init_struct(ac, av, 1, env);
	if (!env)
		return (EXIT_FAILURE);
	if (!start_simulation(env))
		return (EXIT_FAILURE);
	end_simulation(env);
	return (EXIT_SUCCESS);
}
