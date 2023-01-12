#include<iostream>
#include<algorithm>
#include<stdexcept>

using namespace std;

template<class T>
class Stack
{
private:
	T* stack = nullptr;
	int top = -1;
	int capacity = 0;
	void changeArray1D(T*& array, int oldSize, int newSize)
	{
		T* temp = new T[newSize];
		copy(array, array + oldSize, temp);
		delete[] array;
		array = temp;
	}
public:
	Stack(int capacity = 10)
	{
		if (capacity <= 0)
			throw length_error("");
		stack = new T[capacity];
		this->capacity = capacity;
	}
	inline ~Stack()
	{
		delete[] stack;
	}
	inline bool Empty()
	{
		if (top == -1)
			return true;
		return false;
	}
	inline T& Top() const
	{
		if (top == -1)
			throw out_of_range("");
		return stack[top];
	}
	void Push(const T& item)
	{
		if (capacity == top + 1)
		{
			changeArray1D(stack, capacity, capacity * 2);
			capacity *= 2;
		}
		stack[++top] = item;
	}
	void Pop()
	{
		if (top == -1)
			throw out_of_range("");
		top--;
	}
};

int main()
{
	Stack<int> s1(5);
	s1.Push(5);
	s1.Push(4);
	s1.Push(3);
	s1.Push(2);
	s1.Push(1);
	s1.Push(0);
	s1.Pop();
	cout << s1.Top();
	return 0;
}