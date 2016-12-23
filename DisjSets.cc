/****************************************
 File:      DisjSets.cpp
 Author:    James Kluz
 Purpose:   
      Function definitions for 
      template class DisjSets.h
 Date:      4/14/2016
 **************************************/

template <typename Object>
void DisjSets<Object>::Add_elem(const Object & elem){
  if(sets_.find(elem) == sets_.end()){
    sets_[elem] = elem;
    set_heights_[elem] = 0;
    ++number_of_sets_;
  }   
}
template <typename Object>
const Object & DisjSets<Object>::Find_root(const Object & elem){
  if(sets_[elem] == elem)
    return elem;
  else
    return sets_[elem] = Find_root(sets_[elem]);
}
template <typename Object>
void DisjSets<Object>::Connect_elems(const Object & elem1, const Object & elem2){
  Add_elem(elem1);
  Add_elem(elem2);
  const Object & root1 = Find_root(elem1);
  const Object & root2 = Find_root(elem2);
  if(root1 == root2){
    return; 
  }
  Union_sets(root1, root2);
  number_of_sets_ = number_of_sets_ - 1;
}
template <typename Object>
int DisjSets<Object>::Get_number_of_sets() const{
  return number_of_sets_;
}
template <typename Object>
void DisjSets<Object>::Union_sets(const Object & root1, const Object & root2){
  if(set_heights_[root1] < set_heights_[root2]){
    sets_[root1] = root2;
  } else if(set_heights_[root2] < set_heights_[root1]){
    sets_[root2] = root1;
  } else{
    sets_[root2] = root1;
    set_heights_[root1] += 1; 
  }
}