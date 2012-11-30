#include <iostream>

#include "common/util/tick_timer.h"
#include "common/util/timespec.h"

TickTimer::TickTimer(long tick_time)
{
  m_tick_time.tv_sec = 0;
  m_tick_time.tv_nsec = tick_time;
  std::cout << "tick time " << m_tick_time << std::endl;
}

void TickTimer::start_tick()
{
  clock_gettime(CLOCK_MONOTONIC, &m_start);
  m_target = m_start + m_tick_time;
}

void TickTimer::end_tick()
{
  clock_gettime(CLOCK_MONOTONIC, &m_end);
  std::cout << "tick lasted " << (m_end - m_start);
  if (m_end < m_target)
  {
    timespec diff(m_target - m_end);
    nanosleep(&diff, NULL);
    std::cout << ", slept " << diff << std::endl;
  }
  else
  {
    timespec diff(m_end - m_target);
    std::cout << ", went " << diff << " overtime" << std::endl;
  }
}

