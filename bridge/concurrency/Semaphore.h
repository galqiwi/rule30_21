//
// Created by galqiwi on 02.01.2022.
//

#ifndef RULE30_21_SEMAPHORE_H
#define RULE30_21_SEMAPHORE_H

#include <cstdint>
#include <mutex>
#include <condition_variable>

namespace concurrency {

using i64 = int64_t;

class Semaphore {
 public:
  void Acquire();
  void Release();

 private:
  i64 counter_{0};
  std::mutex mutex_;
  std::condition_variable can_acquire_;
};

}

#endif  // RULE30_21_SEMAPHORE_H
