/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:23:03 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 15:38:19 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
#define PHILOS_H

#include <pthread.h>
#include <stdio.h>	//printf
#include <stdlib.h> //malloc, free
#include <unistd.h> //write, usleep
#include <string.h> //memset
#include <sys/time.h> //gettimeofday
#include <stdbool.h>

//time to wait  before starting sim, to allow all philos to be created
# define OFFSET_TIME 2000

typedef struct s_program t_program;

//code readablility

//opcode for mutex/thread fts
typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DETACH,//threads only
	JOIN, //threads only
	DESTROY,
	CREATE //threads only
}	t_opcode;

//philo states
typedef enum e_state
{
	TAKE_FORK_1,
	TAKE_FORK_2,
	EATING,
	SLEEPING,
	THINKING,
	DEAD
}	t_philo_state;

//STRUCTS
typedef struct s_philo
{
	pthread_t		thread;
	int				num;
	int				times_eaten;
	bool			full;
	long			philo_start;
	long			end_last_meal;
	pthread_mutex_t	*mutex_own_fork;
	pthread_mutex_t	*mutex_fork_right;
	pthread_mutex_t	philo_mutex; //for races with monitor
	t_program		*program_ptr;

	int				time_die;
	int				time_eat;
	int				time_sleep;



	// pthread_mutex_t	mutex_times_eaten; //to lock times eaten variable
	// pthread_mutex_t	mutex_end_last_meal;
	// pthread_mutex_t	*write_lock;
	// pthread_mutex_t	*mutex_dead_flag;
	//...
}	t_philo;

typedef struct s_program
{
	int					num_philos;
	bool				end_sim;
	int					time_die;
	int					time_eat;
	int					time_sleep;
	int					times_to_eat; //-1 if not given as cl arg
	bool				all_threads_ready; //sync philos
	pthread_mutex_t		program_mutex; //avoid races while reading from program struct
	pthread_mutex_t		write_mutex; //used???? yes
	long				start_time; //saving start time of prorgram in millisec
	// long				philo_start;
	pthread_mutex_t		*fork_mutexes; //as many as forks, match by index?
	t_philo 			**philos;//array of ptrs to philo structs
	pthread_t			monitor;
	int					running_philos;
	
}	t_program;


//main
void	*routine(void *arg);
int		init_program(t_program *program, char *argv[]);

//input
int		check_input_valid(int argc, char *argv[]);

//simulation
void	start_sim(t_program *program);

//sim helers
int		lock_forks(t_philo *philo);
int		unlock_forks(t_philo *philo);

//monitor
void *monitor(void *data);

//routine
void	think(t_philo *philo, int i);
int		eat(t_philo *philo);
void	*routine(void *data);

//utils
int		ft_atoi(const char *nptr);
long	get_time_ms(void);
int	handle_mutex(pthread_mutex_t *mutex, t_opcode opcode);
int		log_status(t_philo_state status, t_philo *philo);


//sync utils
void		wait_all_ready(t_program *program); //spinlock, runnning until bool set to true
// void	precise_usleep(long time, t_program *program);
int		add_program_counter(pthread_mutex_t *mutex, int *counter);
bool	all_threads_running(t_program *program);//not same as oceano, check if ok
// int	ft_usleep(size_t milliseconds);
// size_t	get_current_time(void);

void precise_usleep(long usec);


//get set
int		get_int(pthread_mutex_t *mutex, int *value);
int		set_int(pthread_mutex_t *mutex, int value, int *dest);
bool	get_bool(pthread_mutex_t *mutex, bool *value);
int		set_bool(pthread_mutex_t *mutex, bool value, bool *dest);
bool 	sim_finished(t_program *program);
int		set_long(pthread_mutex_t *mutex, long value, long *dest);
long	get_long(pthread_mutex_t *mutex, long *value);

void	print_philo(t_philo *philo);//rm

//lonely 
void 	*lonely(void *data);


//free
void	free_program(t_program *program);
void	free_philo(t_philo *philo);


//mac
void my_sleep(int milliseconds);


#endif