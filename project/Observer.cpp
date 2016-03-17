#include "Observer.h"
#include <algorithm>

using namespace std;

void Observable::attach(Observer * o)
{
	list.push_back(o);
}
void Observable::detach(Observer * o)
{
	list.erase(std::remove(list.begin(), list.end(), o), list.end());
}

void Observable::notify(int returnCode, std::string returnPassword)
{
	for (vector<Observer*>::const_iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		if (*iter != 0)
		{
			(*iter)->update(returnCode, returnPassword);
		}
	}
}
