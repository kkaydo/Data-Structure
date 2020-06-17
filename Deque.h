//A deque (pronounced deck) is a double-ended queue. It is a data structure that supports insertion 
//and removal at both its front and its back, but not at any other position. It combines and extends 
//the operations of both a stack (insert at the front and remove from the front) and a queue 
//(insert at the back, remove from the front). This assignment is to  to implement this data structure using a doubly linked list.

#pragma once
#include <iostream>
#include <stdexcept>
using namespace std;


template<class T>
class Node {
public:
	T data;
	Node* next;
	Node* pre;
	Node() : data(NULL), next(nullptr), pre(nullptr) { ; ; };
	Node(T val) : data(val), next(nullptr), pre(nullptr) { ; ; };
	Node(T val, Node* nd, Node* pd) : data(val), next(nd), pre(pd) { ; ; };
};

template<class T>
class Deque {
public:
	Deque();
	Deque(const Deque<T>& dq);
	Deque& operator=(const Deque<T>& dq);
	~Deque();

	void insertFront(T val);
	void insertBack(T val);
	T removeFront();
	T removeBack();
	T peekFront();
	T peekBack();
	bool empty();
	int size();

private:
	Node<T>* head;
	Node<T>* tail;
	int dqsize;
};


//creates an empty Deque
template<class T>
Deque<T>::Deque()
{
	head = nullptr;
	tail = nullptr;
	dqsize = 0;
}


//copy constructor ¨C creates a deep copy of its constant Deque reference parameter
template<class T>
Deque<T>::Deque(const Deque<T>& dq)
{
	//when copied deque is empty
	if (dq.dqsize==0) {
		head = nullptr;
		tail = nullptr;
		dqsize = 0;
	}
	else {
		dqsize = dq.dqsize;
		Node<T>* dqtemp = dq.head;
		Node<T>* temp = new Node<T>();
		temp->data = dqtemp->data;
		head = temp;
		dqtemp = dqtemp->next;
		for (int i = 1; i < dqsize; i++) {
			Node<T>* newNode = new Node<T>();
			newNode->data = dqtemp->data;
			newNode->pre = temp;
			temp->next = newNode;
			if (dqtemp->next != nullptr) {
				dqtemp = dqtemp->next;
			}
			temp = newNode;
		}
		tail = temp;
	}
}

//overloads the assignment operator for Deque ¨C deep copy
template<class T>
Deque<T>& Deque<T>::operator=(const Deque<T>& dq)
{
	//Not to copy if the calling object is the same as the parameter
	if (head == dq.head) {
		return *this;
	}
	else {
		if (dq.dqsize == 0) {
			head = nullptr;
			tail = nullptr;
			dqsize = 0;
			return *this;
		}
		else {
			dqsize = dq.dqsize;
			Node<T>* dqtemp = dq.head;
			Node<T>* temp = new Node<T>();
			temp->data = dqtemp->data;
			head = temp;
			dqtemp = dqtemp->next;
			for (int i = 1; i < dqsize; i++) {
				Node<T>* newNode = new Node<T>();
				newNode->data = dqtemp->data;
				newNode->pre = temp;
				temp->next = newNode;
				if (dqtemp->next != nullptr) {
					dqtemp = dqtemp->next;
				}
				temp = newNode;
			}
			tail = temp;
			return *this;
		}
	}
}

//destructor
template<class T>
Deque<T>::~Deque<T>()
{
	Node<T>* temp = head;
	while (head != nullptr) {
		head = head->next;
		delete temp;
		temp = head;
	}
	dqsize = NULL;
}

//inserts a value at the front of the Deque
template<class T>
void Deque<T>::insertFront(T val)
{
	Node<T>* newNode = new Node<T>(val);
	if (head == nullptr) {
		head = newNode;
		tail = newNode;
		dqsize += 1;
	}
	else {
			Node<T>* temp = head;
			newNode->next = head;
			head->pre = newNode;
			head = newNode;
			head->next = temp;
			dqsize += 1;
		}
	
}

//inserts a value at the back of the Deque
template<class T>
void Deque<T>::insertBack(T val)
{
	Node<T>* newNode = new Node<T>(val);
	if (tail == nullptr) {
		head = newNode;
		tail = newNode;
		dqsize += 1;
	}
	else {
			Node<T>* temp = tail;
			newNode->pre = tail;
			tail->next = newNode;
			tail = newNode;
			tail->pre = temp;
			dqsize += 1;
		}
	
}

//removes and returns the value at the front of the Deque
template<class T>
T Deque<T>::removeFront()
{
	//throws a runtime_error if the Deque is empty 
	if (empty()) throw runtime_error("The deque is empty");
	else if (size() == 1) {
		T data = head->data;
		delete head;
		head = nullptr;
		tail = nullptr;
		return data;
	}
	else {
		Node<T>* temp = head;
		T data = temp->data;
		head = head->next;
		delete temp;
		head->pre = nullptr;
		dqsize -= 1;
		return data;
	}

}

//removes and returns the value at the back of the Deque
template<class T>
T Deque<T>::removeBack()
{
	if (empty()) throw runtime_error("The deque is empty");
	else if (size() == 1) {
		T data = head->data;
		delete head;
		head = nullptr;
		tail = nullptr;
		return data;
	}
	else {
		Node<T>* temp = tail;
		T data = temp->data;
		tail = tail->pre;
		delete temp;
		tail->next = nullptr;
		dqsize -= 1;
		return data;
	}
}

//returns the value at the front of the Deque
template<class T>
T Deque<T>::peekFront()
{
	if (empty()) throw runtime_error("The deque is empty");
	else {
		return head->data;
	}
}

//returns the value at the back of the Deque
template<class T>
T Deque<T>::peekBack()
{
	if (empty()) throw runtime_error("The deque is empty");
	else {
		return tail->data;
	}
}

template<class T>
bool Deque<T>::empty()
{
	if (head == nullptr && tail == nullptr) {
		return true;
	}
	else {
		return false;
	}
}

//returns the number of items stored in the Deque
template<class T>
int Deque<T>::size()
{
	return dqsize;
}
