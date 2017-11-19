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
    glGenTextures( 6, textures );
    
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
    cout << up << ' ' << normalize(up) << endl;
    
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
    
    mat4 transform = Translate( 0.0, 0.005, 0.0 ) * Scale(2.5, 0.05, 25.0);
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
    
    
    
    
//    //---- Ramp
//   //---- Center Piece
//    
//    glBindTexture( GL_TEXTURE_2D, textures[4] );
//    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
//    
//    SetMaterial(vec4(0.2, 0.2, 0.2, 1.0), vec4(0.23, 0.726, 0.421, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
//    glUniform1i( glGetUniformLocation(program, "light_out"), false );
//    
//    
//    transform = Translate( 0.0, 0.25, 2.0 ) * Scale(1.0, 0.5, 1.0);
//    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
//    
//    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
//    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
//    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
//    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
//    
//    //---- Side Skirts
//    
//    //---- #1
//    glBindTexture( GL_TEXTURE_2D, textures[4] );
//    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
//    
//    SetMaterial(vec4(0.2, 0.2, 0.2, 1.0), vec4(0.23, 0.726, 0.421, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
//    glUniform1i( glGetUniformLocation(program, "light_out"), false );
//    
//    
//    transform = Translate( 0.0, 0.3, 2.85 ) * RotateX(25) * Scale(1.0, 0.05, 0.8);
//    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
//    
//    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
//    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
//    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
//    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
//    
//    //---- #2
//    glBindTexture( GL_TEXTURE_2D, textures[4] );
//    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
//    
//    SetMaterial(vec4(0.2, 0.2, 0.2, 1.0), vec4(0.23, 0.726, 0.421, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
//    glUniform1i( glGetUniformLocation(program, "light_out"), false );
//    
//    
//    transform = Translate( 0.0, 0.3, 1.15 ) * RotateY(180) * RotateX(25) * Scale(1.0, 0.05, 0.8);
//    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
//    
//    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
//    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
//    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
//    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
//    
//    glBindTexture( GL_TEXTURE_2D, textures[1] );
//    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
//    
//    SetMaterial(vec4(0.2, 0.2, 0.2, 1.0), vec4(0.23, 0.726, 0.421, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
//    glUniform1i( glGetUniformLocation(program, "light_out"), false );
//    
//    
//    transform = Translate( 0.0, 0.25, 3.0 ) * Scale(3.0, 0.5, 2.0);
//    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
//    
//    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TRIANGLE_OFFSET) );
//    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TRIANGLE_NORMALS_OFFSET) );
//    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(TRIANGLE_TEXCOORDS_OFFSET) );
//    glDrawArrays( GL_TRIANGLES, 0, NumVerticesTri );
    
    
    
    
    //----- Grinding Rail
    
    // Rail
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial(vec4(0.0, 0.0, 1.0, 1.0), vec4(0.925, 0.925, 0.925, 1.0), vec4(0.2, 0.2, 0.2, 1.0), 1.0);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    mat4 rail = Translate( -1, 0.2325, 2.75 ) * RotateY(90) * Scale(1.5, 0.04, 0.04) * RotateZ(90);
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
    
    mat4 post = Translate( -1, 0.122, 3.475 ) * Scale(0.04, .265, 0.05);
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
    
    mat4 postTwo = Translate( -1, 0.120, 2.0 ) * Scale(0.04, .265, 0.05);
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



    
    
    
    glutSwapBuffers();
}




//----------------------------------------------------------------------------

float popAngle = 30;
float counter = 0;
int turnFacing = 0;
bool northFacing, southFacing, eastFacing, westFacing = false;// if 1 then skateRight/skateLeft, else skateForward/Backward
bool skateForward = false, skateBackwards = false, skateRight = false, skateLeft = false;
bool inQOne, inQTwo, inQThree, inQFour = false;
bool turnRight = false, turnLeft = false, turnDone = false;
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
            
        case 't':  // turn right 90deg
            turnRight = true;
            turnDone = false;
            turnLeft = false;
            glutPostRedisplay();
            break;
            
        case 'T':  // turn left 90deg
            turnLeft = true;
            turnDone = false;
            turnRight = false;
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
            
//        case 'p': // pedal backwards
//            skateBackwards = true;
//            skateForward = false;
//            glutPostRedisplay();
//            break;
            
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
    
    float frontBoundary = 3.0;
    float rearBoundary = -3.0;
    float step = 0.1/10;
    float oneRadian = M_PI/180;
    
    if(fmod(thetaY,360) == 0)
    {
        northFacing = true;
        southFacing = false;
        eastFacing = false;
        westFacing = false;
    }
    else if(fmod(thetaY,270) == 0)
    {
        northFacing = false;
        southFacing = false;
        eastFacing = true;
        westFacing = false;
    }
    else if(fmod(thetaY,180) == 0)
    {
        northFacing = false;
        southFacing = true;
        eastFacing = false;
        westFacing = false;
    }
    else if(fmod(thetaY,90) == 0)
    {
        northFacing = false;
        southFacing = false;
        eastFacing = false;
        westFacing = true;
    }
    
    if (skateForward)
    {
      if(northFacing)
      {
        boardZ += step;
        frontTrAxZ += step;
        frontWheelsZ += step;
        rearTrAxZ += step;
        rearWheelsZ += step;
        boardZ = fmod(boardZ,3.0);
        rearWheelsZ = fmod(rearWheelsZ,frontBoundary);
        frontWheelsZ = fmod(frontWheelsZ,frontBoundary);
        rearTrAxZ = fmod(rearTrAxZ,frontBoundary);
        frontTrAxZ = fmod(frontTrAxZ,frontBoundary);
        if(frontBoundary-frontTrAxZ <= 0.3)
        {
          skateForward = false;
        }
      }
      else if(southFacing)
      {
          boardZ -= step;
          frontTrAxZ -= step;
          frontWheelsZ -= step;
          rearTrAxZ -= step;
          rearWheelsZ -= step;
          boardZ = fmod(boardZ,rearBoundary);
          rearWheelsZ = fmod(rearWheelsZ,rearBoundary);
          frontWheelsZ = fmod(frontWheelsZ,rearBoundary);
          rearTrAxZ = fmod(rearTrAxZ,rearBoundary);
          frontTrAxZ = fmod(frontTrAxZ,rearBoundary);
          if(frontBoundary + frontTrAxZ <= 0.1)
          {
              skateForward = false;
          }
      }
      else if(eastFacing)
      {
          boardX -= step;
          frontTrAxX -= step;
          frontWheelsX -= step;
          rearTrAxX -= step;
          rearWheelsX -= step;
          boardX = fmod(boardX,rearBoundary);
          rearWheelsX = fmod(rearWheelsX,rearBoundary);
          frontWheelsX = fmod(frontWheelsX,rearBoundary);
          rearTrAxX = fmod(rearTrAxX,rearBoundary);
          frontTrAxX = fmod(frontTrAxX,rearBoundary);
          if(frontBoundary + frontTrAxX <= 0.1)
          {
              skateForward = false;
          }
      }
      else if(westFacing)
      {
          boardX += step;
          frontTrAxX += step;
          frontWheelsX += step;
          rearTrAxX += step;
          rearWheelsX += step;
          boardX = fmod(boardX,rearBoundary);
          rearWheelsX = fmod(rearWheelsX,rearBoundary);
          frontWheelsX = fmod(frontWheelsX,rearBoundary);
          rearTrAxX = fmod(rearTrAxX,rearBoundary);
          frontTrAxX = fmod(frontTrAxX,rearBoundary);
          if(frontBoundary - frontTrAxX <= 0.1)
          {
              skateForward = false;
          }
      }
    }
    if (skateBackwards)
    {
      boardZ -= step;
      frontTrAxZ -= step;
      frontWheelsZ -= step;
      rearTrAxZ -= step;
      rearWheelsZ -= step;
      boardZ = fmod(boardZ,rearBoundary);
      rearWheelsZ = fmod(rearWheelsZ,rearBoundary);
      frontWheelsZ = fmod(frontWheelsZ,rearBoundary);
      rearTrAxZ = fmod(rearTrAxZ,rearBoundary);
      frontTrAxZ = fmod(frontTrAxZ,rearBoundary);
      if(frontBoundary + frontTrAxZ <= 0.1)
      {
          skateBackwards = false;
      }
    }
    if (turnLeft)
    {
        if(!turnDone)
        {
          thetaY += 9;
          if (fmod(thetaY,90) == 0)
          {
              turnDone = true;
              turnLeft = false;
              skateForward = false;
              skateBackwards = false;
          }
        }
        turnFacing += 1;
    }
    if (turnRight)
    {
        if(!turnDone)
        {
            thetaY -= 9;
            if (fmod(thetaY,90) == 0)
            {
                turnDone = true;
                turnRight = false;
                skateForward = false;
                skateBackwards = false;
            }
        }
        turnFacing += 1;
    }
    if (doManual)
    {
      if(!manualDone)
      {
        if(manualReached)
        {
          thetaX += 1;
          if(thetaX == 0)
          {
            thetaX = 0;
            manualDone = true;
            doManual = false;
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
              counter++;
              float oneRadian = M_PI/180;
              boardY -= oneRadian;
              frontTrAxY -= oneRadian;
              frontWheelsY -= oneRadian;
              rearTrAxY -= oneRadian;
              rearWheelsY -= oneRadian;
              if(counter >= popAngle)
              {
                  ollieDone = true;
                  counter = 0;
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















