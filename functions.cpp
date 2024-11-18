#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string> 
#include "functions.h"

using std::cout, std::endl, std::string;

void initializeImage(Pixel image[][MAX_HEIGHT]) {
  // iterate through columns
  for (unsigned int col = 0; col < MAX_WIDTH; col++) {
    // iterate through rows
    for (unsigned int row = 0; row < MAX_HEIGHT; row++) {
      // initialize pixel
      image[col][row] = {0, 0, 0};
    }
  }
}

void loadImage(string filename, Pixel image[][MAX_HEIGHT], unsigned int& width, unsigned int& height) {
  // TODO: implement (part 1)
  std::ifstream preImage;
  preImage.open(filename);
  if(!preImage.is_open()){
    throw std::runtime_error("Failed to open " + filename);
  }

  std::string fileType;
  preImage >> fileType;
  if(fileType != "P3" && fileType != "p3"){
    throw std::runtime_error("Invalid type " + fileType);
  }

  preImage >> width >> height;
  if (width <= 0 || width > MAX_WIDTH || height <= 0 || height > MAX_HEIGHT){
    throw std::runtime_error("Invalid dimensions");
  }

  int maxColor;
  preImage >> maxColor;
  if(maxColor > 255 || maxColor < 0) {
    throw std::runtime_error("Invalid color value");
  }

    for(unsigned int h = 0; h < height; h++){
      for(unsigned int w = 0; w < width; w++){
      short r, g, b;
      if(!(preImage >> r >> g >> b)){
        throw std::runtime_error("Invalid color value");
      }
      if(r < 0 || g < 0 || b < 0 || r > maxColor || b > maxColor || g > maxColor){
        throw std::runtime_error("Invalid color value");
      }
      image[w][h] = {r,g,b};
      }
    }
  short anyExtra;
  if(preImage >> anyExtra){
  throw std::runtime_error("Too many values");
  }
  preImage.close();
}

void outputImage(string filename, Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height) {
  // TODO: implement (part 1)
  std::ofstream postImage;
  postImage.open(filename);
  if(!postImage.is_open()){
    throw std::runtime_error("Failed to open " + filename);
  }
  postImage << "P3" << endl;
  postImage << width << " " << height << endl;
  postImage << "255" << endl;
  for(unsigned int h = 0; h < height; h++){
    for(unsigned int w = 0; w < width; w++){
    postImage << image[w][h].r << " " << image[w][h].g << " " << image[w][h].b << " ";
    }
  }
  postImage.close();
}

unsigned int energy(Pixel image[][MAX_HEIGHT], unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
  // TODO: implement (part 1)
  int xGradient;
  int yGradient;
  if(x==0){
    xGradient = pow(image[width-1][y].r-image[x+1][y].r,2)+pow(image[width-1][y].g-image[x+1][y].g,2)+pow(image[width-1][y].b-image[x+1][y].b,2);
  }
  else if(x==width-1){
    xGradient = pow(image[x-1][y].r-image[0][y].r,2)+pow(image[x-1][y].g-image[0][y].g,2)+pow(image[x-1][y].b-image[0][y].b,2);
  }
  else{
    xGradient = pow(image[x+1][y].r-image[x-1][y].r,2)+pow(image[x+1][y].g-image[x-1][y].g,2)+pow(image[x+1][y].b-image[x-1][y].b,2);
  }
  if(y==0){
    yGradient = pow(image[x][height-1].r-image[x][y+1].r,2)+pow(image[x][height-1].g-image[x][y+1].g,2)+pow(image[x][height-1].b-image[x][y+1].b,2);
  }
  else if(y==height-1){
    yGradient = pow(image[x][y-1].r-image[x][0].r,2)+pow(image[x][y-1].g-image[x][0].g,2)+pow(image[x][y-1].b-image[x][0].b,2);
  }
  else{
    yGradient = pow(image[x][y+1].r-image[x][y-1].r,2)+pow(image[x][y+1].g-image[x][y-1].g,2)+pow(image[x][y+1].b-image[x][y-1].b,2);
  }
  return xGradient+yGradient;
}

// uncomment functions as you implement them (part 2)

unsigned int loadVerticalSeam(Pixel image[][MAX_HEIGHT], unsigned int start_col, unsigned int width, unsigned int height, unsigned int seam[]) {
//TODO: implement (part 2)
unsigned int energySum = 0;
for(unsigned int rows = 0; rows < height; rows++){
  energySum += energy(image, start_col, rows, width, height);
  seam[rows] = start_col;
  if(rows!=height-1){
    if(start_col == 0){
    unsigned int nextCenter = energy(image, start_col, rows+1, width, height);
    unsigned int nextRight = energy(image, start_col+1, rows+1, width, height);
      if(nextRight < nextCenter){
        start_col+=1;
      }
    }
    else if(start_col == width - 1){
    unsigned int nextLeft = energy(image, start_col-1, rows+1, width, height);
    unsigned int nextCenter = energy(image, start_col, rows+1, width, height);
      if(nextLeft < nextCenter){
        start_col-=1;
      }
    }
    else{
    unsigned int nextLeft = energy(image, start_col-1, rows+1, width, height);
    unsigned int nextCenter = energy(image, start_col, rows+1, width, height);
    unsigned int nextRight = energy(image, start_col+1, rows+1, width, height);
      if(nextRight < nextCenter && nextRight <= nextLeft){
        start_col += 1;
      }
      else if(nextLeft < nextCenter && nextLeft < nextRight){
        start_col -= 1;
      }
    }
  }
}
  return energySum;
}

 unsigned int loadHorizontalSeam(Pixel image[][MAX_HEIGHT], unsigned int start_row, unsigned int width, unsigned int height, unsigned int seam[]) {
  // TODO: implement (part 2)
  unsigned int energySum = 0;
  for(unsigned int cols = 0; cols < width; cols++){
    energySum += energy(image, cols, start_row, width, height);
    seam[cols] = start_row;
    if(cols!=width-1){
      if(start_row == 0){
      unsigned int nextCenter = energy(image, cols+1, start_row, width, height);
      unsigned int nextDown = energy(image, cols+1, start_row+1, width, height);
        if(nextDown < nextCenter){
          start_row+=1;
        }
      }
      else if(start_row == height - 1){
      unsigned int nextUp = energy(image, cols+1, start_row-1, width, height);
      unsigned int nextCenter = energy(image, cols+1, start_row, width, height);
        if(nextUp < nextCenter){
          start_row-=1;
        }
      }
      else{
      unsigned int nextUp = energy(image, cols+1, start_row-1, width, height);
      unsigned int nextCenter = energy(image, cols+1, start_row, width, height);
      unsigned int nextDown = energy(image, cols+1, start_row+1, width, height);
        if(nextDown < nextCenter && nextDown < nextUp){
          start_row += 1;
        }
        else if(nextUp < nextCenter && nextUp <= nextDown){
          start_row -= 1;
        }
      }
    }
  }
  return energySum;
}

void findMinVerticalSeam(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height, unsigned int seam[]) {
//   // TODO: implement (part 2)
  unsigned int lowest = loadVerticalSeam(image,0,width,height,seam);
  unsigned int lowCol = 0;
  for(unsigned int col = 1; col < width; col++){
    if(loadVerticalSeam(image,col,width,height,seam)<lowest){
      lowest = loadVerticalSeam(image,col,width,height,seam);
      lowCol = col;
    }
  }
  loadVerticalSeam(image,lowCol,width,height,seam); //sets seam to the lowest vertical seam by running it again
}

void findMinHorizontalSeam(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height, unsigned int seam[]) {
  // TODO: implement (part 2)
  unsigned int lowest = loadHorizontalSeam(image,0,width,height,seam);
  unsigned int lowRow = 0;
  for(unsigned int row = 1; row < height; row++){
    if(loadHorizontalSeam(image,row,width,height,seam)<lowest){
      lowest = loadHorizontalSeam(image,row,width,height,seam);
      lowRow = row;
    }
  }
  loadHorizontalSeam(image,lowRow,width,height,seam); //sets seam to the lowest horizontal seam by running it again
 }

void removeVerticalSeam(Pixel image[][MAX_HEIGHT], unsigned int& width, unsigned int height, unsigned int verticalSeam[]) {
//   // TODO: implement (part 2)
  for(unsigned int rows = 0; rows<height; rows++){
    for(unsigned int tempCol = verticalSeam[rows];tempCol<width-1;tempCol++){
      image[tempCol][rows] = image[tempCol+1][rows];
    }
  }
  width-=1;
}

void removeHorizontalSeam(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int& height, unsigned int horizontalSeam[]) {
   // TODO: implement (part 2)
  for(unsigned int cols = 0; cols<width; cols++){
    for(unsigned int tempRow = horizontalSeam[cols];tempRow<height-1;tempRow++){
      image[cols][tempRow] = image[cols][tempRow+1];
    }
 }
  height-=1;
}
