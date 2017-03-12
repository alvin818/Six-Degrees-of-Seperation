/*
	
 * File: Movie.cpp
 * Author: Alvin Vanegas
 * Date:   2/25/17
 * Winter 2017
 * Overview: Constructor for the movie class/objects


*/

#include "Movie.h"
#include <string>
 using namespace std;

/*Constructor*/

 Movie::Movie(int year, string name){
 	
 	movieYear = year;
 	
 	movieName = name;
 }

 bool Movie::operator<(Movie const & other){
  // if movie yers are different, just compare movie years
  if(movieYear != other.movieYear)
    return this->movieYear > other.movieYear;

  // Use movie name value to break ties
  return this->movieName < other.movieName; 
}