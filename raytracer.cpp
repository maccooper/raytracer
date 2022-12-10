#include <iostream>
#include <string>
#include "./lib/mat.h"      //GLM Library code
#include "./lib/vec.h"      //GLM Library code
#include "./lib/invert.h"   //Course instructor code

using namespace std;

struct img_params {
    float near,
          left,
          right,
          top,
          bottom;
    int rows,
        cols;
    //vec sphere_list;
    //vec light_list;
    //vec back;
    //vec ambient;
    string output;    
};


void parse_file(char *file_name) {

}

void save_imageP6(int Width, int Height, char* fname,unsigned char* pixels) {
    //Code supplied from course instructor in Assignment 3
  FILE *fp;
  const int maxVal=255;
  
  printf("Saving image %s: %d x %d\n", fname,Width,Height);
  fp = fopen(fname,"wb");
  if (!fp) {
        printf("Unable to open file '%s'\n",fname);
        return;
  }
  fprintf(fp, "P6\n");
  fprintf(fp, "%d %d\n", Width, Height);
  fprintf(fp, "%d\n", maxVal);

  for(int j = 0; j < Height; j++) {
		  fwrite(&pixels[j*Width*3], 3,Width,fp);
  }

  fclose(fp);
}

int main(int argc, char *argv[]) {
    std::cout <<"Hello World\n";
    return 0;

}
