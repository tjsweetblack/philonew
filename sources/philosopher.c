/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <belmiro@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:12:00 by badriano          #+#    #+#             */
/*   Updated: 2024/11/08 10:07:29 by badriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->env->forks[philo->left_fork]);
	write_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->env->forks[philo->right_fork]);
	write_status(philo, false, GOT_FORK_2);
	write_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	philo->total_eat -= 1;
	pthread_mutex_unlock(&philo->meal_time_lock);
	philo_sleep(philo->env, philo->env->time_to_eat);
	if (is_sim_running(philo->env) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	write_status(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->env->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->env->forks[philo->left_fork]);
	philo_sleep(philo->env, philo->env->time_to_sleep);
}

static void	think_process(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_to_think = (philo->env->time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->env->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		write_status(philo, false, THINKING);
	philo_sleep(philo->env, time_to_think);
}

static void	*case_one(t_philo *philo)
{
	pthread_mutex_lock(&philo->env->forks[philo->left_fork]);
	write_status(philo, false, GOT_FORK_1);
	philo_sleep(philo->env, philo->env->time_to_die);
	write_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->env->forks[philo->left_fork]);
	return (NULL);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->env->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->env->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	delay_start(philo->env->start_time);
	if (philo->env->time_to_die == 0)
		return (NULL);
	if (philo->env->nb_philos == 1)
		return (case_one(philo));
	else if (philo->id % 2)
		think_process(philo, true);
	while (is_sim_running(philo->env) == false)
	{
		philo_routine(philo);
		if (philo->total_eat == 0)
			break;
		think_process(philo, false);
		
	}
	return (NULL);
}
