#ifndef SUBJECT_H_INCLUDED
#define SUBJECT_H_INCLUDED

#include <vector>
#include "Observer.h"

// An abstract class that implements the notification and registers methods for its observers
class Subject
{
	public:
		virtual ~Subject();
		void notify();
		void addObserver(Observer &obs);
		void removeObserver(Observer &obs);
	private:
		std::vector<Observer*> observers; //Array of observer's pointers
};

#endif