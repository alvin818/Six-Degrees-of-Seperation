/*
 * File: actorconnections.cpp
 * Author: Alvin Vanegas
 * Date:   2/25/17
 * Winter 2017
 * Overview: TODO
 */

#include <iostream>
#include <string>
#include "ActorGraph.h"
#include <utility> 
#include <vector>
using namespace std;


/* pathfinder will take in 3 parameters via command line
 	argv[1]: movie_casts.tsv
 	argv[2]: test_pairs.tsv
 	argv[3]: out_connections_bfs.tsv
 	argv[4]: ufind or bfs
*/


int main(int argc, char *argv[]){

	// Check if correct # of parameters passed in
	if(argc != 5){
		cout << "Incorrect number of parameters passed in. Try again." << endl;
		return -1;
	}

	// Get parameters
	const char* movie_casts = argv[1];
	const char* test_pairs = argv[2];
	const char* out_file = argv[3];
	ofstream outFile(out_file, ofstream::out);
	string typeOfSearch = argv[4];

	// Create instance of an actorGraph
    ActorGraph actor_graph;

    // get vector of pairs (actor 1, actor 2)
    vector<pair<string,string>> pairsToSearchFor = actor_graph.getActorPairs(test_pairs);
    

    vector<string> actorsToSearchfor;

    // Add actor pairs to new vector of strings
    for(auto onePair = pairsToSearchFor.begin(); onePair != pairsToSearchFor.end(); onePair++){
    	// push each element of pair to vector
		cout << onePair->first << onePair->second << endl;
		//auto pair = onePair->second;
    	actorsToSearchfor.push_back(get<0>(*onePair));
    	actorsToSearchfor.push_back(get<1>(*onePair));
    }


    // Now find the earliest year to begin building graph
    int yearToStart = actor_graph.findOldestFilmYear(movie_casts, actorsToSearchfor);
    // sanity print statement
    cout << "Earliest movie year in actors to search for is: " << yearToStart << endl;

    /*
		Using this year, a graph can be built using all movies starting from this year and forward.
		Create movie objects for each year and place into priority queue that orders the objects from 
		increasing year. 
    */

	actor_graph.createMovieObjects(movie_casts, yearToStart);

	// map will contain movie objects, now add to priority queue
	actor_graph.addMovieObjects();

	/*
		Now build graph using queue of movie objects
	*/
		
	// will hold pairs that have been found to avoid pointless searches
	unordered_map<string, pair<string, string>> foundPairs;

	// holds strings of the pairs and year they connected
	unordered_map<string, string> connections;

	// Do BFS on pairs until all pairs have been found or the year exceeds 2015
	while (yearToStart < 2016){		
		// create add actor nodes for given year
		actor_graph.createGraph(yearToStart);

		// run BFS on all pairs
		for (auto onePair = pairsToSearchFor.begin(); onePair != pairsToSearchFor.end(); onePair++){
			// if search was successful then keep track of year and connect it with the pair
			string pairKey = onePair->first + onePair->second;
			unordered_map<string, pair<string, string>>::iterator got = foundPairs.find(pairKey);
			if (got == foundPairs.end()){				
				// Match was found
				if (actor_graph.BFSearchAC(get<1>(*onePair), get<0>(*onePair))) {
					cout << "Earliest connection for pairs:" << get<1>(*onePair) << " " << get<0>(*onePair) << " "<< yearToStart << endl;
					foundPairs[pairKey] = *onePair;
					string pairAndYear = get<0>(*onePair) + "\t" + get<1>(*onePair) + "\t" + to_string(yearToStart);
					connections[pairKey] = pairAndYear;				
				}
			}				
		}
		// once all pairs have been found break from loop
		if (foundPairs.size() == pairsToSearchFor.size()){
			cout << "All pairs have been found" << endl;
			break;
		}
		// increment year until all pairs have been found
		yearToStart++;
	}	
	// print results to file 
	actor_graph.printActorConnections(connections, pairsToSearchFor, outFile);


	return 0;
}
