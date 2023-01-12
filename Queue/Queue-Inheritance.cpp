#include<cmath>
#include<ctime>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<stdexcept>

template<class T>
class Bag
{
protected:
	int top = -1;
	int capacity = 0;
	T* array = nullptr;
	virtual void changeSize(int newSize)
	{
		T* newArray = new T[newSize];
		std::copy(array, array + Size(), newArray);
		delete[] array;
		array = newArray;
	}
public:
	Bag(int capacity = 10)
	{
		if (capacity < 1)
			throw std::length_error("Capacity must be greater than 0.");
		array = new T[capacity];
		this->capacity = capacity;
	}
	virtual ~Bag()
	{
		delete[] array;
	}
	virtual int Size()
	{
		return top + 1;
	}
	virtual bool Empty()
	{
		return (top + 1 == 0);
	}
	virtual T& Element() const
	{
		if (top + 1 == 0)
			throw std::out_of_range("Bag is empty.");
		srand(time(NULL));
		return array[rand() % (top + 1)];
	}
	virtual void Push(const T& item)
	{
		if (top + 1 == capacity)
			changeSize(2 * capacity);
		capacity *= 2;
		array[++top] = item;
	}
	virtual void Pop()
	{
		if (top + 1 == 0)
			throw std::out_of_range("Bag is empty.");
		srand(time(NULL));
		int target = rand() % Size();
		std::copy(array + target + 1, array + target + top + 1, array + target);
		top--;
	}
};

template<class T>
class Queue:public Bag<T>
{
protected:
	int front = 0;
	int rear = 0;
	void changeSize(int newSize) override
	{
		T* newQueue = new T[newSize];
		int start = (front + 1) % this->capacity;
		if (start < 2)
			std::copy(this->array + start, this->array + start + this->capacity - 1, newQueue);
		else
		{
			std::copy(this->array + start, this->array + this->capacity, newQueue);
			std::copy(this->array, this->array + rear + 1, newQueue + this->capacity - start);
		}
		front = newSize - 1;
		rear = this->capacity - 2;
		this->capacity *= 2;
		delete[] this->array;
		this->array = newQueue;
	}
public:
	Queue(int capacity = 10){}
	~Queue(){}
	int Size() override
	{
		return abs(rear - front);
	}
	bool Empty() override
	{
		return (front == rear);
	}
	void Push(const T& item) override
	{
		if ((rear + 1) % this->capacity == front)
			changeSize(2 * this->capacity);
		rear = (rear + 1) % this->capacity;
		this->array[rear] = item;
 	}
	void Pop() override
	{
		if (Empty())
			throw std::out_of_range("Queue is empty.");
		front = (front + 1) % this->capacity;
	}
};

int main()
{
	Queue<int> q;
	q.Push(10);
	q.Push(5);
	return 0;
}