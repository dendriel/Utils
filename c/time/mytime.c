/* Accurate time measuring for C development 
 *
 * vitor.rozsa
 *
 * 09/01/2013
 *
 * vitor.rozsa@hotmail.com
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>


 /* actually, i found this function at some C reference */
//int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y);
int timeval_subtract (result, x, y)
          struct timeval *result, *x, *y;
     {
       /* Perform the carry for the later subtraction by updating y. */
       if (x->tv_usec < y->tv_usec) {
         int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
         y->tv_usec -= 1000000 * nsec;
         y->tv_sec += nsec;
       }
       if (x->tv_usec - y->tv_usec > 1000000) {
         int nsec = (x->tv_usec - y->tv_usec) / 1000000;
         y->tv_usec += 1000000 * nsec;
         y->tv_sec -= nsec;
       }
     
       /* Compute the time remaining to wait.
          tv_usec is certainly positive. */
       result->tv_sec = x->tv_sec - y->tv_sec;
       result->tv_usec = x->tv_usec - y->tv_usec;


     
       /* Return 1 if result is negative. */
       return x->tv_sec < y->tv_sec;
     }



int main()
{
	struct timeval t_start = { 0 };
	struct timeval t_end = { 0 };
	struct timeval t_res = { 0 };

	gettimeofday(&t_start, NULL);

	usleep(1000100);

	gettimeofday(&t_end, NULL);

	printf("t_start.tv_sec = %ld; t_start.tv_usec = %ld\n", t_start.tv_sec, t_start.tv_usec);
	  printf("t_end.tv_sec = %ld; t_end.tv_usec = %ld\n", t_end.tv_sec, t_end.tv_usec);
	//printf("sec: %ld usec: %ld\n",  t_end.tv_sec - t_start.tv_sec, t_end.tv_usec - t_start.tv_usec);
	//printf("sec: %ld usec: %ld\n",  t_end.tv_sec, t_end.tv_usec);

	timeval_subtract(&t_res, &t_end, &t_start);

	printf("sec: %ld usec: %ld\n",  t_res.tv_sec, (long)(t_res.tv_usec/1000));
	printf("sec: %ld usec: %ld\n",  t_res.tv_sec, t_res.tv_usec);

	return 0;
}
