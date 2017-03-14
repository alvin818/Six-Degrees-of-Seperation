/*
* ActorGraph.cpp
* Author: Alvin Vanegas
* Date:   3/3/17
*
* This file is meant to exist as a container for starter code that you can use to read the input file format
* defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
*/

#include "ActorNodeDS.h"
#include "MovieDS.h"
#include "DisjointSet.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;


DisjointSet::DisjointSet(void) {}


// Function will will create film object starting from given year
bool DisjointSet::getMoviesFromFile(const char* in_filename, int startYear){
	// Initialize the file stream
	ifstream infile(in_filename);

	bool have_header = false;

	// keep reading lines until the end of file is reached
	while (infile) {
		string s;

		// get the next line
		if (!getline(infile, s)) break;
		if (!have_header) {
			// skip the header
			have_header = true;
			continue;
		}

		istringstream ss(s);
		vector <string> record;

		while (ss) {
			string next;

			// get the next string before hitting a tab character and put it in 'next'
			if (!getline(ss, next, '\t')) break;

			record.push_back(next);
		}

		if (record.size() != 3) {
			// we should have exactly 3 columns
			continue;
		}

		string actor_name(record[0]);
		string movie_title(record[1]);
		int movie_year = stoi(record[2]);	

		if (movie_year >= startYear){

			// Combine movie title and year into string variable
			string movieKey = (movie_title + to_string(movie_year));

			// First check if movie object has not been made already to avoid repeat movie objects
			unordered_map<string, MovieDS*>::const_iterator got = movies.find(movieKey);

			// If key is not found then create new movie object and add it to map
			if (got == movies.end()){
				MovieDS *newMovie = new MovieDS(movie_title, movie_year);				
				newMovie->_actors.push_back(actor_name);
				movies[movieKey] = newMovie;
			}

			// Add actor to movie object returned by the iterator
			else{

				MovieDS *movieObject = got->second;
				movieObject->_actors.push_back(actor_name);
			}

			/*
			At this point the unordered_map of movie objects has been filled.
			Now create the actorNodes using this data.
			*/
		}
	}

	if (!infile.eof()) {
		cerr << "Failed to read " << in_filename << "!\n";
		return false;
	}

	infile.close();
	cout << endl;
	return true;

}

// will use the movies map and insert all into prioirty queue
void DisjointSet::addMoviesToPQ(){

	for (auto currMovie = movies.begin(); currMovie != movies.end(); currMovie++){		
		movieObjects.push(currMovie->second);	
	}	
}

// Function unifies two sentinal nodes of two given actors, will be called for each 2 nodes in a given movie
void DisjointSet::set_union(string actor_1, string actor_2){
	
	string sentNode1 = find(actor_1);
	string sentNode2 = find(actor_2);

	cout << "Sentinal node for actor 1: " << sentNode1 << endl;
	cout << "Sentinal node for actor 2: " << sentNode2 << endl;

	// already in the same set
	if (sentNode1 == sentNode2){
		return;
	}
	
	
	// get sentinal nodes of each actor
	unordered_map<string, ActorNodeDS*>::iterator it_1 = mapOfSets.find(sentNode1);
	unordered_map<string, ActorNodeDS*>::iterator it_2 = mapOfSets.find(sentNode2);

	if (it_1 == mapOfSets.end()){
		cout << "FUCK" << endl;
	}

	ActorNodeDS *sentinalNode1 = it_1->second;
	ActorNodeDS *sentinalNode2 = it_2->second;
	

	// now compare sizes 

	// if size of one node is greater then set the smallers parent to larger node
	// add size of smaller node to larger node
	if (sentinalNode1->size > sentinalNode2->size){
		sentinalNode2->parent = sentinalNode1;
		sentinalNode1->size += sentinalNode2->size;
	}
	else if (sentinalNode1->size < sentinalNode2->size){
		sentinalNode1->parent = sentinalNode2;
		sentinalNode2->size += sentinalNode1->size;
	}
	// sizes are equal, tie is given to first sentinal node
	else{
		sentinalNode2->parent = sentinalNode1;
		sentinalNode1->size += 1;
	}
}

// function will find the sentinal node of give actor, will be used for searching for two pairs
string DisjointSet::find(string actorName){

	unordered_map<string, ActorNodeDS*>::iterator actorSet = mapOfSets.find(actorName);
	ActorNodeDS *currNode = actorSet->second;

	// node is a sentinal node
	if (currNode->parent->actorName == actorName){
		return actorName;
	}

	// recursively call find on parents name until sentinal node reached
	currNode->parent->actorName = find(currNode->parent->actorName);
	
	// return name
	return currNode->parent->actorName;

}

// Function will get films for a certain year and create sets for actors
void DisjointSet::createSets(vector<string> movie_titles){
	
	// For each movie title, get movie object
	for (auto movieIt = movie_titles.begin(); movieIt != movie_titles.end(); movieIt++){
		// get movie object
		MovieDS* movie = getMovie(*movieIt);
		cout << "Creating sets with actors from film: " << movie->movieName << endl;
		cout << endl;
		// get actor list
		vector<string> actorList = movie->_actors;

		/* 
			Now check if a set has been made for given actor in actor list
			for actors that haven't had a set made for, make set and connect them
			if one of the actors has a set then union them 
		*/
		
		searchActorList(actorList);
		// call union on each actor pair
		connectSets(actorList);
	}


}

vector<string> DisjointSet::getMoviesFromYear(int year){

	vector<string> movieTitles;
	// flag will be true once all movies from given year have been popped off
	bool finished = false;
	
	MovieDS *currMovie;
	// pop all movies from given year and return the vector of movies names
	while (!movieObjects.empty() && !finished){
		// pop top movie object
		currMovie = movieObjects.top();
		// check movie year before popping
		if (currMovie->movieYear == year){
			movieObjects.pop();
			string movieKey = currMovie->movieName + to_string(currMovie->movieYear);
			movieTitles.push_back(movieKey);		
		}
		// stop popping movie objects
		else{
			finished = true;		
		}
	
	}
	return movieTitles;

}


MovieDS* DisjointSet::getMovie(string key){
	return movies[key];
}


/*
	Searches each movies actor list to see if a set has been made for it
*/
void DisjointSet::searchActorList(vector<string> actorList){

	// for each actor in single film do the following....
	for (auto actor_it = actorList.begin(); actor_it != actorList.end(); actor_it++){
	
		unordered_map<string, ActorNodeDS*>::iterator actorSet = mapOfSets.find(*actor_it);
		
		// if it doesnt exit then create sentinal node
		if(actorSet == mapOfSets.end()){
			ActorNodeDS *newActorNode = new ActorNodeDS(*actor_it);
			cout << "Creating single set for actor : " << *actor_it << endl;
			// set its parent to itself
			newActorNode->parent = newActorNode;
			// add to map
			mapOfSets[*actor_it] = newActorNode;			
		}
		// actor already has a set made, iterator hold actor node and key
		else{
			cout << "Set for actor: "<< actorSet->first << " already exits" << endl;
		}	
	}

}

void DisjointSet::connectSets(vector<string> actorList){
	// Now connect the sets using first two actors from list
	for (unsigned int i = 1; i < actorList.size(); i++){
		string actor1 = actorList.at(i - 1);
		string actor2 = actorList.at(i);

		cout << "calling union on the following two actors: " << actor1 << " " << actor2 << endl;

		// set union between two actors
		set_union(actor1, actor2);
	
	}

}