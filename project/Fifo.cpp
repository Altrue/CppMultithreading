#include "Fifo.h"

template<typename T>
Fifo<T>::Fifo()
{
	this->_que.clear();
}

template<typename T>
Fifo<T>::~Fifo()
{
}

template<typename T>
void Fifo<T>::push(T element) {
	this->_mtx.lock();
	this->_que.push_back(element);
	this->_mtx.unlock();
}

template<typename T>
T Fifo<T>::pull() {
	this->_mtx.lock();
	T element = this->_que.front();
	this->_que.pop_front();
	this->_mtx.unlock();
	return element;
}

template<typename T>
int Fifo<T>::getSize() {
	return this->_que.size
}
