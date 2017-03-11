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
		cout << "Incorrect number of parameters passed in. Try again."
		return -1;
	}

	// Get parameters
	const char* movie_casts = argv[1];
	const char* test_pairs = argv[2];
	const char* out_file = argv[3];
	string typeOfSearch = argv[4];

	// Create instance of an actorGraph
    ActorGraph actor_graph;








	return 0;
}