
#include "../includes/philos.h"

//wrappers to shorten main code
int	get_int(pthread_mutex_t *mutex, int *value)
{
	int	ret;

	handle_mutex(mutex, LOCK);
	ret = *value;
	handle_mutex(mutex, UNLOCK);
	return (ret);
}

void	set_int(pthread_mutex_t *mutex, int value, int *dest)
{
	handle_mutex(mutex, LOCK);
	*dest = value;
	handle_mutex(mutex, UNLOCK);	
}

bool	get_bool(pthread_mutex_t *mutex, bool *value)
{
	int	ret;

	handle_mutex(mutex, LOCK);
	ret = *value;
	handle_mutex(mutex, UNLOCK);
	return (ret);
}

void	set_bool(pthread_mutex_t *mutex, bool value, bool *dest)
{
	handle_mutex(mutex, LOCK);
	*dest = value;
	handle_mutex(mutex, UNLOCK);	
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	ret;

	handle_mutex(mutex, LOCK);
	ret = *value;
	handle_mutex(mutex, UNLOCK);
	return (ret);
}

void	set_long(pthread_mutex_t *mutex, long value, long *dest)
{
	handle_mutex(mutex, LOCK);
	*dest = value;
	handle_mutex(mutex, UNLOCK);	
}

//wrapper to clearly see what happens
bool sim_finished(t_program *program)
{
	return (get_bool(&program->program_mutex, &program->end_sim));
}