//
// Created by galqiwi on 02.01.2022.
//

#include <iostream>
#include "Semaphore.h"

namespace concurrency {

void Semaphore::Release() {
  std::unique_lock<std::mutex> lock(mutex_);
  ++counter_;
  can_acquire_.notify_one();
}
void Semaphore::Acquire() {
  std::unique_lock<std::mutex> lock(mutex_);

  while (counter_ == 0) {
    can_acquire_.wait(lock);
  }

  --counter_;
}

}