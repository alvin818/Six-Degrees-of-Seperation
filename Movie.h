/*
* Movie.h
* Author: <YOUR NAME HERE>
* Date:   <DATE HERE>
*
* This class will hold the name of the film and a set of actor names
  that acted in the film.
*/

#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <unordered_set>
using namespace std;


class Movie{

public:
	// name of film 
	string movieName;
	// will hold actors that have acted in the film
	unordered_set<string> _actors;
	// constructor
	Movie(string name);

};

#endif