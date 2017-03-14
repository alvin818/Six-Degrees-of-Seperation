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

	ActorNodeDS(string actorName);
	
};

#endif