#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "./lib/mat.h"      //GLM Library code
#include "./lib/vec.h"      //GLM Library code
#include "./lib/invert.h"   //Course instructor code
#include <sstream>

using namespace std;

struct Sphere {
    string name;
    vec3 pos; //xyz
    vec3 scale; //scale<xyz>
    vec3 colour; //RGB
    float ka,
          kd,
          ks,
          kr;
    int n;
};

struct img_params {
    float near,
          left,
          right,
          top,
          bottom;
    int resolution_rows,
        resolution_cols;
    vector<Sphere> s_list;
    //vec<light/> light_list;
    //vec back;
    //vec ambient;
    string output;    
} scene;

void parse_file(string file_name) {
    ifstream f(file_name);
    if(!f) {
        cout << "File does not exist\n";
        return;
    }
    string line;
    while(getline(f, line)) {  
        stringstream s;
        string token;
        s << line;
        s >> token;
        if(token == "NEAR") {
            s >> scene.near;
        } else if(token == "LEFT") {
            s >> scene.left;
        } else if(token == "RIGHT") {
            s >> scene.right;
        } else if(token == "TOP") {
            s >> scene.top;
        } else if(token == "BOTTOM") {
            s >> scene.bottom;
        } else if(token == "RES") {
            s >> scene.resolution_rows;
            s >> scene.resolution_cols;
        } else if(token == "SPHERE") {
            cout << "sphere line" << "\n";
        } else if(token == "LIGHT") {
            cout << "Light Line" << "\n";
        } else if(token == "BACK") {
            cout << "Back Line" << "\n";
        } else if(token == "AMBIENT") {
        } else if(token == "OUTPUT") {
            s >> scene.output;
            if ((scene.output).length() < 20) {
            } else {
                cout << "Error on input file, output name specified is too large" << "\n";
            }
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
