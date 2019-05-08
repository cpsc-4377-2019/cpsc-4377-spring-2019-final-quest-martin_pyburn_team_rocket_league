#ifndef PLANETOIDFACTORY_H
#define PLANETOIDFACTORY_H

#include"ObjectFactory.h"
#include"Planetoid.h"
#include<memory>
#include<vector>

//#include<iostream>
class PlanetoidFactory : ObjectFactory {
public:
	PlanetoidFactory(Library* lib);
	Object* create(tinyxml2::XMLElement* objectElement);

private:
	static shared_ptr<Planetoid> attachChildren(
		shared_ptr<Planetoid> Parent,
		vector<const char*> ChildLists,
		vector<shared_ptr<Planetoid>> PlanetoidArray,
		int index,
		shared_ptr<Texture> img
	);
};

#endif