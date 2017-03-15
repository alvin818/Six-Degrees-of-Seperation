/*
 * ActorGraph.h
 * Author: Alvin Vanegas
 * Date:   3/3/2017
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H
#include <fstream> 
#include <iostream>
#include <vector>
#include "ActorNode.h"
#include "Movie.h"
#include <unordered_map>
#include <unordered_set>
#include <queue>
// Maybe include some data structures here

 using namespace std;


/** A 'function class' for use as the Compare class in a
 *  priority_queue<ActorNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */

 class ActorNodePtrComp {
public:
    bool operator()(ActorNode*& lhs, ActorNode*& rhs) const {
        return *lhs < *rhs;
    }
};


class MoviePtrComp {
public:
    bool operator()(Movie*& lhs, Movie*& rhs) const {
        return *lhs < *rhs;
    }
};

 class ActorGraph {
 protected:   

    // will hold actorNodes and use actor names as key
    unordered_map<string, ActorNode*> actorNode_map;

	// Holds movie objects and use movie info as key
    unordered_map<string, Movie*> movies_map;

	// Holds set of visited actorNodes
	unordered_set<string> seenActorNodes;

    // will hold movie objects for actor connections
    priority_queue<Movie*, vector<Movie*>, MoviePtrComp> movieObjects;

	// Holds edges created
	unordered_set<ActorEdge*> edges;


public:
    ActorGraph(void);

	// Destructor
	~ActorGraph();
    
	// Creates ActorNodes
    void createActorNodes();
    
	// Creates edges for each ActorNode and fully connects the graph
    ActorNode* createEdges(ActorNode*, unordered_set<string>, string, int);

	// Performs breadth first search starting from one string value to another
    bool BFSearch(string, string, ofstream&);
    
	// Helper function that returns the actornode using the actor name
    ActorNode* getActorNode(string);
    
	// Will print path to file
    void printPath(unordered_map<string, string>, ActorNode*, ofstream&);
    
	// gets each set of pairs and calls BFS function
    bool getActorPairs(const char* in_filename, ofstream&, bool use_weighted_edges);

    // Will get each pair being searched for and put into a vector
    vector<pair<string, string>> getActorPairs(const char* actorPairs);

    // Will get the oldest movie with it corresponding actor 
    int findOldestFilmYear(const char* movie_casts, vector<string> actors);

    void createMovieObjects(const char* movie_casts, int startingYear);

    // method wil perform breadth first search using dijkstra's algorithm
    void dijkstraSearch(string, string, ofstream&);

	// writes path to file
	void printWeightedPath(unordered_map<string, string>, ActorNode*, ofstream&);
	
	// Resets actor nodes for weighted BFS
	void resetNodes(unordered_set<string>);

	// Adds movies to priority queue
    void addMovieObjects();
	
	// Creates nodes using films made in given year
    void createGraph(int startYear);

	// Prints to output file
	void printActorConnections(unordered_map<string, string>, vector<pair<string, string>>, ofstream&);

	// BFS for actor connections
	bool BFSearchAC(string startActor, string actorToFind);

    /** 
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
