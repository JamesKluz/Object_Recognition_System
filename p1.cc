// Author: James Kluz
// Computer Vision Assignment 2
// Professor Ioannis Stamos
// Start date: 9/13/2016
// Converts a .pgm file into a binary .pgm file

#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace ComputerVisionProjects;
using namespace std;

void GrayToBinary(Image *an_image, size_t gray_threshold);

int
main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s {input-file} {gray level threshold} {output-file}\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const size_t gray_threshold = std::stoi(argv[2]);
  const string output_file(argv[3]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  // turn file into binary image
  GrayToBinary(&an_image, gray_threshold);
  
  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
  }
   
 return 0;
} 

void GrayToBinary(Image *an_image, size_t gray_threshold){
  size_t rows = an_image->num_rows();
  size_t cols = an_image->num_columns();
  for(int j = 0; j < rows; ++j){
    for(int k = 0; k < cols; ++k){
      if(an_image->GetPixel(j, k) < gray_threshold){
        an_image->SetPixel(j, k, 0);
      } else {
        an_image->SetPixel(j, k, 1);
      }
    }
  }
  an_image->SetNumberGrayLevels(1);
}