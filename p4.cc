// Author: James Kluz
// Computer Vision Assignment 2
// Professor Ioannis Stamos
// Start date: 9/13/2016
// Takes a labled .pgm file and database of object charecteristics:
// object label, row position of the center, column position of the center, the minimum moment of inertia, 
// orientation, roundness, min factor, and max factor
// Two objects are considered the same if their roundness, min factor and max factors are within certain thresholds.
// If a simlar object is found in the labled .pgm file then the centroid and axis of minimum moment is drawn
// in the output image file. 


#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h> //trig functions, floor and sqrt()
#include <cmath>
#include <vector>
#include <algorithm>    // std::min
#include <sstream>

using namespace ComputerVisionProjects;
using namespace std;

//structure representing an object from the object database
struct object{
  double roundness;
  double min_factor;
  double max_factor;
};

void FindObjects(const std::string &input_database, Image *a_labled_image);

int
main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s {input labeled image} {input database} {output image}\n", argv[0]);
    return 0;
  }
  const string input_image_file(argv[1]);
  const string input_text_file(argv[2]);
  const string output_image_file(argv[3]);
  Image an_image;
  if (!ReadImage(input_image_file, &an_image)) {
    cout <<"Can't open file " << input_image_file << endl;
    return 0;
  }

  // search for objects in image that are similar to those in database
  // if object found, draw centroid and axis of lsm
  FindObjects(input_text_file, &an_image);
  
  if (!WriteImage(output_image_file, an_image)){
    cout << "Can't write to file " << output_image_file << endl;
  }
   
  return 0;
} 

void FindObjects(const std::string &input_database, Image *a_labled_image){
  std::ifstream input(input_database);
  std::vector<object> object_database;
  std::string data_line;
  std::getline(input, data_line); 
  //Get objects from database that we are looking for
  while(std::getline(input, data_line)){
    if(!data_line.empty()){
      std::stringstream input_stream(data_line);
      object temp;
      double garbage;
      //skip first 5 elements in row
      input_stream >> garbage >> garbage >> garbage >> garbage >> garbage;
      input_stream >> temp.roundness >> temp.min_factor >> temp.max_factor;
      object_database.push_back(temp);
    }
  }
  input.close();
  //now we check the image
  int num_of_obj = a_labled_image->num_gray_levels();
  //I'm adding 1 because I want the pixel color to match the position in the vector
  //using this to tally area and get info for moments
  // label is position in outer vector then six columns: 
  //area / A * x_center / A * y_center / i^2 / j^2 / i*j
  std::vector<std::vector<double>> objects(num_of_obj+1, vector<double>(6, 0)); 
  size_t rows = a_labled_image->num_rows();
  size_t cols = a_labled_image->num_columns();
  for(int j = 0; j < rows; ++j){
    for(int k = 0; k < cols; ++k){
      int pix = a_labled_image->GetPixel(j, k);
      if(pix!=0){
        objects[pix][0]+= 1; //update area
        objects[pix][1]+= j; //update x
        objects[pix][2]+= k; //update y
        objects[pix][3]+= (j*j);
        objects[pix][4]+= (k*k);
        objects[pix][5]+= (j*k);
      }
    }
  }
  //Set centers correctly and get minimum moment and orientation, output to .txt file
  for(int k = 1; k < num_of_obj+1; ++k){
    double area_obj = objects[k][0];
    double integral_x = objects[k][1];
    double integral_y = objects[k][2];
    double integral_of_x_squared = objects[k][3];
    double integral_of_y_squared = objects[k][4];
    double integral_of_xy = objects[k][5];

    double x_hat = integral_x / area_obj;  
    double y_hat = integral_y / area_obj;  

    //get second moments for object: 
    double a = integral_of_x_squared - (area_obj * x_hat * x_hat);  
    double b = 2*(integral_of_xy - x_hat*y_hat*area_obj);  
    double c = integral_of_y_squared - area_obj*y_hat * y_hat;

    //theta is angle bewtween horizontal axis and axis of least moment
    double theta = atan2(b, a-c);
    theta /= 2;

    //Get min and max moments
    double Emin = a*sin(theta)*sin(theta) - b*sin(theta)*cos(theta) + c*cos(theta)*cos(theta);
    double theta_max = theta + 3.14159265/2;
    double Emax = a*sin(theta_max)*sin(theta_max) - b*sin(theta_max)*cos(theta_max) + c*cos(theta_max)*cos(theta_max);
    
    //Thresholds for testing
    double roundness_threshold = .025;
    double min_factor_threshold = 1.1;
    double max_factor_threshold = 1.1;
    
    // go through the database of objects
    // if this object passes these three tests 
    // for an object in the database then we have a match and we draw a line:
    for(int i = 0; i < object_database.size(); i++){
      bool round_test = abs((Emin/Emax) - object_database[i].roundness) < roundness_threshold;
      bool min_test = false;
      bool max_test = false;
      if(round_test){
        //second "max" is being used to find the min
        min_test = (max(Emin/area_obj, object_database[i].min_factor) / (0 - max(0 - Emin/area_obj, 0 - object_database[i].min_factor))) < min_factor_threshold;
      }
      if(min_test){
        max_test = (max(Emax/area_obj, object_database[i].max_factor) / (0 - max(0 - Emax/area_obj, 0 - object_database[i].max_factor))) < max_factor_threshold;
      }

      //this object is in our database so we will draw a line now
      if(max_test){
        //need to convert to closest integer value since we are in a discrete space
        int x_hat_int = floor(x_hat + 0.5);
        int y_hat_int = floor(y_hat + 0.5);

        //getting length of line to represent axis of least moment
        //making sure our line for axis of least moment does not go out of bounds:
        double line_length = ((cols - x_hat_int-1) < x_hat_int) ? (cols - x_hat_int-1) : x_hat_int;
        line_length        = ((rows - y_hat_int-1) < line_length) ? (rows - y_hat_int-1) : line_length;  
        line_length        = (y_hat_int < line_length) ? y_hat_int : line_length;
        //find the radius of a circle with 1.5 * area of object
        double circle_of_area_A = 1.5*(sqrt(area_obj / 3.14159265)); 
        //set line length to minimum "out of bounds" length or 1.5*area of circle with same area as object
        line_length = (circle_of_area_A < line_length) ? circle_of_area_A : line_length;

        //calculate an end point for axis of least moment of length line_length
        int x_1 = floor(x_hat_int + cos(theta)*line_length);
        int y_1 = floor(y_hat_int + sin(theta)*line_length); 

        //add two more grey levels for centroid and line drawing
        a_labled_image->SetNumberGrayLevels(a_labled_image->num_gray_levels()+2);
        //draw a line representing axis of least moment
        DrawLine(x_hat_int, y_hat_int, x_1, y_1, a_labled_image->num_gray_levels()-1, a_labled_image);
        //put a dot at centroid
        a_labled_image->SetPixel(x_hat_int, y_hat_int, a_labled_image->num_gray_levels());
        //we found an object so we can get out
        break;
      }
    }
  }
}