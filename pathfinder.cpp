/*
 * pathfinder.cpp
 * Author: Alvin Vanegas
 * Date:   2/25/17
 *
 * Overview: TODO 
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

	if(argc != 5){
		cout << "" << endl;
		return -1;
	}

	ActorGraph actor_graph;
	// call ActorGraph on movie_cast
	if(!actor_graph.loadFromFile( argv[1], argv[2])){
		// return -1 if loadFromFile returns false
		return -1;
	}

	vector<ActorNode*>graph = actor_graph.createGraph();

	/* now use argv[3]: test_pairs.tsv argv[4]: out_paths_unweighted 
       to do breadth first search
	*/


	
	return 0;
}