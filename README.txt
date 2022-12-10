# Raytracer

### Built in Fall 2022 for CSC305 at UVic

codebase written in C++, takes an input from a text file containing parameters to generate a raytraced image like the one below. Outputs a .png file using p6  code supplied by the course instructor.


## Example output based on testSpecular.txt

![specular output](https://user-images.githubusercontent.com/26943573/206874720-c19ae8fc-bc02-4337-be21-768e20336b18.png)

## Input File Format

NEAR \<n>   <br>
LEFT \<l>   <br>
RIGHT \<r>  <br>
BOTTOM \<b> <br>
TOP \<t>    <br>
RES \<x> \<y> <br>
SPHERE \<name> \<pos x> \<pos y> \<pos z> \<scl x> \<scl y> \<scl z> \<r> \<g> \<b> \<Ka> \<Kd> \<Ks> \<Kr> \<n> <br>
//… up to 14 additional sphere specifications <br>
LIGHT \<name> \<pos x> \<pos y> \<pos z> \<Ir> \<Ig> \<Ib> <br>
… // up to 9 additional light specifications <br>
BACK \<r> \<g > \<b> <br>
AMBIENT \<Ir> \<Ig> \<Ib> <br>
OUTPUT \<name>

#### To run this code, follow the procedure below:

1. Compile with g++ raytracer.cpp -o raytracer.exe
1. Run with ./raytracer.exe \<file_name>



