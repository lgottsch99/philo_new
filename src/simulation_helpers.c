//HEADER
#include "../includes/philos.h"

void	lock_forks(t_philo *philo)
{
	if (philo->num % 2 == 0)//if even grab own fork first
	{
		handle_mutex(philo->mutex_own_fork, LOCK);
		log_status(TAKE_FORK_1, philo);
		handle_mutex(philo->mutex_fork_right, LOCK);
		log_status(TAKE_FORK_2, philo);
	}
	else //uneven grab right fork first
	{
		handle_mutex(philo->mutex_fork_right, LOCK);
		log_status(TAKE_FORK_2, philo);
		handle_mutex(philo->mutex_own_fork, LOCK);
		log_status(TAKE_FORK_1, philo);
	}
}

void	unlock_forks(t_philo *philo)
{
	handle_mutex(philo->mutex_own_fork, UNLOCK);
	handle_mutex(philo->mutex_fork_right, UNLOCK);
}
