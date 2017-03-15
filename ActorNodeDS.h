/*
* File: ActorNodeDS.h
* Author: Alvin Vanegas
* Date:   2/25/17
* Winter 2017
* Overview: Header file for actor nodes 
*/



#ifndef ACTORNODEDS_H
#define ACTORNODEDS_H

#include <iostream>
#include <string>


using namespace std;


/** A class, instances of which arae nodes in the actorGraph.
*/

class ActorNodeDS {

public:
	// Name of actor
	string actorName;	
	// pointer to the previous node
	ActorNodeDS* parent;
	// number of node in tree, use for sentinal node
	int size;
	// constructor
	ActorNodeDS(string actorName);
	
};

#endif