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

	string movieName;
	int movieYear;

	// this will be updated when the graph & edges is created
	string coStarName;

	ActorEdge(string name, int year);
};
#endif