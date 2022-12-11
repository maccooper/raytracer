#include <iostream>
#include <fstream>
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
    int resolution_rows,
        resolution_cols;
    //vec sphere_list;
    //vec light_list;
    //vec back;
    //vec ambient;
    string output;    
};

img_params scene;

void parse_file(string file_name) {
    string line;
    size_t pos;
    string token;
    ifstream f(file_name);
    if(!f) {
        cout << "File does not exist\n";
        return;
    }
    while(getline(f,line)) {
        pos = line.find(" ");
        token = line.substr(0,pos);
        if(token == "NEAR") {
            scene.near = stof(line.substr(pos));
        } else if(token == "LEFT") {
            scene.left = stof(line.substr(pos));
        } else if(token == "RIGHT") {
            scene.right = stof(line.substr(pos));
        } else if(token == "TOP") {
            scene.top = stof(line.substr(pos));
        } else if(token == "BOTTOM") {
            scene.bottom = stof(line.substr(pos));
        } else if(token == "RES") {
            //Fuck
        }
    }
    f.close();
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
    parse_file(argv[1]);
    return 0;
}
