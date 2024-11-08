/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <belmiro@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 12:00:18 by badriano          #+#    #+#             */
/*   Updated: 2024/11/08 06:58:24 by badriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	set_sim_flag(t_env *env, bool state)
{
	pthread_mutex_lock(&env->sim_stop_lock);
	env->sim_stop = state;
	pthread_mutex_unlock(&env->sim_stop_lock);
}

bool	is_sim_running(t_env *env)
{
	bool	r;

	r = false;
	pthread_mutex_lock(&env->sim_stop_lock);
	if (env->sim_stop == true)
		r = true;
	pthread_mutex_unlock(&env->sim_stop_lock);
	return (r);
}

static bool	stop_philo(t_philo *philo)
{
	time_t	time;

	time = get_time_in_ms();
	if ((time - philo->last_meal) >= philo->env->time_to_die)
	{
		set_sim_flag(philo->env, true);
		write_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

static bool	should_philo_stop(t_env *env)
{
	unsigned int	i;
	bool			all_ate_enough;

	all_ate_enough = true;
	i = 0;
	while (i < env->nb_philos)
	{
		pthread_mutex_lock(&env->philos[i]->meal_time_lock);
		if (stop_philo(env->philos[i]))
			return (true);
		if (env->must_eat_count != -1)
			if (env->philos[i]->times_ate
				< (unsigned int)env->must_eat_count)
				all_ate_enough = false;
		pthread_mutex_unlock(&env->philos[i]->meal_time_lock);
		i++;
	}
	if (env->must_eat_count != -1 && all_ate_enough == true)
	{
		set_sim_flag(env, true);
		return (true);
	}
	return (false);
}

void	*monitor_thread(void *data)
{
	t_env			*env;

	env = (t_env *)data;
	if (env->must_eat_count == 0)
		return (NULL);
	set_sim_flag(env, false);
	delay_start(env->start_time);
	while (true)
	{
		if (should_philo_stop(env) == true)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
