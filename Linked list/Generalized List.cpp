#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <variant>
#include <memory>

template<class T>class GenList;
template<class T>class GenListNode;

template<class T>
class GenListNode
{
	friend class GenList<T>;
private:
	bool flag = false;
	std::shared_ptr<GenListNode<T>> next;
	std::variant<T, std::shared_ptr<GenListNode<T>>> element;
};

template<class T>
class GenList
{
private:
	std::shared_ptr<GenListNode<T>> first;
	std::shared_ptr<GenListNode<T>> copy(const std::shared_ptr<GenListNode<T>>& current)
	{
		std::shared_ptr<GenListNode<T>> new_node;
		if (current != nullptr)
		{
			new_node = std::make_shared<GenListNode<T>>();
			new_node->flag = current->flag;
			if (new_node->flag)
				new_node->element = copy(std::get<std::shared_ptr<GenListNode<T>>>(current->element));
			else
				new_node->element = std::get<T>(current->element);
			new_node->next = copy(current->next);
		}
		return new_node;
	}
	bool equal(const std::shared_ptr<GenListNode<T>>& list1,const std::shared_ptr<GenListNode<T>>& list2)
	{
		if (list1 == nullptr && list2 == nullptr)
			return true;
		if (list1 != nullptr && list2 != nullptr && (list1->flag == list2->flag))
			if (list1->flag == true)
				return equal(std::get<std::shared_ptr<GenListNode<T>>>(list1->element), std::get<std::shared_ptr<GenListNode<T>>>(list2->element)) && equal(list1->next, list2->next);
			else
				return (std::get<T>(list1->element) == std::get<T>(list2->element)) && equal(list1->next, list2->next);
		return false;
	}
	int depth(const std::shared_ptr<GenListNode<T>>& current)
	{
		if (current == nullptr)
			return 0;
		int max = 0;
		std::shared_ptr<GenListNode<T>> index = current;
		while (index != nullptr)
		{
			if (index->flag != nullptr)
				max = std::max(max, depth(std::get<std::shared_ptr<GenListNode<T>>>(current->element)));
			current = current->next;
		}
		return max + 1;
	}
public:
	GenList<T>() noexcept {}
	GenList<T>(const T& element) noexcept
	{
		first = std::make_shared<GenListNode<T>>();
		first->element = element;
	}
	GenList<T>(const GenList<T>& subList) noexcept
	{
		first = std::make_shared<GenListNode<T>>();
		first->flag = true;
		first->element = subList.first;
	}
	void push_back(const T& element)
	{
		std::shared_ptr<GenListNode<T>> new_node = std::make_shared<GenListNode<T>>();
		new_node->element = element;
		if (first == nullptr)
		{
			first = new_node;
			return;
		}
		std::shared_ptr<GenListNode<T>> last = first;
		while (last->next != nullptr)
			last = last->next;
		last->next = new_node;
	}
	void push_back(const GenList<T>& subList)
	{
		std::shared_ptr<GenListNode<T>> new_node = std::make_shared<GenListNode<T>>();
		new_node->flag = true;
		new_node->element = subList.first;
		if (first == nullptr)
		{
			first = new_node;
			return;
		}
		std::shared_ptr<GenListNode<T>> last = first;
		while (last->next != nullptr)
			last = last->next;
		last->next = new_node;
	}
	void pop_back()
	{
		if (first->next == nullptr)
		{
			first = nullptr;
			return;
		}
		std::shared_ptr<GenListNode<T>> last = first;
		while (last->next->next != nullptr)
			last = last->next;
		last->next = nullptr;
	}
	void copy(const GenList<T>& otherList)
	{
		first = copy(otherList.first);
	}
	int depth(const GenList<T>& otherList)
	{
		return depth(first);
	}
	inline void clear() noexcept
	{
		first = nullptr;
	}
	inline bool empty() noexcept
	{
		return (first == nullptr);
	}
	bool operator==(const GenList<T>& otherList)
	{
		return equal(first, otherList.first);
	}
	bool operator!=(const GenList<T>& otherList)
	{
		return !equal(first, otherList.first);
	}
};

int main()
{
	return 0;
}