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
  
  bool weightedEdges = false;
  
	if(argc != 4){
		cout << "Incorrect number of paramters" << endl;
		return -1;
	}
  

  ActorGraph actor_graph;
  const char* in_filename = argv[1];
  //if(argv[2] == 'w'){
    //weightedEdges = true;
  //}

	// call ActorGraph on movie_cast
	if(!actor_graph.loadFromFile( in_filename, weightedEdges)){
		// return -1 if loadFromFile returns false
		return -1;
	}
  	
  unordered_map<string, ActorNode*> map = actor_graph.createActorNodes();
/*
  // print the actor nodes
  for(auto currNode = map.begin(); currNode != map.end(); currNode++){
    ActorNode *actor_node = currNode->second;
    cout << "Actor name of node : " << actor_node->actorName << endl;
    // print edges
    unordered_set<ActorEdge*> edgeList = actor_node->movieEdges;
    cout << "Actor has connections with the following actors:" << endl;
    for (auto itr = edgeList.begin(); itr != edgeList.end(); ++itr){
      cout << (*itr)->coStarName << " in film: " << (*itr)->movieInfo << endl;
    }
    cout << endl;

  }
  */
  cout << "Entering BFS....." << endl;
  const char* pairFiles = argv[3];

  if (!actor_graph.getActorPairs(pairFiles)){
	  return -1;  
  }

	/* now use argv[3]: test_pairs.tsv argv[4]: out_paths_unweighted 
       to do breadth first search
	*/


	
	return 0;
}
