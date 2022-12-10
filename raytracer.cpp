#include <iostream>
#include <string>

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
    std::string output;    
};


void save_imageP6(int Width, int Height, char* fname,unsigned char* pixels) {
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

int main() {
    std::cout <<"Hello World\n";
    return 0;

}
