#include "Subject.h"

using namespace std;

Subject::~Subject()
{

}

void Subject::notify()
{
	int i = 0;

	for (i = 0; i < observers.size(); i++) {
		observers[i]->update();
	}
}

void Subject::addObserver(Observer &obs)
{
	observers.push_back(&obs);
}

void Subject::removeObserver(Observer &obs)
{
	vector<Observer*> copyTab = observers;
	observers.clear();
	int i = 0;

	for (i = 0; i < copyTab.size(); i++) {
		if (copyTab[i] != &obs) {
			observers.push_back(&obs);
		}
	}
}