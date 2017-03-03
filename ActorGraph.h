/*
 * ActorGraph.h
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <vector>
#include "ActorNode.h"
#include "Movie.h"
#include <unordered_map>
#include <unordered_set>
// Maybe include some data structures here

using namespace std;

class ActorGraph {
protected:
      
    // Maybe add class data structure(s) here

    // will hold actorNodes and use actor names as key
    unordered_map<string, ActorNode*> actorNode_map;
	// Holds movie objects and use movie info as key
	unordered_map<string, Movie*> movies_map;

public:
    ActorGraph(void);

    // Maybe add some more methods here

	// Creates ActorNodes
    unordered_map<string, ActorNode*> createActorNodes();
    
	// Creates edges for each ActorNode
    ActorNode* createEdges(ActorNode*, unordered_set<string>, string);

	// Performs breadth first search starting from one string value to another
    void BFSearch(unordered_map<string, ActorNode*>, string, string );
	
	// Helper function that returns the actornode using the actor name
    ActorNode* getActorNode(string);
	
	// Will print path to console/file
	void printPath(ActorNode*);
	
	// gets each set of pairs and calls BFS function
	bool getActorPairs(const char* in_filename);

    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
  
};


#endif // ACTORGRAPH_H
