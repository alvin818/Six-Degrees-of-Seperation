/* 
   File: ActorEdge.cpp
   Name: Alvin Vanegas
   Date: 2/25/17
   Overview: Bascially the constructor for an actorEdge object
*/

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "ActorEdge.h"
   using namespace std;

/*
	Constructor for ActorEdges objects
*/
	ActorEdge::ActorEdge(string movie, string name){	
	// Movie name + year
		movieInfo = movie;
	// Name of actor 
		coStarName = name;
	}