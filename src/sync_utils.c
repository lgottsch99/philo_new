//HEADER
#include "../includes/philos.h"

//DOESNT WORK ON MAC
void		wait_all_ready(t_program *program) //spinlock, runnning until bool set to true
{
	while (!get_bool(&program->program_mutex, &program->all_threads_ready))
		;
}



//precise usleep (real one apparently sucks), also checking if sim finished while sleeping
void	precise_usleep(long time, t_program *program)
{
	long	start;
	long	elapsed;
	long	remain;

	start = get_time_ms();
	while(get_time_ms() - time < time)
	{
		if (sim_finished(program))
			break ;
		elapsed = get_time_ms() - start;
		remain = time - elapsed;
		if (remain > 1e3) //1 ms threshold
			usleep(remain / 2);
		else //spinlock
		{
			while (get_time_ms() - time < time)
				;
		}
	}

}

/* make monitor thread wait until all philos run
*/
bool	all_threads_running(t_program *program)//not same as oceano, check if ok
{
	int	counter_value;

	handle_mutex(&program->program_mutex, LOCK);
	counter_value = program->running_philos;
	handle_mutex(&program->program_mutex, UNLOCK);
	if (counter_value == program->num_philos)	
		return (true);
	else
		return (false);
}

void add_program_counter(pthread_mutex_t *mutex, int *counter)
{
	handle_mutex(mutex, LOCK);
	*counter = *counter + 1;
	handle_mutex(mutex, UNLOCK);
}

/* make the system FAIR even if num philos uneven (sb might not think at all)*/
// void	desync_philos(t_philo *philo)
// {
// 	if (philo->program_ptr->num_philos % 2 == 0)//even philo num
// 	{
// 		if ()
// 	}
// 	else //uneven philo num
// 	{
// 		if (philo->num % 2)
// 			think()
// 	}
// }