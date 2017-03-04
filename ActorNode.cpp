/* 
   File: ActorNode.cpp
   Name: Alvin Vanegas
   Date: 2/25/17
   Overview: constructor for trhe actorn ndoe
*/

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
   }

