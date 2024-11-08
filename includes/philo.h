/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badriano <belmiro@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 11:46:01 by badriano          #+#    #+#             */
/*   Updated: 2024/11/08 09:36:32 by badriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

# define MAX_PHILOS	250
# define STR_MAX_PHILOS "250"

# define STR_PROG_NAME	"philo:"
# define STR_USAGE	"%s usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid unsigned integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"

typedef struct s_philo	t_philo;

typedef struct s_env
{
	time_t			start_time;
	unsigned int	nb_philos;
	pthread_t		monitor;
	int				must_eat_count;
	bool			sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
	t_philo			**philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
}	t_env;

typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		total_eat;
	unsigned int		times_ate;
	unsigned int		right_fork;
	unsigned int		left_fork;
	pthread_mutex_t		meal_time_lock;
	time_t				last_meal;
	t_env				*env;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK_1 = 4,
	GOT_FORK_2 = 5
}	t_status;

int				check_arguments(int ac, char **av);
int				atoi_digits(char *str);
void			init_struct(int ac, char **av, int i, t_env *table);
void			*philosopher(void *data);
time_t			get_time_in_ms(void);
void			philo_sleep(t_env *table, time_t sleep_time);
void			delay_start(time_t start_time);
void			write_status(t_philo *philo, bool reaper, t_status status);
void			*start_close_safely(char *str, char *details, t_env *table);
int				print_msg(char *str, char *detail, int exit_no);
void			*monitor_thread(void *data);
bool			is_sim_running(t_env *table);
int				close_safely(char *str, char *details, t_env *table);
void			free_env(t_env *table);
void			destroy_mutexes(t_env *table);
#endif