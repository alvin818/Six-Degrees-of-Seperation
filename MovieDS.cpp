/*
* File: Movie.cpp
* Author: Alvin Vanegas
* Date:   2/25/17
* Winter 2017
* Overview: Constructor for the movie class/objects
*/

#include "MovieDS.h"

// Constructor for movie objects
MovieDS::MovieDS(string movie_name, int movie_year){
	movieName = movie_name;
	movieYear = movie_year;
}

// Defines operator for priority queue
bool MovieDS::operator<(MovieDS const & other){

	// if movie yers are different, just compare movie years
	if (movieYear != other.movieYear)
		return this->movieYear > other.movieYear;

	// Use movie name value to break ties
	return this->movieName < other.movieName;
}