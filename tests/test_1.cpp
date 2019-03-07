#define TRACE_SYSTEM_IMPLEMENTATION
#include "TraceSystem/trace_system.h"
#include <cmath>

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
  for(unsigned i = 0; i < 100; ++i)
  {
    for(unsigned j = 0; j < 100; ++j) {
      sqrt(i) / sqrt(j);
    }
  }
}

int main()
{
  METRICS();
  quicklyFunc();
  for(unsigned i = 0; i < 40; ++i)
    slowFunc();
  UnreasonableSlowFunc();
  
  return 0;
}