#include "myQueue.h"

myQueue::myQueue(int cap) {
  capacity = cap;
  arr = new int[capacity];
  front = 0;
  size = 0;
}

myQueue::~myQueue() {
  delete[] arr;  // Clean up memory
}

void myQueue::enqueue(int x) {
  if (size == capacity) {
    Serial.println("Queue is full!");
    return;
  }
  int rear = (front + size) % capacity;
  arr[rear] = x;
  size++;
}

int myQueue::dequeue() {
  if (size == 0) {
    Serial.println("Queue is empty!");
    return -1;
  }
  int res = arr[front];
  front = (front + 1) % capacity;
  size--;
  return res;
}

int myQueue::getFront() {
  return (size == 0) ? -1 : arr[front];
}

int myQueue::getRear() {
  if (size == 0) return -1;
  int rear = (front + size - 1) % capacity;
  return arr[rear];
}

int myQueue::getAt(int index) {
  if (index < 0 || index >= size) return -1;
  int pos = (front + index) % capacity;
  return arr[pos];
}