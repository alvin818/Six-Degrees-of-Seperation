/* 
   Name: Alvin Vanegas
   Date: 2/25/17
   Overview: this class represents the nodes in the graph. Each instance of this 
   class will have the actor name, a pointer to the parent, and a set of edges
   aka movies 
*/

#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include "Movie.h"
#include "ActorEdge.h"

   using namespace std;


/** A class, instances of which arae nodes in the actorGraph.
*/

   class ActorNode {

   public:

	// Name of actor
   	string actorName;
   	
	// ajancency list of edges (movies) 
   	unordered_set<ActorEdge*> movieEdges;

   	ActorNode* parent;
   	
	// initialize node with actor name
   	ActorNode(string name);
   };

#endif