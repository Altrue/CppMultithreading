#pragma once

#include <deque>
#include "FMutex.h"

template<typename T>
class Fifo
{
private:
	FMutex _mtx;
	std::deque<T> _que;                                                                                 

public:
	Fifo();
	~Fifo();
	//ajoute un élément à la fin
	void push(T element);
	//récupère le premier élément
	T pull();
	//renvoi la taille de la fifo
	int getSize();
};

