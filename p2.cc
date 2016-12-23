// Author: James Kluz
// Computer Vision Assignment 2
// Professor Ioannis Stamos
// Start date: 9/13/2016
// Converts a binary .pgm file to a labled-object .pgm file

#include "image.h"
#include "DisjSets.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace ComputerVisionProjects;
using namespace std;

void BinaryToLabeled(Image *a_binary_image);

int
main(int argc, char **argv){
  
  if (argc!=3) {
    printf("Usage: %s {input-Binary .pgm file} {output-Labled_file}\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string output_file(argv[2]);
  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  // turn file into labeled image
  BinaryToLabeled(&an_image);
  
  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
  }
   
  return 0;
} 

void BinaryToLabeled(Image *a_binary_image){
  size_t rows = a_binary_image->num_rows();
  size_t cols = a_binary_image->num_columns();
  DisjSets<int> connected_components;
  int cntr = 0;
  //we apply the raster scan algorithm d|c 
  //                                   b|a
  for(int j = 0; j < rows; ++j){
    for(int k = 0; k < cols; ++k){
      int a = a_binary_image->GetPixel(j, k);
      //std::cout << a << " ";
      if( a!= 0){
        //Get neighbors but make sure neighbors aren't out of bounds
        int d = (j != 0 && k != 0) ? a_binary_image->GetPixel(j-1, k-1) : 0;
        int b = (k != 0) ? a_binary_image->GetPixel(j, k-1) : 0;  
        int c = (j != 0) ? a_binary_image->GetPixel(j-1, k) : 0; 

        if(d != 0){
          a = d;
        } else if ((b!=0 && c == 0) || (b!=0 && c == b)){
          a = b;  
        } else if (c!=0 && b == 0){
          a = c;
        } else if (b == 0 && c == 0){ //we potentially have a new object
          a = ++cntr;  
          connected_components.Add_elem(a);
        } else {//c and b are non-zero AND not equal, we have given 1 object two numbers.
          a = c;
          //make numbers b and c equivalent
          connected_components.Connect_elems(b, c);
        }
        a_binary_image->SetPixel(j, k, a);
      }  
    }
  }
  //maps the root of a disjoint set to the "real" representative for an object
  std::unordered_map<int, int> object_int;
  cntr = 0;
  int num_objects = connected_components.Get_number_of_sets();
  for(int j = 0; j < rows; ++j){
    for(int k = 0; k < cols; ++k){
      int a = a_binary_image->GetPixel(j, k);
      if( a!= 0){
        int root = connected_components.Find_root(a);
        if(object_int.find(root) == object_int.end()){
          ++cntr;
          a = cntr;
          object_int[root] = cntr;
        } else {
          a = object_int[root];
        }  
        a_binary_image->SetPixel(j, k, a);
      }
      
    }
  }
  if(cntr != num_objects){
    std::cout << "Problem in BinaryToLabeled function, aborting.\n";
    exit(-1);
  }  
  a_binary_image->SetNumberGrayLevels(num_objects);
}


