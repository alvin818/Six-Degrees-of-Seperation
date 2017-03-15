/*
* File: ActorNodeDS.cpp
* Author: Alvin Vanegas
* Date:   2/25/17
* Winter 2017
* Overview: constructor for the actor node using for disjoint sets
*/

#include "ActorNodeDS.h"

ActorNodeDS::ActorNodeDS(string actor_name){
	actorName = actor_name;
	parent = nullptr;
	size = 0;
}