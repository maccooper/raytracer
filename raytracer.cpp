#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "./lib/mat.h"      //GLM Library code
#include "./lib/vec.h"      //GLM Library code
#include "./lib/invert.h"   //Course instructor code
#include <sstream>

using namespace std;

struct Sphere {
    string name;
    vec4 pos; //xyz
    vec3 scale; //scale<xyz>
    vec4 colour; //RGBA
    float k_ambient, //K_a
          k_diffuse, //K_d
          k_specular, //K_s
          k_fresnel; //K_r
    int specular_exp; //n
};

struct Light {
    string name;
    vec3 pos;
    vec3 intensity;
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
    vector<Light> l_list;
    vec3 back;
    vec3 ambient;
    string output;    
} scene;

struct Ray {
    vec3 origin;
    vec3 direction;
    int depth;
};

struct Intersection {
    Ray ray;
    Sphere sphere;
    vec3 norm;
    vec3 point;
    float dist;
    int is_inside;
};

void parse_file(string file_name) {
    ifstream f(file_name);
    if(!f) {
        cout << "File does not exist\n";
        return;
    }
    string line;
    while(getline(f, line)) {  
        stringstream ss;
        string token;
        ss << line;
        ss >> token;
        if(token == "NEAR") {
            ss >> scene.near;
        } else if(token == "LEFT") {
            ss >> scene.left;
        } else if(token == "RIGHT") {
            ss >> scene.right;
        } else if(token == "TOP") {
            ss >> scene.top;
        } else if(token == "BOTTOM") {
            ss >> scene.bottom;
        } else if(token == "RES") {
            ss >> scene.resolution_rows;
            ss >> scene.resolution_cols;
        } else if(token == "SPHERE") {
            Sphere sphere;
            //Name
            ss >> sphere.name;
            //Position
            ss >> sphere.pos.x;
            ss >> sphere.pos.y;
            ss >> sphere.pos.z;
            sphere.pos.w = 1.0f;
            //Scale
            ss >> sphere.scale.x;
            ss >> sphere.scale.y;
            ss >> sphere.scale.z;
            //Colour
            ss >> sphere.colour.x;
            ss >> sphere.colour.y;
            ss >> sphere.colour.z;
            sphere.colour.w = 1.0f;
            //Reflection Coefficients
            ss >> sphere.k_ambient, //K_a
            ss >> sphere.k_diffuse, //K_d
            ss >> sphere.k_specular, //K_s
            ss >> sphere.k_fresnel; //K_r
            //Specular Exponent
            ss >> sphere.specular_exp;
            //Add sphere to list
            scene.s_list.push_back(sphere);
        } else if(token == "LIGHT") {
            Light light;
            //Name
            ss >> light.name;
            //Position
            ss >> light.pos.x;
            ss >> light.pos.y;
            ss >> light.pos.z;
            //Intensity
            ss >> light.intensity.x;
            ss >> light.intensity.y;
            ss >> light.intensity.z;
            //Add Light to list
            scene.l_list.push_back(light);
        } else if(token == "BACK") {
            //cout << "Back Line" << "\n";
            ss >> scene.back.x;
            ss >> scene.back.y;
            ss >> scene.back.z;
        } else if(token == "AMBIENT") {
            ss >> scene.ambient.x;
            ss >> scene.ambient.y;
            ss >> scene.ambient.z;
        } else if(token == "OUTPUT") {
            ss >> scene.output;
            if ((scene.output).length() < 20) {
            } else {
                cout << "Error on input file, output name specified is too large" << "\n";
            }
        }
    }
    f.close();
}

Intersection compute_closest_intersections(Ray &ray) {
    Intersection intersect;
    intersect.ray = ray;
    return intersect;
}


vec3 ray_trace(Ray &r) {
    if(r.depth > 2) {
        //return black
        return vec3(0,0,0);
    }
    // p = closest intersection of ray with all objects
    
    return vec3(1,1,1);
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
    for (int i = 0; i < scene.l_list.size(); i++) {
            cout << scene.ambient.x<< "\n";
    }
    return 0;
}
