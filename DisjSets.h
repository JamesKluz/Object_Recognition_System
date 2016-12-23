/****************************************
 File:      DisjSets.h
 Author:    James Kluz
 Purpose:   
			Template Class for dynamic
			connectivity of sets.
			Implemented with Union-Find algorithm
			using path compression.
 Requires:	DisjSets.cpp			
 Date:      4/14/2016
 **************************************/

 #ifndef 	CSCI_335_DISJSETS_H
 #define	CSCI_335_DISJSETS_H

 #include <unordered_map>

template <typename Object>
class DisjSets{
public:
	DisjSets() : number_of_sets_{0} {}
	//adds a new singleton set comprised
	//of just elem
	void Add_elem(const Object & elem);
	//returns the root node of elem's set
	//This function also changes the parent
	//of each element along the path to elems
	//root to the root
	const Object & Find_root(const Object & elem);
	//makes the root of the set with larger height
	//the parent of the root of the set with smaller height
	//if elem1 and elem2 have the same root then 
	//nothing changes
	void Connect_elems(const Object & elem1, const Object & elem2);
	//returns an int corresponding to the number of
	//sets in the data structure
	int Get_number_of_sets() const;
private:
	//internal function for Connect_elems(Object elem1, Object elem2)
	void Union_sets(const Object & root1, const Object & root2);
	std::unordered_map<Object, Object> sets_;
	std::unordered_map<Object, int> set_heights_;
	int number_of_sets_;	
 }; 

#include "DisjSets.cc"
#endif