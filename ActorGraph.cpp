/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "ActorGraph.h"
#include "ActorNode.h"
#include "ActorEdge.h"
#include "Movie.h"
#include <queue>

using namespace std;

ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
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

		// Combine movie title and year into string variable
		string movieKey = (movie_title + " " + to_string(movie_year));
		// exmaple: "Memento 2000" would be key for this movie object
		/*  Create movie class objects with given values
			Use Movie title + year as key for unordered_map
			Put the movie object as value
			Use key to make sure movie object has not been made

			*/

		// First check if movie object has not been made for input
		unordered_map<string, Movie*>::const_iterator got = movies_map.find(movieKey);

		// If key is not found then create new movie object and add it to map
		if (got == movies_map.end()){
			Movie *newMovie = new Movie(movieKey);
			newMovie->_actors.insert(actor_name);
			pair<string, Movie*> moviePair(movieKey, newMovie);
			movies_map.insert(moviePair);
		}
		// Add actor to movie object that has been created
		else{
			Movie *movieObject = got->second;
			movieObject->_actors.insert(actor_name);
		}
	
		/* 
		   At this point the unordered_map of movie objects has been filled. 
		   Now create the actorNodes using this data.
		*/

		// CreateActorNodes

    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    cout << endl;
    return true;
}


/* 
   Will use the map of movie object to create actor nodes
   1. Will retrieve the first movie obj, access the objects set of actors
   2. First check if an actor node has not been created for it already
   3. if not then create a node for actor and create an actor edge with the rest of the list
   4. if node has been created then add to the nodes edge list with the rest of the cast
   5. Repeat steps 2 - 4 for the whole list of actors 
   6. Reinsert the node into the has table/map
*/
unordered_map<string, ActorNode*> ActorGraph::createActorNodes(){

	// For each movie object, do the following....
	for (auto it = movies_map.begin(); it != movies_map.end(); ++it){

		Movie *currMovieObj = it->second;
		string movieName = it->first;

		unordered_set<string> currActorList = currMovieObj->_actors;
		
		// For each actor in actor list, do the following....
		for (auto actor = currActorList.begin(); actor != currActorList.end(); ++actor){
			// Search actorNode map
			unordered_map<string, ActorNode*>::iterator got = actorNode_map.find(*actor);
			
			// if node for actor does exist then create a new node
			if (got == actorNode_map.end()){
				ActorNode *newActor = new ActorNode(*actor);
				//call create edges, pass in actor node and
				newActor = createEdges(newActor, currActorList, movieName);
				// Create pair: key(actor name) and new actor node
				pair<string, ActorNode*> actorPair(*actor, newActor);
				actorNode_map.insert(actorPair);
			}
			
			// Node exists, simply update current actors node with edges
			else{
				ActorNode *existingActor = got->second;
				existingActor = createEdges(existingActor, currActorList, movieName);
				// Update the actor node already in map
				got->second = existingActor;
			}
		}
	
	
	}


return actorNode_map; 
}

/*Created edges for the given actor node using the list from the movie object*/
ActorNode* ActorGraph::createEdges(ActorNode* node, unordered_set<string> actorList, string movieName){

	for (auto actor = actorList.begin(); actor != actorList.end(); actor++){
		// create new edge object
		if (*actor != node->actorName){
			ActorEdge *newEdge = new ActorEdge(movieName, *actor);
			// Add new edge to actor node
			node->movieEdges.insert(newEdge);
		}
	
	}

	return node;
}

/*
	Function performs breadth first search using a queue of actor nodes.
	Will print the path to a file.
*/
void ActorGraph::BFSearch(unordered_map<string, ActorNode*> actor_map, string startActor, string actorToFind){

    // get node using actors name
    ActorNode* currActorNode = getActorNode(startActor);
    // Make parent ptr of first node null
    currActorNode->parent = nullptr;
    // create queue of actorNodes
    queue<ActorNode*> actorQueue;
    // push first node into queue
    actorQueue.push(currActorNode);
    // have a set containing actor nodes already put into the queue
    unordered_set<ActorNode*> seenActorNodes;
	// map of visited edges using actor name as key
    unordered_map<string, string> seenActorEdges;
    seenActorNodes.insert(currActorNode);

    while(!actorQueue.empty()){
        currActorNode = actorQueue.front();
        actorQueue.pop();
        // now get all adjacent actors via edges
        auto edge = currActorNode->movieEdges.begin();
        // for each name in edge, get node and add to queue
        for(; edge != currActorNode->movieEdges.end(); ++edge){    

            // get actor node using name from edge list
            ActorNode* childrenNode = getActorNode((*edge)->coStarName);
            // check if node is the actor being searched for 
            /*if(childrenNode->actorName == actorToFind){
                cout << "Actor has been found!" << endl;
                childrenNode->parent = currActorNode;
				// print path function.
				printPath(seenActorEdges, childrenNode);
                return;
            }*/
			// See if node had been seen before
            unordered_set<ActorNode*>::iterator got = seenActorNodes.find(childrenNode);
            // push node to queue if not found in set
            if(got == seenActorNodes.end()){
			// Create an edge pair with costar name and movie info
            pair<string, string> edgePair((*edge)->coStarName,(*edge)->movieInfo);
            // insert into map
			seenActorEdges.insert(edgePair);
			childrenNode->parent = currActorNode;
			seenActorNodes.insert(childrenNode);
            actorQueue.push(childrenNode);
            }

            if(childrenNode->actorName == actorToFind){
                cout << "Actor has been found!" << endl;
                childrenNode->parent = currActorNode;
				// print path function.
				printPath(seenActorEdges, childrenNode);
                return;
            }


        }

    }




}

/*Helper function to retrieve the actor's node using the key (actor name)*/
ActorNode* ActorGraph::getActorNode(string actorName){
    // get starting actor node using startactor
    unordered_map<string, ActorNode*>::iterator node = actorNode_map.find(actorName);
    // first actor node 
    ActorNode* actor_node = node->second;
    // return node
    return actor_node;

}

/* 
	Function prints the path of the breadth first search using the 
	nodes parent node. Traverse the parent nodes until the parent pointer
	is null;
*/
void ActorGraph::printPath(unordered_map<string, string> edgeMap, ActorNode* lastActorNode){

	cout << "Printing the path of the breadth first search......." << endl;
	ActorNode *curr = lastActorNode;
  	
  while (curr){	
    // get parents actor name to get the mutual film 

    cout<< curr->actorName << "\t";
    //curr = curr->parent;
    unordered_map<string, string>::iterator got = edgeMap.find(curr->actorName);
    if(got != edgeMap.end()){
      cout << got->second <<"\t";
    }
    curr = curr->parent;

	}
  
  cout << endl;
}

bool ActorGraph::getActorPairs(const char* in_filename){
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

		if (record.size() != 2) {
			// we should have exactly 3 columns
			continue;
		}

		string actor_1(record[0]);
		string actor_2(record[1]);
    
    cout << "Pair to find: " << actor_1 << "\t" << actor_2 << endl;
		// Call breadth first search using two actors 
		BFSearch(actorNode_map, actor_2, actor_1);
			
	}

	if (!infile.eof()) {
		cerr << "Failed to read " << in_filename << "!\n";
		return false;
	}
	infile.close();

	return true;	
}
