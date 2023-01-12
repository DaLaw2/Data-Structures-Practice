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
	void changeSize(int newSize)
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
	inline virtual ~Bag()
	{
		delete[] array;
	}
	inline virtual int Size()
	{
		return top + 1;
	}
	inline virtual bool Empty()
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
class Stack:public Bag<T>
{
public:
	Stack(int capacity = 10){}
	~Stack(){}
	T& Top() const
	{
		if (this->top + 1 == 0)
			throw std::out_of_range("Stack is empty.");
		return this->array[this->top];
	}
	void Pop()
	{
		if (this->top + 1 == 0)
			throw std::out_of_range("Stack is empty.");
		this->top--;
	}
};

int main()
{
}