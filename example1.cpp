// spherical camera + persp + lighting + texture
// Display a color cube
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

#include <GLUT/glut.h>

#include "Angel.h"
#include "simplemodels.h"

GLuint loadBMP_custom(unsigned char** data, int* w, int* h, const char * imagepath);


// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };

int      Axis = Zaxis;

GLfloat boardX,boardY,boardZ,frontTrAxX,frontTrAxY,frontTrAxZ,frontWheelsX,frontWheelsY,frontWheelsZ,rearTrAxX,rearTrAxY,rearTrAxZ,rearWheelsX,rearWheelsY,rearWheelsZ = 0.0;

GLfloat  Theta1[NumAxes] = { 80.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 30.0, 0.0 };
GLfloat  Theta3[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta4[NumAxes] = { 0.0, 0.0, 0.0 };

GLfloat dist = 0.0;

bool lightSource = false;
bool lS1 = true;
bool lS2 = true;


size_t CUBE_OFFSET;
size_t COLOR_OFFSET;
size_t CUBE_NORMALS_OFFSET;
size_t CUBE_TEXCOORDS_OFFSET;
size_t CYLINDER_OFFSET;
size_t CYLINDER_NORMALS_OFFSET;
size_t SPHERE_OFFSET;
size_t SPHERE_NORMALS_OFFSET;
size_t SPHERE_TEXCOORDS_OFFSET;
size_t TORUS_OFFSET;
size_t TORUS_NORMALS_OFFSET;
size_t TORUS_TEXCOORDS_OFFSET;
size_t TRIANGLE_OFFSET;
size_t TRIANGLE_NORMALS_OFFSET;
size_t TRIANGLE_TEXCOORDS_OFFSET;


//----------------------------------------------------------------------------

GLuint program;

GLuint vPosition;
GLuint vNormal;
GLuint vColor;
GLuint vTexCoord;

GLuint textures[10];

void init(){
    // Load shaders and use the resulting shader program
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );
    
    
    
    //---------------------------------------------------------------------
    colorcube();
    colortube();
    colorbube();
    colortorus();
    colorTri();
    
    
    //----- generate a checkerboard pattern for texture mapping
    const int  TextureSize  = 256;
    GLubyte checker_tex[TextureSize][TextureSize][3];
    
    for ( int i = 0; i < TextureSize; i++ )
    {
        for ( int j = 0; j < TextureSize; j++ )
        {
            GLubyte c = (((j & 0x4) == 1)) * 255;
            checker_tex[i][j][0]  = c;
            checker_tex[i][j][1]  = c;
            checker_tex[i][j][2]  = c;
        }
    }
    
    
    //---- Initialize texture objects
    glGenTextures( 2, textures );
    glActiveTexture( GL_TEXTURE0 );
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0, GL_BGR, GL_UNSIGNED_BYTE, checker_tex );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    
    unsigned char* pic1 = NULL;
    int w,h;
    loadBMP_custom(&pic1, &w, &h, "Grass.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic1 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic2 = NULL;
    int w2,h2;
    loadBMP_custom(&pic2, &w2, &h2, "griptape.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[2] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w2, h2, 0, GL_BGR, GL_UNSIGNED_BYTE, pic2 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    
    unsigned char* pic3 = NULL;
    int w3,h3;
    loadBMP_custom(&pic3, &w3, &h3, "Concrete.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w3, h3, 0, GL_BGR, GL_UNSIGNED_BYTE, pic3 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic4 = NULL;
    int w4,h4;
    loadBMP_custom(&pic4, &w4, &h4, "sideskirtgraffiti.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[4] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w4, h4, 0, GL_BGR, GL_UNSIGNED_BYTE, pic4 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic5 = NULL;
    int w5,h5;
    loadBMP_custom(&pic5, &w5, &h5, "Metal.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w5, h5, 0, GL_BGR, GL_UNSIGNED_BYTE, pic5 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    
    
    int w6,h6;
    
    unsigned char* negx_s = NULL;
    loadBMP_custom(&negx_s, &w6, &h6, "negx_s.bmp");
    unsigned char* negy_s = NULL;
    loadBMP_custom(&negy_s, &w6, &h6, "negy_s.bmp");
    unsigned char* negz_s = NULL;
    loadBMP_custom(&negz_s, &w6, &h6, "negz_s.bmp");
    unsigned char* posx_s = NULL;
    loadBMP_custom(&posx_s, &w6, &h6, "posx_s.bmp");
    unsigned char* posy_s = NULL;
    loadBMP_custom(&posy_s, &w6, &h6, "posy_s.bmp");
    unsigned char* posz_s = NULL;
    loadBMP_custom(&posz_s, &w6, &h6, "posz_s.bmp");
    
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_CUBE_MAP, textures[6] );
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, w6, h6, 0, GL_BGRA, GL_UNSIGNED_BYTE, posx_s);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, w6, h6, 0, GL_BGRA, GL_UNSIGNED_BYTE, posy_s);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, w6, h6, 0, GL_BGRA, GL_UNSIGNED_BYTE, posz_s);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, w6, h6, 0, GL_BGRA, GL_UNSIGNED_BYTE, negx_s);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, w6, h6, 0, GL_BGRA, GL_UNSIGNED_BYTE, negy_s);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, w6, h6, 0, GL_BGRA, GL_UNSIGNED_BYTE, negz_s);
    
    glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    
    
    
    //----set offset variables
    
    CUBE_OFFSET = 0;
    COLOR_OFFSET = sizeof(points_cube);
    CUBE_NORMALS_OFFSET = COLOR_OFFSET + sizeof(colors);
    CUBE_TEXCOORDS_OFFSET = CUBE_NORMALS_OFFSET + sizeof(normals_cube);
    CYLINDER_OFFSET = CUBE_TEXCOORDS_OFFSET + sizeof(tex_coords_cube);
    CYLINDER_NORMALS_OFFSET = CYLINDER_OFFSET + sizeof(points_cylinder);
    SPHERE_OFFSET = CYLINDER_NORMALS_OFFSET + sizeof(normals_cylinder);
    SPHERE_NORMALS_OFFSET = SPHERE_OFFSET + sizeof(points_sphere);
    SPHERE_TEXCOORDS_OFFSET = SPHERE_NORMALS_OFFSET + sizeof(normals_sphere);
    TORUS_OFFSET = SPHERE_TEXCOORDS_OFFSET + sizeof(tex_coords_sphere);
    TORUS_NORMALS_OFFSET = TORUS_OFFSET + sizeof(points_torus);
    TORUS_TEXCOORDS_OFFSET = TORUS_NORMALS_OFFSET + sizeof(normals_torus);
    TRIANGLE_OFFSET = TORUS_TEXCOORDS_OFFSET + sizeof(tex_coords_torus);
    TRIANGLE_NORMALS_OFFSET = TRIANGLE_OFFSET + sizeof(points_triangle);
    TRIANGLE_TEXCOORDS_OFFSET = TRIANGLE_NORMALS_OFFSET + sizeof(normals_triangle);

    
    
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    
    
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(normals_cube) + sizeof(tex_coords_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(normals_cylinder) + sizeof(points_sphere) + sizeof(normals_sphere) + sizeof(tex_coords_sphere) + sizeof(points_torus) + sizeof(normals_torus) + sizeof(tex_coords_torus) + sizeof(points_triangle) + sizeof(normals_triangle) + sizeof(tex_coords_triangle), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, COLOR_OFFSET, sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_NORMALS_OFFSET, sizeof(normals_cube), normals_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_TEXCOORDS_OFFSET, sizeof(tex_coords_cube), tex_coords_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_NORMALS_OFFSET, sizeof(normals_cylinder), normals_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_NORMALS_OFFSET, sizeof(normals_sphere), normals_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_TEXCOORDS_OFFSET, sizeof(tex_coords_sphere), tex_coords_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, TORUS_OFFSET, sizeof(points_torus), points_torus );
    glBufferSubData( GL_ARRAY_BUFFER, TORUS_NORMALS_OFFSET, sizeof(normals_torus), normals_torus );
    glBufferSubData( GL_ARRAY_BUFFER, TORUS_TEXCOORDS_OFFSET, sizeof(tex_coords_torus), tex_coords_torus);
    glBufferSubData( GL_ARRAY_BUFFER, TRIANGLE_OFFSET, sizeof(points_triangle), points_triangle );
    glBufferSubData( GL_ARRAY_BUFFER, TRIANGLE_NORMALS_OFFSET, sizeof(normals_triangle), normals_triangle );
    glBufferSubData( GL_ARRAY_BUFFER, TRIANGLE_TEXCOORDS_OFFSET, sizeof(tex_coords_triangle), tex_coords_triangle);
    
    
    
    
    //---------------------------------------------------------------------
    
    
    
    
    // set up vertex arrays
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(COLOR_OFFSET) );
    
    vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_TEXCOORDS_OFFSET) );
    
    // Set the value of the fragment shader texture sampler variable (myTextureSampler) to GL_TEXTURE0
    glUniform1i( glGetUniformLocation(program, "myTextureSampler"), 0 );
    glUniform1i( glGetUniformLocation(program, "myCubeSampler"), 1 );
    
    glUniform1i( glGetUniformLocation(program, "cube_map_on"), false );
    
    
    //---------------------------------------------------------------------
    
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.8632, 0.9804, 1.0, 1.0 );               //blue sky
}




//----------------------------------------------------------------------------



void
SetMaterial( vec4 ka, vec4 kd, vec4 ks, float s )
{
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
}


//----------------------------------------------------------------------------



void
SetLight( vec4 lpos, vec4 la, vec4 ld, vec4 ls )
{
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    glUniform4fv( glGetUniformLocation(program, "AmbientLight"), 1, la);
    glUniform4fv( glGetUniformLocation(program, "DiffuseLight"), 1, ld);
    glUniform4fv( glGetUniformLocation(program, "SpecularLight"), 1, ls);
}




//----------------------------------------------------------------------------



mat4 proj_matrix;
mat4 view_matrix;
mat4 wheelOneTransform, wheelTwoTransform, wheelThreeTransform, wheelFourTransform, frontAxleTransform, rearAxleTransform, frontTruckTransform, rearTruckTransform, boardCenterTransform, rearBoardEdgeTransform, frontBoardEdgeTransform;
float r = 5.0;
float railY = 0.2325;
float nearPostX = 0.0,nearPostY = .122,nearPostZ=3.475,farPostX=0.0,farPostY = .120,farPostZ = 2.0, roadY = .005;
float initialThetaX, initialThetaY, initialThetaZ, thetaX, thetaY, thetaZ = 0.0;
float yOffset = 0.16, zOffset = 0.225;



void display( void ){
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    
    //---- lights
    
    SetLight( vec4( 0.0, 3.55, 0.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), vec4(0.8, 0.8, 0.8, 1.0), vec4(0.2, 0.2, 0.2, 1.0));
    
    
    

    //---- camera intrinsic parameters
    
    float left = -1.0;
    float right = 1.0;
    float bottom = -1.0;
    float top = 1.0;
    float zNear = 1.0;
    float zFar = 15.0;
    
    
    
    
    proj_matrix = Frustum( left, right, bottom, top, zNear, zFar );
    
    glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_TRUE, proj_matrix );
    
    
    
    
    
    //---- camera extrinsic parameters
    
    float tr_y = Theta3[Yaxis]* M_PI/180.0;
    float tr_z = Theta3[Zaxis]* M_PI/180.0;
    float eye_z = r * (cos(tr_z)) * cos(tr_y);
    float eye_x = r * (cos(tr_z)) * sin(tr_y);
    float eye_y = r * sin(tr_z);
    
    vec4 up = vec4(0.0, cos(tr_z), 0.0, 0.0);
//    cout << up << ' ' << normalize(up) << endl;
    
    view_matrix = LookAt( vec4(eye_x, eye_y, eye_z, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, cos(tr_z), 0.0, 0.0));
    
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, view_matrix );
    
    
    
    
    
    
    
    
    // ----------- OBJECTS ------------

    
    
    
    
    // ----------- SKATEBOARD ------------

   //---- wh33l 0n3 rear left wheel
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial( vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.0, 1.0), 0.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    wheelOneTransform = Translate(rearWheelsX,rearWheelsY,rearWheelsZ) * RotateX(thetaX) * RotateY(thetaY) * RotateZ(thetaZ) * Translate(-0.118, 0.08, -zOffset+zOffset ) * RotateY(90) * RotateZ(initialThetaZ) * Scale(0.08, 0.08, 0.08);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, wheelOneTransform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesTorus );

    
    
    
    //---- wh33l 7w0 rear right wheel
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial( vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.0, 1.0), 0.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    wheelTwoTransform = Translate(rearWheelsX,rearWheelsY,rearWheelsZ) * RotateX(thetaX) * RotateY(thetaY) * RotateZ(thetaZ) *Translate( 0.118, 0.08, -zOffset+zOffset ) * RotateY(90) * RotateZ(initialThetaZ) * Scale(0.08, 0.08, 0.08);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, wheelTwoTransform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesTorus );
    
    
    
    
    //---- wh33l 7HR33 front wheel
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial( vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.0, 1.0), 0.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    wheelThreeTransform = Translate(frontWheelsX,frontWheelsY,frontWheelsZ) * RotateX(thetaX) * RotateY(thetaY) * RotateZ(thetaZ) *Translate( -0.118, 0.08, zOffset+zOffset ) * RotateY(90) * RotateZ(initialThetaZ) * Scale(0.08, 0.08, 0.08);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, wheelThreeTransform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesTorus );
    
    
    
    
    //---- wh33l F0|_|R front wheel
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial( vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.0, 1.0), 0.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    wheelFourTransform = Translate(frontWheelsX,frontWheelsY,frontWheelsZ)* RotateX(thetaX) * RotateY(thetaY) * RotateZ(thetaZ) *Translate( 0.118, 0.08, zOffset+zOffset ) * RotateY(90) * RotateZ(initialThetaZ) * Scale(0.08, 0.08, 0.08);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, wheelFourTransform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TORUS_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesTorus );
    
    
    
    
    //----- Axle 1, front axle
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.0, 0.4, 0.4, 1.0), vec4(0.925, 0.925, 0.925, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    frontAxleTransform =  Translate(frontTrAxX,frontTrAxY,frontTrAxZ)* RotateX(thetaX) * RotateY(thetaY) * RotateZ(thetaZ) *Translate( 0, 0.08, zOffset+zOffset ) * Scale(0.3, 0.04, 0.04) * RotateZ(90 + initialThetaZ);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, frontAxleTransform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );

    
    
    
    //----- Axle 2, rear axle
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.0, 0.4, 0.4, 1.0), vec4(0.925, 0.925, 0.925, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    rearAxleTransform = Translate(rearTrAxX,rearTrAxY,rearTrAxZ)* RotateX(thetaX) * RotateY(thetaY) * RotateZ(thetaZ) *Translate( 0, 0.08, -zOffset+zOffset ) * Scale(0.3, 0.04, 0.04) * RotateZ(90 + initialThetaZ);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, rearAxleTransform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );

    
    
    
    
    //----- Truck1, front truck
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.0, 0.4, 0.4, 1.0), vec4(0.925, 0.925, 0.925, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    frontTruckTransform = Translate(frontTrAxX,frontTrAxY,frontTrAxZ) * RotateX(thetaX) * RotateY(thetaY) * RotateZ(thetaZ) * Translate( 0, 0.11, zOffset+zOffset ) * * RotateZ(initialThetaZ) * Scale(0.03, 0.08, 0.04);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, frontTruckTransform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );

    
    
    
    //----- Truck2, rear truck
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.0, 0.4, 0.4, 1.0), vec4(0.925, 0.925, 0.925, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    rearTruckTransform = Translate(rearTrAxX,rearTrAxY,rearTrAxZ)* RotateX(thetaX) * RotateY(thetaY) * RotateZ(thetaZ) *Translate( 0, 0.11, -zOffset+zOffset ) * RotateZ(initialThetaZ) *Scale(0.03, 0.08, 0.04);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, rearTruckTransform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );

    
    
    
    
    
    //---- board
    
    glBindTexture( GL_TEXTURE_2D, textures[2] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.94, 0.95, 0.95, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    boardCenterTransform = Translate(boardX,boardY, boardZ) * RotateX(thetaX) * RotateY(thetaY) * RotateZ(thetaZ) * Translate( 0, yOffset, zOffset ) * RotateZ(initialThetaZ) * Scale(0.25, 0.025, 0.75);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, boardCenterTransform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    
    //---- Concrete road
    
    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true);

    SetMaterial(vec4(0.2, 0.2, 0.2, 1.0), vec4(0.7343, 0.5546, 0.4336, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    mat4 transform = Translate( 0.0, roadY, 0.0 ) * Scale(2.5, 0.05, 25.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );

    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    
    //---- Grass
    
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.2, 0.2, 0.2, 1.0), vec4(0.23, 0.726, 0.421, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( 0.0, 0.0, 0.0 ) * Scale(8.0, 0.05, 25.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    //----- Grinding Rail
    
    // Rail
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.0, 0.0, 1.0, 1.0), vec4(0.925, 0.925, 0.925, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    mat4 rail = Translate( 0, railY, 2.75 ) * RotateY(90) * Scale(1.5, 0.04, 0.04) * RotateZ(90);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, rail );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    //------ Post 1
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.0, 0.0, 1.0, 1.0), vec4(0.925, 0.925, 0.925, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    mat4 post = Translate( nearPostX, nearPostY, nearPostZ ) * Scale(0.04, .265, 0.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, post);
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    //------ Post 2
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.0, 0.0, 1.0, 1.0), vec4(0.925, 0.925, 0.925, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    mat4 postTwo = Translate( farPostX, farPostY, farPostZ ) * Scale(0.04, .265, 0.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, postTwo);
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    
    //---- BenchBase
    
    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.94, 0.95, 0.95, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( -2.5, 0.2, 2.0 ) * Scale(0.5, 0.07, 1.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    
    
    //---- BenchLeftLeg
    
    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.94, 0.95, 0.95, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( -2.5, 0.2, 2.5 ) * Scale(0.5, 0.4, 0.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    

    
    //---- BenchRightLeg
    
    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.94, 0.95, 0.95, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( -2.5, 0.2, 1.5 ) * Scale(0.5, 0.4, 0.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );

    
    
    
    
    //---- BenchBack
    
    glBindTexture( GL_TEXTURE_2D, textures[4] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.94, 0.95, 0.95, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( -2.75, 0.3, 2.0 ) * Scale(0.05, 0.5, 1.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    
    
    //---- Bench Body
    
    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( 2.5, 0.45, 2.0 ) * Scale(0.2, 0.35, 0.2);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    
    
    
    //  --------    GUY     -------
    
    // ---- Leg1
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( 2.5, 0.15, 2.05 ) * Scale(0.05, 0.25, 0.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    // ---- Leg2
    
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( 2.5, 0.15, 1.95 ) * Scale(0.05, 0.25, 0.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    
    // ---- Arm1
    
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( 2.5, 0.45, 2.15 ) * RotateX(-45) * Scale(0.05, 0.25, 0.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    
    
    // ---- Arm2
    
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial(vec4(0.4, 0.4, 0.4, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform = Translate( 2.5, 0.45, 1.85 ) * RotateX(45) * Scale(0.05, 0.25, 0.05);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    
    
    //---- Head
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial( vec4(0.4, 0.4, 0.4, 1.0), vec4(0.925, 0.925, 0.925, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    mat4 transform_bube3 = Translate( 2.5, 0.75, 2.0 ) * Scale(0.25, 0.25, 0.25);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube3 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );



    
    //---- Sky Cube
    
    glBindTexture( GL_TEXTURE_2D, textures[6] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.4, 0.4, 0.4, 1.0), vec4(0.95, 0.95, 0.95, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), true );
    
    mat4 sky = Translate( 0.0, 0.0, 0.0 ) * Scale(15.0, 15.0, 15.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, sky );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    
    glutSwapBuffers();
}




//----------------------------------------------------------------------------

enum Trick {ollie=0,kickflip=1};
Trick entryTrick = kickflip;
float popAngle = 30;
float counter = 0;
bool postReached = false, grind = false, enterGrind = false, grindStart = false, exitGrind = false, endReached = false;
bool skateForward = false, skateBackwards = false, skateRight = false, skateLeft = false;
bool doKickflip = false;
bool doOllie = false, doPop = false, ollieReached = false, ollieDone = false;
bool doManual = false, manualReached = false, manualDone = false;
void keyboard( unsigned char key, int x, int y ){
    switch( key ) {
        case 033:    // Escape key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
        break;
        
        case 'y':    //---- theta
            Axis = Yaxis;
            Theta3[Axis] += 4.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
            
        case 'Y':    //---- theta
            Axis = Yaxis;
            Theta3[Axis] -= 4.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;

        case 'z':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] += 4.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
            
        case 'Z':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] -= 4.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;

        case 'r':    //---- increase radius
            r += 0.5;
            glutPostRedisplay();
            break;
       
        case 'R':    //---- decrease radius
            r -= 0.5;
            glutPostRedisplay();
            break;
            
        case 'u':
            //view_matrix = default_view_matrix;
            Theta3[Xaxis] = 0.0;
            Theta3[Yaxis] = 0.0;
            Theta3[Zaxis] = 0.0;
            glutPostRedisplay();
            break;
        
        case 'k':  // kickflip
            doOllie = true;
            doPop = true;
            ollieReached = false;
            ollieDone = false;
            doKickflip = true;
            glutPostRedisplay();
            break;
            
        case 'm':   // manual
            doManual = true;
            manualReached = false;
            manualDone = false;
            glutPostRedisplay();
            break;
            
        case 'o':  // ollie
            doOllie = true;
            doPop = true;
            ollieReached = false;
            ollieDone = false;
            glutPostRedisplay();
            break;
        
         case 'P': // pedal forward
            skateForward = true;
            skateBackwards = false;
            glutPostRedisplay();
            break;
         case 'p':
            skateForward = true;
            enterGrind = true;
            skateBackwards = true;
            glutPostRedisplay();
            break;
        case 's': // stop
            skateForward = false;
            skateBackwards = false;
            glutPostRedisplay();
            break;
        
    }
}




//----------------------------------------------------------------------------

void mouse( int button, int state, int x, int y ){
    if ( state == GLUT_DOWN ) {
        switch( button ) {
            case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
            case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
            case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
        }
    }
}

//----------------------------------------------------------------------------

void idle( void ){
    Theta4[Xaxis] = fmod(Theta4[Xaxis]+0.5, 360.0);
    
    float frontBoundary = 4.0;
    float rearBoundary = -3.0;
    float step = 0.1/10;
    float oneRadian = M_PI/180;
    
    if (farPostZ - (boardZ+zOffset) <= 0.6  && enterGrind)
    {
        grind = true;
        enterGrind = false;
        postReached = true;
        switch(entryTrick)
        {
            case kickflip:
            {
                doKickflip = true;
            }
            case ollie:
            {
                doOllie = true;
                doPop = true;
                ollieReached = false;
                ollieDone = false;
                break;
            }
        }
        grindStart = false;
        endReached = false;
        exitGrind = false;
    }
    if (postReached && !doPop)
    {
        if (frontTrAxY-railY <= 0)
        {
            switch(entryTrick)
            {
                case kickflip:
                {
                    doKickflip = false;
                }
                case ollie:
                {
                    doOllie = false;
                    doPop = false;
                    ollieReached = true;
                    ollieDone = true;
                    break;
                }
            }
            grindStart = true;
            doManual = true;
            manualReached = false;
            manualDone = false;
            postReached = false;
        }
    }
    if (grindStart && !endReached)
    {
        if (nearPostZ - frontTrAxZ <= 0.35)
        {
            grindStart = false;
            endReached = true;
            doManual=false;
            manualDone = true;
            manualReached = true;
        }
    }
    if (endReached)
    {
        endReached = false;
        exitGrind = true;
        postReached = false;
        grindStart = false;
        doOllie = true;
        doPop = true;
        ollieReached = false;
        ollieDone = false;
    }
    if (skateForward)
    {
        boardZ += step;
        frontTrAxZ += step;
        frontWheelsZ += step;
        rearTrAxZ += step;
        rearWheelsZ += step;
        boardZ = fmod(boardZ,frontBoundary);
        rearWheelsZ = fmod(rearWheelsZ,frontBoundary);
        frontWheelsZ = fmod(frontWheelsZ,frontBoundary);
        rearTrAxZ = fmod(rearTrAxZ,frontBoundary);
        frontTrAxZ = fmod(frontTrAxZ,frontBoundary);
        if(frontBoundary-frontTrAxZ <= 0.1)
        {
          skateForward = false;
        }
    }
    if (doManual)
    {
      if(!manualDone)
      {
        if(manualReached)
        {
          if(grind)
          {
            float boardNose = boardZ+zOffset;
            if(nearPostZ-boardNose <= 0.3)
            {
                grind = false;
            }
          }
          else
          {
            thetaX += 1;
            if(thetaX == 0)
            {
              thetaX = 0;
              manualDone = true;
              doManual = false;
            }
          }
        }
        else
        {
          thetaX -= 1;
          thetaX = fmod(thetaX, 25);
          if(thetaX == 0)  // angle reached
          {// theta reached
            thetaX = -25;
            manualReached = true;
          }
        }
      }
    }
    if (doOllie)
    {
      if (!ollieDone)
      {
          if(doPop)
          {
            if(!ollieReached)
            {
              thetaX -= 1;
              thetaX = fmod(thetaX, popAngle);
              if(thetaX == 0)
              {
                dist = 0;
                ollieReached = true;
                thetaX = -popAngle;
              }
            }
            else
            {
                float oneRadian = M_PI/180;
                boardY += oneRadian;
                frontTrAxY += oneRadian;
                frontWheelsY += oneRadian;
                rearTrAxY += oneRadian;
                rearWheelsY += oneRadian;
                thetaX += 1;
                if(thetaX == 0)
                  doPop = false;
            }
          }
          else
          {
              cout << exitGrind << endl;
                counter++;
                float oneRadian = M_PI/180;
                boardY -= oneRadian;
                frontTrAxY -= oneRadian;
                frontWheelsY -= oneRadian;
                rearTrAxY -= oneRadian;
                rearWheelsY -= oneRadian;
                if(frontWheelsY <= roadY && !exitGrind)
                {
                  ollieDone = true;
                  doOllie = false;
                  counter = 0;
                }
                else if(exitGrind)
                {
                  if (frontWheelsY-railY <= -railY+.01)
                  {
                      ollieDone = true;
                      doOllie = false;
                  }
                }
          }

        }
    }
    if (doKickflip)
    {
      if (!doPop)
      {
          thetaZ += 36;
          thetaZ = fmod(thetaZ,360);
          if(thetaZ == 0)
              doKickflip = false;
      }
    }
    glutPostRedisplay();
}


//----------------------------------------------------------------------------


void reshape( int w, int h ){
    glViewport(0,0,w,h);
}


//----------------------------------------------------------------------------


int main( int argc, char **argv ){
    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 1000, 1000 );
    glutCreateWindow( "SKATE!" );
    
    init();
    
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    //glutKeyboardUpFunc(<#void (*func)(unsigned char, int, int)#>)
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
    
    glutMainLoop();
    return 0;
}















