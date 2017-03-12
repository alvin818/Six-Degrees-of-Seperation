/*
 * File: pathfinder.cpp
 * Author: Alvin Vanegas
 * Date:   2/25/17
 * Winter 2017
 * Overview: This program will first create a graph of actor nodes using a file
   of movies casts.
   Using this graph the program will also use breadth first search to find the shortest 
   path between two actors
 */

#include <iostream>
#include "ActorGraph.h"
using namespace std;

 /* pathfinder will take in 3 parameters via command line
 	argv[1]: movie_casts.tsv
 	argv[2]: u (unweighted) w (weighted)
 	argv[3]: test_pairs.tsv
 	argv[4]: out_paths_unweighted
*/

  int main(int argc, char *argv[]){

    bool weightedEdges = false;

    if(argc != 5){
      cout << "Incorrect number of parameters" << endl;
      return -1;
    }    

    // Create instance of an actorGraph
    ActorGraph actor_graph;
    // get input file
    const char* in_filename = argv[1];

	char *inputChar = argv[2];

	if(*inputChar == 'w'){
    cout << "Creating graph with weighted edges" << endl;
		weightedEdges = true;
	}

	  // call ActorGraph on movie_cast
    if(!actor_graph.loadFromFile( in_filename, weightedEdges)){
		// return -1 if loadFromFile returns false
      return -1;
    }

    cout << "Creating actor graph...... ";
  // map will have connected graph
    actor_graph.createActorNodes();
    cout << "DONE" << endl;

    cout << "Entering BFS....." << endl;

  // Get the pair file and output file from parameters
    const char* pairFiles = argv[3];
    const char* outfile = argv[4];

  // Open output file   
    ofstream outFile(outfile, ofstream::out);

  // Print header to output file
    outFile << "(actor)--[movie#@year]-->(actor)--..." << endl;

  // Enterting breadth first search 
    if (!actor_graph.getActorPairs(pairFiles, outFile, weightedEdges)){
     cout << "Loading in pairs failed..... exiting...." << endl;
     return -1;  
   }

  // close output file
   outFile.close();

   return 0;
 }
