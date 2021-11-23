//
// Created by galqiwi on 19.11.2021.
//

#ifndef RULE30_21_IWORKCHUNK_H
#define RULE30_21_IWORKCHUNK_H

#include <memory>
#include <vector>


class IWork {
  using IWorkPtr = std::shared_ptr<IWork>;

  virtual IWorkPtr Merge(IWorkPtr top, IWorkPtr left) = 0;
  virtual IWorkPtr Initial() = 0;
  virtual IWorkPtr ProgressFirstRow(IWorkPtr left) = 0;
  virtual IWorkPtr ProgressFirstColumn(IWorkPtr left) = 0;

  virtual void Send(int receiver) = 0;
  virtual void Update(int source) = 0;
  int FromEveryone();

};


#endif  // RULE30_21_IWORKCHUNK_H
