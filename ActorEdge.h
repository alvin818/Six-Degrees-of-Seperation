/* 
   Name: Alvin Vanegas
   Date: 2/25/17
   Overview:
*/
#ifndef ACTOREDGE_H
#define ACTOREDGE_H


#include <iostream>
#include <string>
#include <vector>
using namespace std;

class ActorEdge {

public:

	/* 
	   Edge will contain a pair: 
	   movie (name + year) & name of costar 
	*/
	string movieInfo;
	// name of co-star in film
	string coStarName;

	ActorEdge(string movie, string name);
};
#endif