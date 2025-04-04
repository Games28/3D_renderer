#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "upng.h"
//#include "aabb3.h"

#define N_CUBE_VERTICES 8

extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2)
extern face_t cube_faces[N_CUBE_FACES];



typedef struct {
    vec3_t* vertices;         // mesh dynamic array of vertices
    face_t* faces;            // mesh dynamic array of faces
    upng_t* texture;          // mesh PNG texture
    vec3_t scale;             // mesh scale in x, y, and z
    vec3_t rotation;          // mesh rotation in x, y, and z
    vec3_t translation;       // mesh translation in x, y, and z
} mesh_t;

void load_mesh_obj_data(mesh_t* mesh, char* obj_filename);
void load_mesh_png_data(mesh_t* mesh, char* png_filename);

void load_textured_mesh(char* obj_filename, char* png_filename, vec3_t scale, vec3_t translation, vec3_t rotation);

void load_colored_mesh(char* obj_filename, uint32_t color, vec3_t scale, vec3_t translation, vec3_t rotation);

void load_cube_mesh_data(mesh_t* mesh,int side, uint32_t color, char* png_filename, vec3_t scale, vec3_t translation, vec3_t rotation);

mesh_t* get_mesh(int mesh_index);
int get_num_meshes(void);

void rotate_mesh_x(int mesh_index, float angle);
void rotate_mesh_y(int mesh_index, float angle);
void rotate_mesh_z(int mesh_index, float angle);

void free_meshes(void);

//aabb3 test
//AABB3 GetAABB() const;


#endif
