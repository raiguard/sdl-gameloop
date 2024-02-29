#pragma once
#include <condition_variable>
#include <functional>
#include <thread>

class Worker
{
public:
  Worker() : thread(&Worker::loop, this) {}
  ~Worker();
  void run(std::function<void()> job);
  void synchronize();
  bool isJobFinished() const { return this->jobFinished; }
private:
  void loop();

  bool finished = false;
  bool jobFinished = true;

  std::condition_variable condition;
  std::mutex mutex;
  std::thread thread;
  std::function<void()> job;
};
