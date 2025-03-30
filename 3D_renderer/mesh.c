#include <stdio.h>
#include <string.h>
#include "array.h"
#include "mesh.h"


#define MAX_NUM_MESHES 500
static mesh_t meshes[MAX_NUM_MESHES];
static int mesh_count = 0;


vec3_t cube_vertices[N_CUBE_VERTICES] =
{
    {-1, -1, -1},
    {-1,  1, -1},
    { 1,  1, -1},
    { 1, -1, -1},
    { 1,  1,  1},
    { 1,  -1,  1},
    {-1,  1,  1},
    {-1, -1,  1}
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {1,  2, 3,{ 0, 1 },{ 0, 0 }, { 1, 0 }, 0xffffffff }, //0
    {1,  3, 4,{ 0, 1 },{ 1, 0 }, { 1, 1 }, 0xffffffff }, // 1
    // right
    {4,  3, 5,{ 0, 1 },{ 0, 0 }, { 1, 0 },  0xffFFFFFF }, //2
    {4,  5, 6,{ 0, 1 },{ 1, 0 }, { 1, 1 },  0xFFFFFFFF }, //3
    // back
   { 6,  5, 7,{ 0, 1 },{ 0, 0 }, { 1, 0 },  0xffffffff }, //4
    { 6,  7, 8,{ 0, 1 },{ 1, 0 }, { 1, 1 },  0xffffffff }, //5
    // left
    { 8,  7, 2,{ 0, 1 },{ 0, 0 }, { 1, 0 },  0xffffffff }, //6
    { 8,  2, 1,{ 0, 1 },{ 1, 0 }, { 1, 1 },  0xffffffff }, //7
    // top
    { 2,  7, 5,{ 0, 1 },{ 0, 0 }, { 1, 0 },  0xffffffff }, //8 
    { 2,  5, 3,{ 0, 1 },{ 1, 0 }, { 1, 1 },  0xffffffff }, //9
    // bottom
    { 6,  8, 1,{ 0, 1 },{ 0, 0 }, { 1, 0 },  0xffffffff }, //10
    { 6,  1, 4,{ 0, 1 },{ 1, 0 }, { 1, 1 },  0xffffffff } // 11
    // front


};

void load_mesh_obj_data(mesh_t* mesh, char* obj_filename) {
    
 
    FILE* file;
    file = fopen(obj_filename, "r");
    char line[1024];
    tex2_t* texcoords = NULL;
    

    while (fgets(line, 1024, file)) {
        // Vertex information
        if (strncmp(line, "v ", 2) == 0) {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            int i = 0;
            array_push(meshes[mesh_count].vertices, vertex);
        }
       // Texture coordinate information
       if (strncmp(line, "vt ", 3) == 0) {
           tex2_t texcoord;
           sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
           array_push(texcoords, texcoord);
       }
       // Face information
        
        //if (strncmp(line, "f ", 2) == 0) {
        //    int vertex_indices[3];
        //   
        //    sscanf(
        //        line, "f %d %d %d",
        //        &vertex_indices[0],
        //        &vertex_indices[1],
        //        &vertex_indices[2]
        //    );
        //
        //    face_t face = {
        //        .a = vertex_indices[0],
        //        .b = vertex_indices[1],
        //        .c = vertex_indices[2],
        //        .color = 0xFFFFFFFF
        //    };
        //    array_push(meshes[mesh_count].faces, face);
        //}
    
        if (strncmp(line, "f ", 2) == 0) {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]
            );
            
            if (texture_indices[0] < 0)
            {
                if (strncmp(line, "f ", 2) == 0) {
                    int vertex_indices[3];

                    sscanf(
                        line, "f %d %d %d",
                        &vertex_indices[0],
                        &vertex_indices[1],
                        &vertex_indices[2]
                    );

                    face_t face = {
                        .a = vertex_indices[0],
                        .b = vertex_indices[1],
                        .c = vertex_indices[2],
                        .color = 0xFFFFFFFF
                    };
                    array_push(meshes[mesh_count].faces, face);
                }
            }
            else
            {

                face_t face = {
                    .a = vertex_indices[0],
                    .b = vertex_indices[1],
                    .c = vertex_indices[2],
                    .a_uv = texcoords[texture_indices[0]],
                    .b_uv = texcoords[texture_indices[1]],
                    .c_uv = texcoords[texture_indices[2]],
                    .color = 0xFFFFFFFF
                };
                array_push(meshes[mesh_count].faces, face);
            }{}
        }
    }
    array_free(texcoords);
    fclose(file);
}

void load_mesh_png_data(mesh_t* mesh, char* png_filename) {
    upng_t* png_image = upng_new_from_file(png_filename);
    if (png_image != NULL) {
        upng_decode(png_image);
        if (upng_get_error(png_image) == UPNG_EOK) {
            mesh->texture = png_image;
        }
    }
}

void load_textured_mesh(char* obj_filename, char* png_filename, vec3_t scale, vec3_t translation, vec3_t rotation) {
    load_mesh_obj_data(&meshes[mesh_count], obj_filename);
    load_mesh_png_data(&meshes[mesh_count], png_filename);

    meshes[mesh_count].scale = scale;
    meshes[mesh_count].translation = translation;
    meshes[mesh_count].rotation = rotation;
    meshes[mesh_count].faces->color = meshes[mesh_count].faces->color;

    mesh_count++;
}

void load_colored_mesh(char* obj_filename, uint32_t color , vec3_t scale, vec3_t translation, vec3_t rotation)
{
    load_mesh_obj_data(&meshes[mesh_count], obj_filename);
    

    meshes[mesh_count].scale = scale;
    meshes[mesh_count].translation = translation;
    meshes[mesh_count].rotation = rotation;
    meshes[mesh_count].faces->color = color;

    mesh_count++;
}

void load_cube_mesh_data(mesh_t* mesh, int side, uint32_t color, char* png_filename, vec3_t scale, vec3_t translation, vec3_t rotation)
{
   


 for (int i = 0; i < N_CUBE_VERTICES; i++)
 {
      vec3_t cube_vertex = cube_vertices[i];
      array_push(meshes[mesh_count].vertices, cube_vertex);
     
    
 
 }

   

   //for (int i = 0; i < N_CUBE_FACES; i++)
  {
      switch (side)
      {
      case 1: //front
      {
          //if (i == 0 || i == 1)
          {
              face_t cube_face0 = cube_faces[0];
              cube_face0.color = color;
              array_push(meshes[mesh_count].faces, cube_face0);
              face_t cube_face1 = cube_faces[1];
              cube_face1.color = color;
              array_push(meshes[mesh_count].faces, cube_face1);
          }
      }break;
  
      case 2:  //right
      {
          //if (i == 2 || i == 3)
          {
              face_t cube_face2 = cube_faces[2];
              cube_face2.color = color;
              array_push(meshes[mesh_count].faces, cube_face2);
              face_t cube_face3 = cube_faces[3];
              cube_face3.color = color;
              array_push(meshes[mesh_count].faces, cube_face3);
          }
      }break;
  
      case 3:  //left
      {
         // if (i == 6 || i == 7)
          {
              face_t cube_face6 = cube_faces[6];
              cube_face6.color = color;
              array_push(meshes[mesh_count].faces, cube_face6);
              face_t cube_face7 = cube_faces[7];
              cube_face7.color = color;
              array_push(meshes[mesh_count].faces, cube_face7);
          }
      }break;
  
      case 4:  //back
      {
          //if (i == 4 || i == 5)
          {
              face_t cube_face4 = cube_faces[4];
              cube_face4.color = color;
              array_push(meshes[mesh_count].faces, cube_face4);
              face_t cube_face5 = cube_faces[5];
              cube_face5.color = color;
              array_push(meshes[mesh_count].faces, cube_face5);;
          }
      }break;
  
      case 5:  //top
      {
         // if (i == 8 || i == 9)
          {
              face_t cube_face8 = cube_faces[8];
              cube_face8.color = color;
              array_push(meshes[mesh_count].faces, cube_face8);
              face_t cube_face9 = cube_faces[9];
              cube_face9.color = color;
              array_push(meshes[mesh_count].faces, cube_face9);
          }
      }break;
  
      case 6:  //bottom
      {
          //if (i == 10 || i == 11)
          {
              face_t cube_face10 = cube_faces[10];
              cube_face10.color = color;
              array_push(meshes[mesh_count].faces, cube_face10);
              face_t cube_face11 = cube_faces[11];
              cube_face11.color = color;
              array_push(meshes[mesh_count].faces, cube_face11);
          }
      }break;
  
      default:
      {
          for (int i = 0; i < N_CUBE_FACES; i++)
          {
              face_t cube_face = cube_faces[i];
              array_push(meshes[mesh_count].faces, cube_face);
          }
      }break;
      }
     
  }

   load_mesh_png_data(&meshes[mesh_count], png_filename);

    meshes[mesh_count].rotation = rotation;
    meshes[mesh_count].scale = scale;
    meshes[mesh_count].translation = translation;
    
    mesh_count++;
   
}

mesh_t* get_mesh(int mesh_index) {
    return &meshes[mesh_index];
}

int get_num_meshes(void) {
    return mesh_count;
}

void rotate_mesh_x(int mesh_index, float angle) {
    meshes[mesh_index].rotation.x += angle;
}

void rotate_mesh_y(int mesh_index, float angle) {
    meshes[mesh_index].rotation.y += angle;
}

void rotate_mesh_z(int mesh_index, float angle) {
    meshes[mesh_index].rotation.z += angle;
}

void free_meshes(void) {
    for (int i = 0; i < mesh_count; i++) {
        array_free(meshes[i].faces);
        array_free(meshes[i].vertices);
        upng_free(meshes[i].texture);
    }
}
