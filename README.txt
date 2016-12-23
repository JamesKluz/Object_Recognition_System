A pipeline of programs (p1, p2, p3, p4) that do the following:

p1: converts a greyscale image to a binary image based on a threshold given by the user
p2: converts a binary image to a "labeled" image by running a raster scan and assigning
    the same color to every connected (non-zero) pixel in the image
p3: Takes a labeled image from p2, calculates the first and second moments and
    outputs an image with the axis of least second moment and the centroid drawn
    onto every object in the image. Further more it outputs a .txt file with data
    on each object to be used in p4.
p4: Takes a labeled image from p2 and a .txt from p3 and compares the objects in the image
    to the database in the .txt file. If it finds a match, the corresponding object in the 
    image file has its controid and axis of least moment drawn on.        

----------------------
To compile in Linux:
----------
 
   make all

To run:
---------

./p1 {input_greyscale_image.pgm} {threshold} {output_binary_image.pgm}
./p2 {input_binary_image.pgm} {output_labeled_image.pgm}
./p3 {input_labeled_image.pgm} {output_database.txt} {output_processed_image.pgm}
./p4 {input_labeled_image.pgm} {input_database.txt} {output_processed_image.pgm}

-----------





