#include<iostream>
#include<algorithm>
#include<stdexcept>

template<class T>
class Queue
{
private:
	T* queue = nullptr;
	int front = 0;
	int rear = 0;
	int capacity = 0;
	void changeSize()
	{
		T* newQueue = new T[capacity * 2];
		int start = (front + 1) % capacity;
		if (start < 2)
			std::copy(queue + start, queue + start + capacity - 1, newQueue);
		else
		{
			std::copy(queue + start, queue + capacity, newQueue);
			std::copy(queue, queue + rear + 1, newQueue + capacity - start);
		}
		front = capacity * 2 - 1;
		rear = capacity - 2;
		capacity *= 2;
		delete[] queue;
		queue = newQueue;
	}
public:
	Queue(int capacity = 4)
	{
		if (capacity <= 0)
			throw std::length_error("Capacity must be greater than 0");
		this->capacity = capacity;
		queue = new T[capacity];
	}
	inline bool Empty()
	{
		return (front == rear);
	}
	inline T& Front() const
	{
		if (Empty())
			throw std::out_of_range("Queue is empty.");
		return queue[(front + 1) % capacity];
	}
	inline T& Rear() const
	{
		if (Empty())
			throw std::out_of_range("Queue is empty.");
		return queue[rear];
	}
	void Push(const T& item)
	{
		if ((rear + 1) % capacity == front)
			changeSize();
		rear = (rear + 1) % capacity;
		queue[rear] = item;
	}
	void Pop()
	{
		if (Empty())
			throw std::out_of_range("Queue is empty.");
		front = (front + 1) % capacity;
	}
};

int main()
{
	Queue<int> q(2);
	q.Push(1);
	q.Push(2);
	q.Pop();
	q.Pop();
	return 0;
}