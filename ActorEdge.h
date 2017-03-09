/* 
   File: ActorEdge.h
   Name: Alvin Vanegas
   Date: 2/25/17
   Overview: This class represents the edges in the graph. Each edge contains the 
   movie info and the name of the costar
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

	// Weight of the edge
	   int weight;


	   ActorEdge(string movie, string name, int weight);
	};

#endif