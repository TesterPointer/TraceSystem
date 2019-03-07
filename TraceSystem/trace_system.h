/*
Copyright 2010-2018 Pablo P. Serrano. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDER ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __PSERR_TRACE_SYSTEM_H__
#define __PSERR_TRACE_SYSTEM_H__ 1

#include <mutex>
#include <cstdio>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
/*
HOW TO INCLUDE: Just include "TraceSystem/trace_system.h" and link your application
         to the lib folder and the appropiatte system
WARNING: The use of theese code can reduce "significally"(not much, is just write in file)
         the performance (I said it, not to much, but in MICROSECONDS....
USAGE:  -Enter in Google Chrome and write: chrome://tracing/
        -What�s the difference between METRICS() & METRICS_NAME(function_name)?
          Simple, the first one takes automatically the name of the function,
          in the second you can pass your own name for the Trace.
        -This macros will create a file in your EXECUTABLE path named chrome_tracing.json
          and you can insert it in the chrome://tracing/ app
        -You don�t have to worry about the instance of these Classes or the
          destruction, just use the METRICS

FEATURE: the first time you call METRICS will be placed in the 0 microsecond

////////////////////////////////////////////////////////////////////////////////
CODE EXAMPLE:

#include "TraceSystem/trace_system.h"
#include <cmath>

double CalculateSQRT(int c) {
  return sin(sqrt(c));
}

void first() {
METRICS_NAME("Custom name");
  for (unsigned i = 0; i < 500; i++) {
    CalculateSQRT(i);
  }
}

void second() {
  METRICS();
  for (unsigned i = 0; i < 999; i++) {
    for(unsigned j = 0; j < 999; j++)
      CalculateSQRT(i * j);
  }
}
int main() {
  first();
  second();
  return 0;
}
////////////////////////////////////////////////////////////////////////////////


*/
#if defined(_MSC_VER) || defined(__APPLE__)
#define FUNC_NAME_TRACE __FUNCTION__
#elif defined(__linux__)
#define FUNC_NAME_TRACE __func__
#endif

//**********************USE THIS AND DONT WORRY ABOUT MORE**********************
#define METRICS() ::pserr::Metrics _tmp_metrics ## __LINE__ (FUNC_NAME_TRACE, __FILE__, __LINE__)
#define METRICS_NAME(function_name) ::pserr::Metrics _tmp_metrics ## __LINE__ (function_name, __FILE__, __LINE__)
namespace pserr {

  class TraceSystem {
  public:
    static TraceSystem* Instance();

    enum Type {
      Start,
      End
    };
    enum Precision {
      MicroSeconds,
      MiliSeconds
    };

    ~TraceSystem();

    void LogJSon(std::string function_name, TraceSystem::Type type_log, double time);

    double GetTime(Precision time_precision)const;

  private:
    static std::unique_ptr<TraceSystem> instance_trace_system;
    TraceSystem();
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    bool is_first_line_;

    std::mutex mutex_json_log_;
    FILE* file_json_;
  };

  struct Metrics {
    Metrics(const char *func, const char *file, size_t line);
    ~Metrics();
    const char *func_;
    const char *file_;
    size_t line_;
  };


}


#endif

#ifdef TRACE_SYSTEM_IMPLEMENTATION

#ifndef TRACE_SYSTEM_IMPLEMENTATION_DONE
#define TRACE_SYSTEM_IMPLEMENTATION_DONE

#include <sstream>
#include <iostream>

//Instance
std::unique_ptr<pserr::TraceSystem> pserr::TraceSystem::instance_trace_system = 
std::unique_ptr<TraceSystem>(new TraceSystem);

static void Aux_WritteInFile(FILE* f, std::string buff) {
  fwrite(buff.c_str(), buff.size(), 1, f);
}
pserr::TraceSystem::TraceSystem() {
  start_time_ = std::chrono::high_resolution_clock::now();
  std::cout << "TraceSystem starts at (microseconds): " << GetTime(Precision::MicroSeconds) << std::endl;
  std::string log_place;
  file_json_ = fopen("chrome_tracing.json", "w");

  Aux_WritteInFile(file_json_, "{ \"traceEvents\": [");

  is_first_line_ = true;
}

pserr::TraceSystem* pserr::TraceSystem::Instance() {
  return instance_trace_system.get();
}

pserr::TraceSystem::~TraceSystem() {
  Aux_WritteInFile(file_json_, "], \"displayTimeUnit\": \"ns\",\"meta_user\": \"aras\",\"meta_cpu_count\" : \"8\"}");
  fclose(file_json_);
  std::cout << "TraceSystem ends at (microseconds): " << GetTime(Precision::MicroSeconds) << std::endl;
}

void pserr::TraceSystem::LogJSon(std::string function_name, TraceSystem::Type type_log, double time) {
  mutex_json_log_.lock();

  std::string log_line;

  if (!is_first_line_) {
    log_line += ",";
  }
  else {
    is_first_line_ = false;
  }

  //WRITE NAME FUNCTION
  log_line += "{\"name\": \"" + function_name + "\",";

  //WRITE IF IS STARTING OR ENDING
  switch (type_log) {
    case Start:

    log_line += "\"ph\" : \"B\",";

    break;
    case End:

    log_line += "\"ph\" : \"E\",";

    break;
  }

  //WRITE PROCCESS AND THREAD ID
  std::hash<std::thread::id> hasher;
  size_t id = hasher(std::this_thread::get_id());

  log_line += "\"pid\": 1, \"tid\": " + std::to_string(id) + "";

  //WRITE TIME
  log_line += ", \"ts\": " + std::to_string(time) + "}";

  Aux_WritteInFile(file_json_, log_line);
  mutex_json_log_.unlock();
}

double pserr::TraceSystem::GetTime(TraceSystem::Precision precision_time) const {
  auto actual_time = std::chrono::high_resolution_clock::now();

  auto diff = actual_time - start_time_;

  switch (precision_time) {
    case MicroSeconds:
    return (double)std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    break;
    case MiliSeconds:
    return (double)std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    break;
    default:
    return (double)std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    break;
  }
}

//------------------------------------------------------------------------------
pserr::Metrics::Metrics(const char *func, const char *file, size_t line) {
  func_ = func;
  file_ = file;
  line_ = line;
  TraceSystem::Instance()->LogJSon(func_, TraceSystem::Start, TraceSystem::Instance()->GetTime(TraceSystem::MicroSeconds));
}

pserr::Metrics::~Metrics() {
  TraceSystem::Instance()->LogJSon(func_, TraceSystem::End, TraceSystem::Instance()->GetTime(TraceSystem::MicroSeconds));
}




#endif // TRACE_SYSTEM_IMPLEMENTATION_DONE

#endif // TRACE_SYSTEM_IMPLEMENTATION
