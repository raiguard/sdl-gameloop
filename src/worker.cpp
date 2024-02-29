#include "worker.hpp"
#include <cassert>
#include <format>
#include <iostream>
#include <mutex>

Worker::~Worker()
{
  {
    std::lock_guard<std::mutex> lock(this->mutex);
    this->finished = true;
    this->job = nullptr;
    this->condition.notify_all();
  }
  this->thread.join();
}

void Worker::loop()
{
  std::unique_lock<std::mutex> lock(this->mutex);
  while (!this->finished)
  {
    if (this->jobFinished)
      this->condition.wait(lock);
    try
    {
      if (this->job)
      {
        this->job();
        this->job = nullptr;
        this->jobFinished = true;
        this->condition.notify_all();
      }
    }
    catch (const std::runtime_error& error)
    {
      std::cerr << std::format("Thread exception: {}.", error.what()) << std::endl;
      // exit(-1);
      this->finished = true;
    }
  }
}

void Worker::run(std::function<void()> job)
{
  std::lock_guard<std::mutex> lock(this->mutex);
  assert(this->jobFinished == true);
  this->jobFinished = false;
  this->job = std::move(job);
  this->condition.notify_all();
}

void Worker::synchronize()
{
  std::unique_lock<std::mutex> lock(this->mutex);
  while (!this->jobFinished)
    this->condition.wait(lock);
}
