/*
* Movie.h
* Author: Alvin Vanegas
* Date:   3/3/2017
*
* This class will hold the name of the film and a set of actor names
  that acted in the film.
*/

#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <unordered_set>
  using namespace std;

/*	
	Movie class will have the name of the movie and a set
	of actors that have been in said movie
*/
	class Movie{

	public:
		
		// year of film, will be used for weighted edges
		int movieYear;
	// name of film + year
		string movieName;
	// will hold actors that have acted in the film
		unordered_set<string> _actors;
	// constructor
		Movie(int year, string key);
	// need for priority queue of movie objects
		bool operator<(const Movie& other);

	};

#endif