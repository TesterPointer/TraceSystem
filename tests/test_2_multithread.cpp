#define TRACE_SYSTEM_IMPLEMENTATION
#include "TraceSystem/trace_system.h"
#include <cmath>
#include <thread>
void quicklyFunc()
{
  METRICS();
  for(unsigned i = 0; i < 100000; ++i)
  {
    sqrt (i);
  }
}

void UnreasonableSlowFunc()
{
  METRICS_NAME("My custom happy name function");
  for(unsigned i = 0; i < 10000000; ++i)
  {
    sqrt(i);
  }
}

void slowFunc()
{
  METRICS();
  for(unsigned i = 0; i < 1000; ++i)
  {
    for(unsigned j = 0; j < 100; ++j) {
      sqrt(i) / sqrt(j);
    }
  }
}



int main()
{
  METRICS();
  std::thread t1(slowFunc);
  std::thread t2(slowFunc);
  std::thread t3(slowFunc);
  std::thread t4(slowFunc);
  quicklyFunc();
  
  UnreasonableSlowFunc();

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  
  return 0;
}