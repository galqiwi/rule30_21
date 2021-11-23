//
// Created by galqiwi on 23.11.2021.
//

#ifndef RULE30_21_IGRID_H
#define RULE30_21_IGRID_H

using Cell = std::pair<int, int>;

class IGrid {
 public:
  virtual void Init() = 0;
  virtual void Do(Cell) = 0;
  virtual bool IsValid(Cell) = 0;
};

class ISendableGrid: public IGrid {
 public:
  virtual void SendTop(Cell) = 0;
  virtual void ReceiveTop(Cell) = 0;

  virtual void SendLeft(Cell) = 0;
  virtual void ReceiveLeft(Cell) = 0;
};

#endif  // RULE30_21_IGRID_H
