#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <Arduino.h> // Essential for Arduino types/functions

class myQueue {
private:
  int* arr;
  int front;
  int size;
  int capacity;

public:
  myQueue(int cap);
  ~myQueue(); // Added destructor to free memory
  void enqueue(int x);
  int dequeue();
  int getFront();
  int getRear();
  bool isFull() { return size == capacity; }
  bool isEmpty() { return size == 0; }
  int getAt(int index);
  int getSize() { return size; }
};

#endif