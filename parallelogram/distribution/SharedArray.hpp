//
// Created by galqiwi on 18.11.2021.
//

#ifndef RULE30_21_SHAREDARRAY_HPP
#define RULE30_21_SHAREDARRAY_HPP

#include <mpi.h>
#include "../crunching/process_parallelogram.h"
#define MPI_WORD_SIZE MPI_UINT32_T

template<typename T>
class SharedArray {
 public:
  SharedArray() {

  }
  explicit SharedArray(size_t n): data_(n) {
  }

  void Send(int destination) const {
    MPI_Send(
        /* const void *buf       (aka pointer to start) = */ &data_[0],
        /* int count             (aka number of words)  = */ data_.size(),
        /* MPI_Datatype datatype (aka word type)        = */ MPI_WORD_SIZE,
        /* int dest              (aka rank of receiver) = */ destination,
        /* int tag               (aka tag)              = */ 0,
        /* MPI_Comm comm         (aka communicator)     = */ MPI_COMM_WORLD);
  }

  int Update(int source) {
    if (source == FromAnywhere()) {
      source = MPI_ANY_SOURCE;
    }
    MPI_Status status;
    MPI_Recv(
        /* void *buf             (aka pointer to start) = */ &data_[0],
        /* int count             (aka number of words)  = */ data_.size(),
        /* MPI_Datatype datatype (aka word type)        = */ MPI_WORD_SIZE,
        /* int source            (aka rank of sender)   = */ source,
        /* int tag               (aka tag)              = */ 0,
        /* MPI_Comm comm         (aka communicator)     = */ MPI_COMM_WORLD,
        /* MPI_Status *status    (aka status of recv)   = */ &status);
    return status.MPI_SOURCE;
  }

  std::vector<T>& operator*() {
    return data_;
  }

  const std::vector<T>& operator* () const {
    return data_;
  }

  static int FromAnywhere() {
    return -1;
  }
 private:
  std::vector<T> data_;
};

#endif  // RULE30_21_SHAREDARRAY_HPP
