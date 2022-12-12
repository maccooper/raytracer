#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "./lib/mat.h"      //GLM Library code
#include "./lib/vec.h"      //GLM Library code
//#include "./lib/invert.h"   //Course instructor code
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
    mat4 inverse;
};

struct Light {
    string name;
    vec4 pos;
    vec4 intensity;
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
    vec4 back;
    vec4 ambient;
    string output;    
} scene;

struct Ray {
    vec4 origin;
    vec4 direction;
    int depth;
};

struct Intersection {
    Ray ray;
    Sphere sphere;
    vec4 norm;
    vec4 point;
    float distance;
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
            //Compute Inverse using invert.cpp
            InvertMatrix(Scale(sphere.scale), sphere.inverse); 
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
            string temp;
            ss >> temp;
            //scene.output;
            if (temp.length() < 20) {
            } else {
                cout << "Error on input file, output name specified is too large" << "\n";
                return;
            }
            scene.output = temp;
        }
    }
    f.close();
}

Intersection compute_closest_intersection(Ray &ray) {
    Intersection intersect;
    intersect.ray = ray;
    intersect.distance = -1;
    intersect.is_inside = 0;

    for (int i = 0; i < scene.s_list.size(); i++) {
        float a = dot(scene.s_list[i].inverse * ray.direction, scene.s_list[i].inverse * ray.direction);
        float b = dot(scene.s_list[i].inverse * (scene.s_list[i].pos - ray.origin), scene.s_list[i].inverse * ray.direction);
        float c = dot(scene.s_list[i].inverse * (scene.s_list[i].pos - ray.origin), scene.s_list[i].inverse * (scene.s_list[i].pos - ray.origin)) - 1;
        float q;
        float discriminant = b*b-a*c;
        float soln1 = (b-sqrtf(discriminant)) / a;
        float soln2 = (b+sqrtf(discriminant)) /a;

        int inside_check = 0;

        if(discriminant > 0) {
            if(soln1 > soln2) {
                q = soln2;
            } else {
                q = soln1;
            }
            if(q <= 0.0001f || ray.depth == 0 && q <= 1.0f) {
                if(soln1 > soln2) {
                    q = soln1;
                } else {
                    q = soln2;
                }
                inside_check = 1;
            }
        } else if (discriminant < 0) {
            continue;
        } 
        if (q <= 0.0001f || (ray.depth == 0 && q <= 1.0f)) {
            continue;
        }
        if((intersect.distance == -1 || q < intersect.distance)) {
            intersect.distance = q;
            intersect.sphere = scene.s_list[i];
            intersect.is_inside = inside_check;
        }
    }
    if (intersect.distance != -1) {
        intersect.point = ray.origin + ray.direction * intersect.distance;
        if (intersect.is_inside) {
            vec4 norm = -(intersect.point - intersect.sphere.pos);
            mat4 inverse_transpose = transpose(intersect.sphere.inverse);
            norm = inverse_transpose * intersect.sphere.inverse * norm;
            norm.w = 0;
            intersect.norm = normalize(norm);
        } else {
            vec4 norm = intersect.point - intersect.sphere.pos;
            mat4 inverse_transpose = transpose(intersect.sphere.inverse);
            norm = inverse_transpose * intersect.sphere.inverse * norm;
            norm.w = 0;
            intersect.norm = normalize(norm);
        }
    }
    return intersect;
}

vec4 ray_trace(Ray &r) {
    Intersection intersect = compute_closest_intersection(r);
    if(r.depth > 2) {
        //return black
        return vec4(0,0,0,0);
    } else if (intersect.distance == -1 && r.depth == 0) {
        return scene.back;
    } else if (intersect.distance == - 1) {
        return vec4(0,0,0,0);
    } else {
        vec4 diffuse = (0,0,0,0);
        vec4 specular = (0,0,0,0);
        for(int i = 0; i < scene.l_list.size(); i++) {
            Ray rn;
            rn.origin = intersect.point;
            rn.direction = normalize(scene.l_list[i].pos - intersect.point);

            Intersection light_ray_intersect = compute_closest_intersection(rn);
            vec4 h = normalize(rn.direction - r.direction);
            float s_strength = dot(intersect.norm, h);
            float d_strength = dot(intersect.norm, rn.direction);
            if (light_ray_intersect.distance == -1) {
                if(d_strength > 0) {
                    diffuse += d_strength * scene.l_list[i].intensity * intersect.sphere.colour;
                    specular += powf(powf(s_strength, intersect.sphere.specular_exp), 5) * scene.l_list[i].intensity;
                }
            }
        }
        Ray r_reflection;
        r_reflection.origin = intersect.point;
        r_reflection.direction = normalize(r.direction - 2.0f * dot(intersect.norm, r.direction) * intersect.norm);
        r_reflection.depth = r.depth+1;
        vec4 colour = intersect.sphere.colour * intersect.sphere.k_ambient + diffuse * intersect.sphere.k_diffuse + specular * intersect.sphere.k_specular + ray_trace(r_reflection) * intersect.sphere.k_fresnel;
        colour.w = 1.0f;

        return colour;
    }
}

void save_imageP6(int Width, int Height, const char* fname,unsigned char* pixels) {
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

void render() {
    unsigned char *pixels;
    pixels = new unsigned char[scene.resolution_cols * scene.resolution_rows * 3];
    vector<vec4> pixel_colours;
    pixel_colours.resize((unsigned int) (scene.resolution_cols * scene.resolution_rows)); //Check this one out
    for (int i = 0; i < scene.resolution_cols; i++) {
        for (int j = 0; j < scene.resolution_rows; j++) {
            //for each pixel
            float x = scene.left + ((float) j / scene.resolution_rows) * (scene.right - scene.left);
            float y = scene.bottom + ((float) i / scene.resolution_cols) * (scene.top - scene.bottom);
            Ray r;
            r.origin = vec4(0.0f,0.0f,0.0f,1.0f);
            r.direction = vec4(x,y, -scene.near, 0.0f);
            r.depth = 0;
            vec4 p_colour = ray_trace(r);
            pixel_colours[(scene.resolution_cols - i - 1) * scene.resolution_rows + j] = p_colour;
    }
    for (int i = 0; i < scene.resolution_cols; i++) {
        for (int j = 0; j < scene.resolution_rows; j++) {
            for (int k = 0; k < 3; k++) {
                if(((float*) pixel_colours[i * scene.resolution_rows + j])[k] < 0) {
                    ((float*) pixel_colours[i * scene.resolution_rows + j])[k] *= -1;
                }
                if(((float*) pixel_colours[i * scene.resolution_rows + j])[k] > 1) {
                    ((float*) pixel_colours[i * scene.resolution_rows + j])[k] = 1;
                }
                pixels[i * scene.resolution_rows * 3 + j * 3 + k] = (unsigned char) (((float*) pixel_colours[i * scene.resolution_rows + j])[k] * 255.9f);
            }
        }
    }
        //string temp = scene.output;
    }
    save_imageP6(scene.resolution_rows, scene.resolution_cols, scene.output.c_str(),pixels);                                                               
    return;

}

int main(int argc, char *argv[]) {
    parse_file(argv[1]);
    render();
}
