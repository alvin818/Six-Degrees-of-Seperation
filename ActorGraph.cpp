/*
 * ActorGraph.cpp
 * Author: Alvin Vanegas
 * Date:   3/3/17
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
#include <climits>
#include <utility> 
using namespace std;

ActorGraph::ActorGraph(void) {}

/*
   Takes in a file and extracts the actors name, movie name and movie year
   */
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

    actor_name = ("(" + actor_name + ")");


    // Combine movie title and year into string variable
    string movieKey = ("[" + movie_title + "#@" + to_string(movie_year) + "]");

    /*  
        Create movie class objects with given values
        Use Movie title + year as key for unordered_map
        Put the movie object as value
        Use key to make sure movie object has not been made
		exmaple: "Memento#@2000" would be key for this movie object
	*/

    // First check if movie object has not been made already to avoid repeat movie objects
    unordered_map<string, Movie*>::const_iterator got = movies_map.find(movieKey);

    // If key is not found then create new movie object and add it to map
    if (got == movies_map.end()){
      Movie *newMovie = new Movie(movie_year, movieKey);
      newMovie->_actors.insert(actor_name);

      // Make a pair with movie name and movie object
      pair<string, Movie*> moviePair(movieKey, newMovie);

      // insert the pair into the movie map
      movies_map.insert(moviePair);
    }

    // Add actor to movie object returned by the iterator
    else{
      Movie *movieObject = got->second;
      movieObject->_actors.insert(actor_name);
    }

    /*
       At this point the unordered_map of movie objects has been filled.
       Now create the actorNodes using this data.
       */

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

void ActorGraph::createActorNodes(){

  // For each movie object, do the following....
  for (auto it = movies_map.begin(); it != movies_map.end(); ++it){

    Movie *currMovieObj = it->second;
    string movieName = it->first;
    int movie_year = currMovieObj->movieYear;

    // Get actor list from the movie object
    unordered_set<string> currActorList = currMovieObj->_actors;

    // For each actor in actor list, do the following....
    for (auto actor = currActorList.begin(); actor != currActorList.end(); ++actor){

      // Search actorNode map for the current actor to avoid creating duplicate actor nodes
      unordered_map<string, ActorNode*>::iterator got = actorNode_map.find(*actor);

      // if node for actor does exist then create a new node
      if (got == actorNode_map.end()){

        // Create new actor obj with actor name
        ActorNode *newActor = new ActorNode(*actor);

        //call create edges, pass in actor node, list of actors, movie name + year
        newActor = createEdges(newActor, currActorList, movieName, movie_year);

        // Create pair: key(actor name) and new actor node
        pair<string, ActorNode*> actorPair(*actor, newActor);
        actorNode_map.insert(actorPair);
      }

      // Node exists, simply update current actors node with edges
      else{

        ActorNode *existingActor = got->second;
        existingActor = createEdges(existingActor, currActorList, movieName, movie_year);
        // Update the actor node already in map
        got->second = existingActor;
      }
    }


  }
  // return fully connected graph w/ weighted edges
  //return actorNode_map;
}

/*
   Helper function that created edges for the given actor node using the list from the movie object
   */
ActorNode* ActorGraph::createEdges(ActorNode* node, unordered_set<string> actorList, string movieName, int movie_year){

  for (auto actor = actorList.begin(); actor != actorList.end(); actor++){
    // create new edge object
    if (*actor != node->actorName){
      int weight = 1  + (2015 - movie_year);
      ActorEdge *newEdge = new ActorEdge(movieName, *actor, weight);
      // Add new edge to actor node
      node->movieEdges.insert(newEdge);
    }

  }

  return node;
}

/*
   Function performs breadth first search using a queue of actor nodes.
   Will print the path to a file.
   Only actor nodes that have not been visited will be put into the queue to avoid repeat
   visits
   */
void ActorGraph::BFSearch(string startActor, string actorToFind, ofstream& outFile){

  cout << "Entering unweighted graph search..... " << endl;
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
  // insert the starting node into the set 
  seenActorNodes.insert(currActorNode);

  // Beginning of BFS
  while (!actorQueue.empty()){
    // get actor node from the front of the queue
    currActorNode = actorQueue.front();
    // pop it
    actorQueue.pop();
    // now get all adjacent actors via edges
    auto edge = currActorNode->movieEdges.begin();
    // for each name in edge, get node and add to queue
    for (; edge != currActorNode->movieEdges.end(); ++edge){

      // get actor node using name from edge list
      ActorNode* childrenNode = getActorNode((*edge)->coStarName);

      // See if node had been seen before
      unordered_set<ActorNode*>::iterator got = seenActorNodes.find(childrenNode);
      // push node to queue if not found in set
      if (got == seenActorNodes.end()){
        // Create an edge pair with costar name and movie info
        pair<string, string> edgePair((*edge)->coStarName, (*edge)->movieInfo);
        // insert into map
        seenActorEdges.insert(edgePair);
        // Set parent to current node
        childrenNode->parent = currActorNode;
        // insert node into set and push into queue
        seenActorNodes.insert(childrenNode);
        actorQueue.push(childrenNode);
      }
      // Now check if current node is the node being searched for
      if (childrenNode->actorName == actorToFind){
        // print path function.
        printPath(seenActorEdges, childrenNode, outFile);
        // exit the function, no more need for searching
        cout << "done" << endl;
        return;
      }
    }

  }
  cout << "LOL sorry connection you are searching for does not exist, try again buddy" << endl;
}

/*
   Helper function to retrieve the actor's node using the key (actor name)
   */
ActorNode* ActorGraph::getActorNode(string actorName){
  // get starting actor node using startactor
  unordered_map<string, ActorNode*>::iterator node = actorNode_map.find(actorName);
  // first actor node 
  ActorNode* actor_node = node->second;
  // return nodek
  return actor_node;
}

/*
   Function prints the path of the breadth first search using the
   nodes parent node. Traverse the parent nodes until the parent pointer
   is null;
   */

void ActorGraph::printPath(unordered_map<string, string> edgeMap, ActorNode* lastActorNode, ofstream& outfile){

  ActorNode *curr = lastActorNode;
  // End loop once curr is null
  while (curr){
    // get parents actor name to get the mutual film 
    unordered_map<string, string>::iterator got = edgeMap.find(curr->actorName);
    if (got != edgeMap.end()){
      //cout << got->second <<"\t";
      outfile << got->first << "--";
      outfile << got->second << "-->";
    }
    else{
      outfile << curr->actorName;
    }
    curr = curr->parent;

  }
  outfile << endl;

  cout << endl;
}

/*	
    Reads in pair file and seperates the pairs into two strings
    */
bool ActorGraph::getActorPairs(const char* in_filename, ofstream& out_filename, bool weightedEdges){
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

    if (record.size() != 2){
      continue;
    }


    string actor_1(record[0]);
    string actor_2(record[1]);

    actor_1 = ("(" + actor_1 + ")");
    actor_2 = ("(" + actor_2 + ")");


    cout << "Pair to find: " << actor_1 << "\t" << actor_2 << endl;
    /*
       Have conditional statement to do BFS 
       */

    // use dijstrkaskdfjskadf algorithm
    if (weightedEdges){
      dijkstraSearch(actor_2, actor_1, out_filename); 
    }
    else{
      BFSearch(actor_2, actor_1, out_filename);
    }


  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  cout << "#nodes: " << actorNode_map.size() << endl;
  cout << "#movies: " << movies_map.size() << endl;


  return true;
}


/*
   Implements dijkstra's algorithm using a priority queue to find the path between two actors 
   with the smallest weight.
   */
void ActorGraph::dijkstraSearch(string startActor, string actorToFind, ofstream& outFile){
  
  // reset the nodes
  resetNodes(seenActorNodes);

  bool searchFinished = false;

  // create priority queue
  priority_queue<ActorNode*, vector<ActorNode*>, ActorNodePtrComp> actor_pq;

  // Get first actor
  ActorNode *currActorNode = getActorNode(startActor);

  // set distance to zero for first node
  currActorNode->distance = 0;

  currActorNode->parent = NULL;

  // push first actor into pg
  actor_pq.push(currActorNode);

  // have a set containing actor nodes that have been popped from the queue
  //unordered_set<string> seenActorNodes;

  unordered_map<string, string> actorMoviePair;
  //seenActorNodes.insert(currActorNode->actorName);

  while (!actor_pq.empty()){

    // get the front element from the pq
    currActorNode = actor_pq.top();

    // pop the top element
    actor_pq.pop();
        
    // sanity print statement to check if pq is working
    //cout << "Actor node: " << currActorNode->actorName << " with distance: " << currActorNode->distance << " popped off the pq" << endl;

	// Add to set, use to reset nodes for next search
    seenActorNodes.insert(currActorNode->actorName);

	if (!searchFinished){
		//unordered_set<string>::iterator seen_actor = seenActorNodes.find(currActorNode->actorName);   

		// node has been pooped off, set done to true
		if (!currActorNode->done){
			currActorNode->done = true;

			// Now check if current node is the node being searched for
			if (currActorNode->actorName == actorToFind){
				// print path function.
				printWeightedPath(actorMoviePair, currActorNode, outFile);
				// pop rest of queue and add to set to have them reset for next search
				searchFinished = true;
				// reset the nodes popped

			}

			// iterate thru the nodes edges and add connected actor node to queue
			auto edge = currActorNode->movieEdges.begin();

			// for each name in edge, get node and add to queue
			for (; edge != currActorNode->movieEdges.end(); ++edge){

				// get actor node using name from edge list
				ActorNode* childNode = getActorNode((*edge)->coStarName);

				// Get total distance 
				int edgeDistance = (*edge)->weight + currActorNode->distance;
				// check if a smaller weight path has been found
				if (childNode->distance > edgeDistance){

					// update member variables of the node
					childNode->parent = currActorNode;
					childNode->distance = edgeDistance;
          
					// Add node to pq
					actor_pq.push(childNode);
          
					// insert edge into edge map
					actorMoviePair[childNode->actorName] = (*edge)->movieInfo; 
        
				}
			}
		}
	}
    
  }  
}

/* 
	This function will print the weighted path to the output file
*/
void ActorGraph::printWeightedPath(unordered_map<string, string> edgePairMap, ActorNode* lastActorNode, ofstream& out_file){

  ActorNode* currNode = lastActorNode;
  cout << "Total weight of path: " << currNode->distance << endl;
  
  while(currNode){
    unordered_map<string, string>::iterator got = edgePairMap.find(currNode->actorName);
    if(got != edgePairMap.end()){
      out_file << got->first << "--" << got->second << "-->";   
    }
    else{
      out_file << currNode->actorName;
    }
    
    currNode = currNode->parent; 
  }

  out_file << endl;
}

/*
 *  This function resets all the nodes visited for the next pair search
 *  
 */
void ActorGraph::resetNodes(unordered_set<string> visitedNodes){

	// Get each nodes and reset values
	for (auto node = visitedNodes.begin(); node != visitedNodes.end(); node++){
		ActorNode* currNode = getActorNode(*node);
		currNode->distance = INT_MAX;
		currNode->parent = nullptr;
		currNode->done = false;
	}
	//cout << "Actor nodes have been reset" << endl;
	
}


vector<pair<string, string>> ActorGraph::getActorPairs(const char* actorPairs){

	// Vector to return
	std::vector<pair<string, string>> pairs;

	// Initialize the file stream
	ifstream infile(actorPairs);

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

		if (record.size() != 2){
			continue;
		}


		string actor_1(record[0]);
		string actor_2(record[1]);

		//actor_1 = ("(" + actor_1 + ")");
		//actor_2 = ("(" + actor_2 + ")");


		cout << "Pair to find: " << actor_1 << "\t" << actor_2 << endl;

		// Add to vector
		pairs.push_back(std::make_pair(actor_1, actor_2));


		if (!infile.eof()) {
			cerr << "Failed to read " << actorPairs << "!\n";
			return pairs;
		}

		infile.close();

		// return vector
		return pairs;
	}
}

/*
  Will return the oldest movie year with its corresponding actor
*/
int ActorGraph::findOldestFilmYear(const char *in_filename, vector<string> actors){

  int oldestMovieYear = INT_MAX;
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

    // see if current actor read in is one being searched for
    for(auto currActor = actors.begin(); currActor != actors.end(); currActor++){
      // if the current actor name is one of the actors to search for
      if(actor_name == *currActor){ 
        if(movie_year < oldestMovieYear){
          oldestMovieYear = movie_year;
        }
      }

    }
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return -1;
  }



  infile.close();

  return oldestMovieYear;


}

/*
	
*/
void ActorGraph::createMovieObjects(const char* in_filename, int startYear){
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

    if(movie_year >= startYear){
    // Combine movie title and year into string variable
    string movieKey = ("[" + movie_title + "#@" + to_string(movie_year) + "]");

    /*  
        Create movie class objects with given values
        Use Movie title + year as key for unordered_map
        Put the movie object as value
        Use key to make sure movie object has not been made
    exmaple: "Memento#@2000" would be key for this movie object
  */

    // First check if movie object has not been made already to avoid repeat movie objects
    unordered_map<string, Movie*>::const_iterator got = movies_map.find(movieKey);

    // If key is not found then create new movie object and add it to map
    if (got == movies_map.end()){
      Movie *newMovie = new Movie(movie_year, movieKey);
      newMovie->_actors.insert(actor_name);

      // Make a pair with movie name and movie object
      pair<string, Movie*> moviePair(movieKey, newMovie);

      // insert the pair into the movie map
      movies_map.insert(moviePair);
    }

    // Add actor to movie object returned by the iterator
    else{
      Movie *movieObject = got->second;
      movieObject->_actors.insert(actor_name);
    }

  }
  else
    cout << "Movie too old dawg..." << endl;
    /*
       At this point the unordered_map of movie objects has been filled.
       Now create the actorNodes using this data.
       */

  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return;
  }

  infile.close();
  cout << endl;
  //return true;
}


/*
    ADD SOME COMMENTS DAWG
*/
void ActorGraph::addMovieObjects(){

  // add movies to priority queue 
  for(auto movie_it = movies_map.begin(); movie_it != movies_map.end(); movie_it++){
    movieObjects.push(movie_it->second);
  }

  cout << "Added movies to queue!" << endl;

}


