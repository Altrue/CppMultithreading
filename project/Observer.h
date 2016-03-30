#pragma once

#include <vector>
#include <iostream>
#include <string>

using namespace std;
// Présence de la classe Observable et Observer dans ce fichier qui représente le design pattern Observer dans sa globalité.

class Observable;

class Observer
{
public:
	virtual void update(int returnCode, std::string returnPassword) = 0;
};

class Observable
{
public:
	//Liste des ordonnanceurs qui observent
	std::vector<Observer*> list;
	virtual void attach(Observer *o);
	virtual void detach(Observer *o);
	virtual void notify(int returnCode, std::string returnPassword);
};
