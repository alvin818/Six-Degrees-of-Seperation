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
#include <unordered_set>
#include "ActorGraph.h"
#include "ActorNode.h"
#include "ActorEdge.h"

using namespace std;

ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    int counter = 0;
  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
      
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        /*  Two possible inputs:
            1. new actor name with movie
            2. actor with node already created
            
            case 1: create new node and create new edge and add
            edge to adjacency list of new node
            case 2: add new edge using movie info

        */

        if(counter == 0){ 
            // Create graph node using actors name
            ActorNode *newNode = new ActorNode(actor_name);
            // create new edge
            ActorEdge *newEdge = new ActorEdge( movie_title, movie_year);
            // add edge to actornode 
            newNode->edges.insert(newEdge);
            // push new node into graph vector
            graph.push_back(newNode);
            // increment count since new node was made
            counter++;
        }

        // Now check actor node exists for given input
        if(actor_name == graph.at(counter - 1)->actorName){
            
            /* 
               if actor node exists then simply create new edge 
               and add to actor node 
            */

            // create new edge
            ActorEdge *newEdge = new ActorEdge( movie_title, movie_year);
            // add edge to actornode 
           graph.at(counter - 1)->edges.insert(newEdge);
            // push new node into graph vector
        }
        // New actor node created
        else{
            // Create new actor node
            ActorNode *newNode = new ActorNode(actor_name);
            // create new edge
            ActorEdge *newEdge = new ActorEdge( movie_title, movie_year);
            // add edge to actornode 
            newNode->edges.insert(newEdge);
            // push new node into graph vector
            // Add new node to graph
            graph.push_back(newNode);
            // increment count since new node was made
            counter++;
        }
		cout << "Created node for actor: " << graph.at(counter)->actorName << endl;
        // node have an actor/movie relationship, now what?
        
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}

/* 
   Will create graph using the vector of  actornodes and create the edges
   for the graph and update the adjaceny lists of the actor nodes
*/
vector<ActorNode*> ActorGraph::createGraph(){

    /* 
       Nested loop: outer loop starts at first node in vector, inner loop starts at second node
       Check the movie names in edges of each node, if a match update both edges with each others 
       actor name
    */

    vector<ActorNode*>::iterator it_1 = graph.begin();
    // Outer loop begins ar first node
    for(; it_1 != graph.end(); it_1++){
        // Inner loop starting at second node
        vector<ActorNode*>::iterator it_2 = graph.begin() + 1;
        for(; it_2 != graph.end(); it_2++){
            // now check edge vector inside each node; use helper function!!!
            createEdges(*it_1, *it_2);
        }

    }

	
}


void ActorGraph::createEdges( ActorNode* actor_1, ActorNode* actor_2){
    
    /* 
       Loop thru each nodes edge vector 
       Check each each from actor_1 to actor_2
       If there is a match update both edges with the others actors name
    */ 
    unordered_set<ActorEdge*> actor1Edges = actor_1->edges;
    unordered_set<ActorEdge*> actor2Edges = actor_2->edges;

    // Loop through actor1's edges
    for (const auto& elem: actor1Edges) {

        // iterator will hold edge that has the same movie
        unordered_set<ActorEdge*>::iterator got = actor2Edges.find(elem);
        // If the two nodes share a similar movie then update the edges
        if(got != actor2Edges.end()){
            // update actor1
            elem->coStarName = actor_2->actorName;
            // update actor 2
            (*got)->coStarName = actor_1->actorName;
        }
    }    

}
