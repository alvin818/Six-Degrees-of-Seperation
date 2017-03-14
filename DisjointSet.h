/*
 Name:
 TODO
*/


#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include "MovieDS.h"
#include "ActorNodeDS.h"
#include <unordered_map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// Class for the priority queue
class MovieDSPtrComp {
public:
	bool operator()(MovieDS*& lhs, MovieDS*& rhs) const {
		return *lhs < *rhs;
	}
};



class DisjointSet {
protected:

	// Map will hold movies already created
	unordered_map<string, MovieDS*> movies;

	// will hold movie objects in increasing movie year
	priority_queue<MovieDS*, vector<MovieDS*>, MovieDSPtrComp> movieObjects;

	/* 
		map will hold pointers to actor nodes, if the actor name is equal to key then it is a sentinal node
		else its parent pointer will point to parent/sentinal node
	*/
	unordered_map<string, ActorNodeDS*> mapOfSets;


public:
	DisjointSet(void);
	
	// Function will will create film object starting from given year
	bool getMoviesFromFile(const char* in_filename, int startYear);

	// will use the movies map and insert all into prioirty queue
	void addMoviesToPQ();

	// Function unifies two sentinal nodes of two given actors, will be called if there is a connection
	void set_union(string actor_1, string actor_2);

	// function will find the sentinal node of give actor, will be used for searching for two pairs
	ActorNodeDS* find(string actorName);

	// Function will get films for a certain year and create sets for actors
	void createSets(vector<string> titles);

	vector<string> getMoviesFromYear(int currYear);

	MovieDS* getMovie(string key);

	void searchActorList(vector<string> actors);

	void connectSets(vector<string> actors);

};







#endif