#include <time.h>		// time(), ctime(), difftime(), struct tm
#include <stdio.h>		// sprintf

class Timer
{
 private:
  time_t t_start, t_stop, t_elapse;
  struct tm * t_report;
  char t_buffer[80];

 public:
  Timer();
  ~Timer();
  
  char * start();
  char * stop();
  char * report();
};

Timer::Timer()
{
  t_start = time(NULL);
}

Timer::~Timer()
{
}

char * Timer::start()
{
  t_start = time(NULL);
  sprintf(t_buffer, "Starting time: %s", ctime(&t_start));

  return t_buffer;
}

char * Timer::stop()
{
  t_stop = time(NULL);
  sprintf(t_buffer, "Stopping time: %s", ctime(&t_stop));

  return t_buffer;
}

// report elapsed time in readable format
// limitation: days are counted within one year only...
// timezone used to compensate local time zone different.
char * Timer::report()
{
  t_elapse = (time_t)difftime(t_stop, t_start); // + timezone if use localtime
  t_report = gmtime(&t_elapse);			// based on UTC 00:00:00 1/1/1970

  sprintf(t_buffer, "Elapsed time: %d day(s), %d hour(s), %d min(s), %d sec(s).\n",
	  t_report->tm_yday, 
	  t_report->tm_hour, 
	  t_report->tm_min,
	  t_report->tm_sec);

  return t_buffer;
}
