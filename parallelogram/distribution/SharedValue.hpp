//
// Created by galqiwi on 18.11.2021.
//

#ifndef RULE30_21_SHAREDVALUE_HPP
#define RULE30_21_SHAREDVALUE_HPP

#include "SharedArray.hpp"

template<typename T> // should not be a template
class SharedValue {
 public:
  SharedValue(): data_(1) {

  }

  explicit SharedValue(T initial): data_(1) {
    (*data_)[0] = initial;
  }

  void Send(int destination) const {
    data_.Send(destination);
  }

  int Update(int source) {
    return data_.Update(source);
  }

  T& operator*() {
    return (*data_)[0];
  }

  const T& operator*() const {
    return (*data_)[0];
  }

  static int FromAnywhere() {
    return SharedArray<T>::FromAnywhere();
  }
 private:
  SharedArray<T> data_;
};

#endif  // RULE30_21_SHAREDVALUE_HPP
