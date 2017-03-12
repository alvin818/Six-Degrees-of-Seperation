/* 
   File: ActorNode.cpp
   Name: Alvin Vanegas
   Date: 2/25/17
   Overview: constructor for trhe actorn ndoe
*/
#include <limits.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include "ActorNode.h"
   using namespace std;


/*Constructor*/

   ActorNode::ActorNode(string name){
   	actorName = name;
   	parent = NULL;
   	distance = INT_MAX;
	done = false;
   }


  bool ActorNode::operator<(ActorNode const & other){
  // if counts are different, just compare counts
  if(distance != other.distance)
    return this->distance > other.distance;

  // Use symbol value to break ties
  return this->actorName < other.actorName; 
}

