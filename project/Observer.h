#ifndef OBSERVER_H_INCLUDED
#define OBSERVER_H_INCLUDED

// Class observer, it notify that the daughter class must have an update method
// Instance of this interface have to register themselves to subject and also unregister.
class Observer
{
public:
	virtual ~Observer(); // Have to declare the Observer virtual, in order to remove priority
	virtual void update() = 0;
};

#endif