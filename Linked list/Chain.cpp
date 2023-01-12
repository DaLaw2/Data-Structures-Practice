#include <iostream>
#include <algorithm>
#include <stdexcept>

template<class T>class Chain;
template<class T>class ChainNode;
template<class T>class iterator;

template<class T>
class Chain
{
private:
	ChainNode<T>* first = nullptr;
	ChainNode<T>* last = nullptr;
public:
	~Chain()
	{
		clear();
	}
	void push_front(const T source)
	{
		ChainNode<T>* new_node = new ChainNode<T>(source);
		ChainNode<T>* temp = first;
		if (empty())
		{
			first = last = new_node;
			return;
		}
		first = new_node;
		new_node->next = temp;
	}
	void pop_front()
	{
		if (empty())
			return;
		if (first->next == nullptr)
		{
			delete first;
			first = last = nullptr;
			return;
		}
		ChainNode<T>* new_first = first->next;
		delete first;
		first = new_first;
	}
	void push_back(const T source)
	{
		ChainNode<T>* new_node = new ChainNode<T>(source);
		if (empty())
		{
			first = last = new_node;
			return;
		}
		last->next = new_node;
		last = new_node;
	}
	void pop_back()
	{
		if (empty())
			return;
		if (first->next == nullptr)
		{
			delete first;
			first = last = nullptr;
			return;
		}
		ChainNode<T>* new_last = first;
		while (new_last->next->next != nullptr)
			new_last = new_last->next;
		delete last;
		new_last->next = nullptr;
		last = new_last;
	}
	void insert(const T source, int index)
	{
		ChainNode<T>* new_node = new ChainNode<T>(source);
		ChainNode<T>* target = first;
		for (int step = 0; step < index; step++)
			target = target->next;
		if (target->next == nullptr)
			last = new_node;
		ChainNode<T>* temp = target->next;
		target->next = new_node;
		new_node->next = temp;
	}
	void remove(int index)
	{
		ChainNode<T>* target = first;
		if (index == 0)
		{
			first = first->next;
			delete target;
			return;
		}
		for (int step = 0; step < index - 1; step++)
			target = target->next;
		if (target->next->next == nullptr)
			last = target;
		ChainNode<T>* temp = target->next;
		target->next = temp->next;
		delete temp;
	}
	void ConcatenoteI(Chain<T>& source)
	{
		if (empty())
		{
			first = source.first;
			last = source.last;
		}
		else
		{
			last->next = source.first;
			last = source.last;
		}
		source.first = source.last = nullptr;
	}
	void reverse()
	{
		ChainNode<T>* past = nullptr;
		ChainNode<T>* current = first;
		while (current)
		{
			ChainNode<T>* temp = past;
			past = current;
			current = current->next;
			past->next = temp;
		}
		ChainNode<T>* temp = first;
		first = last;
		last = temp;
	}
	void clear()
	{
		while (first != nullptr)
			pop_back();
	}
	inline bool empty()
	{
		return (first == nullptr);
	}
	T& operator[](int index)
	{
		ChainNode<T>* temp = first;
		for (int step = 0; step < index; step++)
			temp = temp->next;
		return temp->data;
	}
	void operator=(const Chain<T>& source)
	{
		clear();
		first = source.first;
		last = source.last;
	}
	Chain<T>& operator+(const Chain<T> source)
	{
		for (iterator<T> index = source.begin(); index != source.end(); index++)
			push_back(*index);
	}
	void operator+=(Chain<T> source)
	{
		this = this + source;
	}
	inline iterator<T> begin()
	{
		return iterator<T>(first);
	}
	inline iterator<T> end()
	{
		return iterator<T>(nullptr);
	}
};

template<class T>
class ChainNode
{
	friend class Chain<T>;
	friend class iterator<T>;
private:
	T data;
	ChainNode<T>* next = nullptr;
public:
	ChainNode(const T source)
	{
		this->data = source;
	}
};

template<class T>
class iterator
{
private:
	ChainNode<T>* current = nullptr;
public:
	iterator(ChainNode<T>* source = nullptr)
	{
		this->current = source;
	}
	T& operator*()
	{
		return current->data;
	}
	T* operator->()
	{
		return &current->data;
	}
	iterator<T> operator++()
	{
		current = current->next;
		return this;
	}
	iterator<T> operator++(int)
	{
		iterator<T> temp = this;
		current = current->next;
		return temp;
	}
	inline bool operator==(iterator<T> source)
	{
		return (source.current == this->current);
	}
	inline bool operator!=(iterator<T> source)
	{
		return (source.current != this->current);
	}
};

int main()
{
	return 0;
}