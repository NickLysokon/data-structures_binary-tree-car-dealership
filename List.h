#ifndef _MY_NODE_LIST_H
#define _MY_NODE_LIST_H

#include <iostream>

template<class T>
struct ListNode {
	T data;
	ListNode* next;
	ListNode* prev;
};

//For debug purposes only:
template<class T> class List;
template<class T> std::ostream& operator<<(std::ostream& os, const List<T>& lst);


template<class T>
class List {
	ListNode<T> *head, *tail;

	static void copyListAux(const List<T>& lst, ListNode<T>* head, ListNode<T>* tail);

public:
	List();
	~List();
	List(const List& lst);
	List& operator=(const List& lst);

	ListNode<T>* insertLast(const T& element);  //Insert at the end of the list in O(1) and return the node.
	ListNode<T>* insertFirst(const T& element); //Insert at the start of the list in O(1) and return the node.
	ListNode<T>* insertAfter(ListNode<T>* node, const T& element);	//Insert after a specific node in O(1) and return it.
	ListNode<T>* insertBefore(ListNode<T>* node, const T& element);	//Insert before a specific node in O(1) and return it.
	void removeNode(ListNode<T>* element);		//Remove a specific node in O(1)
	ListNode<T>* getHead(); //Returns the first element, or null if empty
	ListNode<T>* getHead() const;
	ListNode<T>* getEnd(); //Returns the last element, or null if empty


	template<class S>
	friend std::ostream& operator<<(std::ostream& os, const List<S>& lst);

};


template<class T>
List<T>::List(): head(new ListNode<T>), tail(new ListNode<T>) 
{
	//Dummy nodes:
	head->prev = nullptr;
	tail->next = nullptr;

	head->next = tail;
	tail->prev = head;
}


template<class T>
List<T>::~List() 
{
	while (head != nullptr) {
		ListNode<T>* ptr = head;
		head = head->next;
		delete ptr;
	}
}


template<class T>
void List<T>::copyListAux(const List<T>& lst, ListNode<T>* head, ListNode<T>* tail)
{
	ListNode<T>* ptr1 = lst.head->next, * curr_ptr = head;

	while (ptr1->next != nullptr) {
		ListNode<T>* ptr2 = new ListNode<T>;
		ptr2->data = ptr1->data;  //Data operator=
		ptr2->prev = curr_ptr;
		curr_ptr->next = ptr2;

		ptr1 = ptr1->next;
		curr_ptr = curr_ptr->next;
	}

	curr_ptr->next = tail;
	tail->prev = curr_ptr;

	head->prev = nullptr;
	tail->next = nullptr;
}


template<class T>
List<T>::List(const List& lst): head(new ListNode<T>), tail(new ListNode<T>) 
{
	copyListAux(lst, head, tail);
}


template<class T>
List<T>& List<T>::operator=(const List<T>& lst)
{
	if (this == &lst) {
		return *this;
	}

	//Delete old list data
	ListNode<T>* head_ptr = head->next;
	while (head_ptr->next != nullptr) {
		ListNode<T>* ptr = head_ptr;
		head_ptr = head_ptr->next;
		delete ptr;
	}

	copyListAux(lst, head, tail);
	return *this;
}


template<class T>
ListNode<T>* List<T>::insertLast(const T& element)
{
	ListNode<T>* ptr = new ListNode<T>;
	ptr->data = element;
	ptr->prev = tail->prev;
	ptr->next = tail;
	
	tail->prev->next = ptr;
	tail->prev = ptr;
	return ptr;
}


template<class T>
ListNode<T>* List<T>::insertFirst(const T& element)
{
	ListNode<T>* ptr = new ListNode<T>;
	ptr->data = element;
	ptr->prev = head;
	ptr->next = head->next;

	head->next->prev = ptr;
	head->next = ptr;
	return ptr;
}


template<class T>
void List<T>::removeNode(ListNode<T>* element)
{
	if (!element) return;

	element->prev->next = element->next;
	element->next->prev = element->prev;

	delete element;
}


template<class T>
ListNode<T>* List<T>::insertAfter(ListNode<T>* node, const T& element)
{
	if (!node) return nullptr;

	ListNode<T>* ptr = new ListNode<T>;
	ptr->data = element;
	ptr->prev = node;
	ptr->next = node->next;

	node->next->prev = ptr;
	node->next = ptr;
	return ptr;

}


template<class T>
ListNode<T>* List<T>::insertBefore(ListNode<T>* node, const T& element)
{
	if (!node) return nullptr;

	ListNode<T>* ptr = new ListNode<T>;
	ptr->data = element;
	ptr->prev = node->prev;
	ptr->next = node;

	node->prev->next = ptr;
	node->prev = ptr;
	return ptr;
}


template<class T>
ListNode<T>* List<T>::getHead()
{
	if (head->next == tail) {
		return nullptr;
	}
	return head->next;
}


template<class T>
ListNode<T>* List<T>::getHead() const {
	if (head->next == tail) {
		return nullptr;
	}
	return head->next;
}

template<class T>
ListNode<T>* List<T>::getEnd()
{
	if (head->next == tail) {
		return nullptr;
	}
	return tail->prev;
}


template<class T>
std::ostream& operator<<(std::ostream& os, const List<T>& lst)
{
	ListNode<T>* iter = lst.head->next;
	os << "Head";
	while (iter->next != nullptr) {
		os << " <--> " << iter->data;
		iter = iter->next;
	}
	return os << " <--> Tail ";
}


#endif /* MY_NODE_LIST */