// OpenGL ES 2.0

#include "ruby2d.h"

#if GLES

// Triangle shader
static GLuint shaderProgram;
static GLuint positionLocation;
static GLuint colorLocation;

// Texture shader
static GLuint texShaderProgram;
static GLuint texPositionLocation;
static GLuint texColorLocation;
static GLuint texCoordLocation;
static GLuint samplerLocation;

static GLushort indices[] =
  { 0, 1, 2,
    2, 3, 0 };


/*
 * Applies the projection matrix
 */
void R2D_GLES_ApplyProjection(GLfloat orthoMatrix[16]) {

  // Use the program object
  glUseProgram(shaderProgram);

  glUniformMatrix4fv(
    glGetUniformLocation(shaderProgram, "u_mvpMatrix"),
    1, GL_FALSE, orthoMatrix
  );

  // Use the texture program object
  glUseProgram(texShaderProgram);

  glUniformMatrix4fv(
    glGetUniformLocation(texShaderProgram, "u_mvpMatrix"),
    1, GL_FALSE, orthoMatrix
  );
}


/*
 * Initalize OpenGL ES
 */
int R2D_GLES_Init() {

  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Vertex shader source string
  GLchar vertexSource[] =
    // uniforms used by the vertex shader
    "uniform mat4 u_mvpMatrix;"   // projection matrix

    // attributes input to the vertex shader
    "attribute vec4 a_position;"  // position value
    "attribute vec4 a_color;"     // input vertex color
    "attribute vec2 a_texcoord;"  // input texture

    // varying variables, input to the fragment shader
    "varying vec4 v_color;"       // output vertex color
    "varying vec2 v_texcoord;"    // output texture

    "void main()"
    "{"
    "  v_color = a_color;"
    "  v_texcoord = a_texcoord;"
    "  gl_Position = u_mvpMatrix * a_position;"
    "}";

  // Fragment shader source string
  GLchar fragmentSource[] =
    "precision mediump float;"
    // input vertex color from vertex shader
    "varying vec4 v_color;"

    "void main()"
    "{"
    "  gl_FragColor = v_color;"
    "}";

  // Fragment shader source string for textures
  GLchar texFragmentSource[] =
    "precision mediump float;"
    // input vertex color from vertex shader
    "varying vec4 v_color;"
    "varying vec2 v_texcoord;"
    "uniform sampler2D s_texture;"

    "void main()"
    "{"
    "  gl_FragColor = texture2D(s_texture, v_texcoord) * v_color;"
    "}";

  // Load the vertex and fragment shaders
  GLuint vertexShader      = R2D_GL_LoadShader(  GL_VERTEX_SHADER,      vertexSource, "GLES Vertex");
  GLuint fragmentShader    = R2D_GL_LoadShader(GL_FRAGMENT_SHADER,    fragmentSource, "GLES Fragment");
  GLuint texFragmentShader = R2D_GL_LoadShader(GL_FRAGMENT_SHADER, texFragmentSource, "GLES Texture Fragment");

  // Triangle Shader //

  // Create the shader program object
  shaderProgram = glCreateProgram();

  // Check if program was created successfully
  if (shaderProgram == 0) {
    R2D_GL_PrintError("Failed to create shader program");
    return GL_FALSE;
  }

  // Attach the shader objects to the program object
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  // Link the shader program
  glLinkProgram(shaderProgram);

  // Check if linked
  R2D_GL_CheckLinked(shaderProgram, "GLES shader");

  // Get the attribute locations
  positionLocation = glGetAttribLocation(shaderProgram, "a_position");
  colorLocation    = glGetAttribLocation(shaderProgram, "a_color");

  // Texture Shader //

  // Create the texture shader program object
  texShaderProgram = glCreateProgram();

  // Check if program was created successfully
  if (texShaderProgram == 0) {
    R2D_GL_PrintError("Failed to create shader program");
    return GL_FALSE;
  }

  // Attach the shader objects to the program object
  glAttachShader(texShaderProgram, vertexShader);
  glAttachShader(texShaderProgram, texFragmentShader);

  // Link the shader program
  glLinkProgram(texShaderProgram);

  // Check if linked
  R2D_GL_CheckLinked(texShaderProgram, "GLES texture shader");

  // Get the attribute locations
  texPositionLocation = glGetAttribLocation(texShaderProgram, "a_position");
  texColorLocation    = glGetAttribLocation(texShaderProgram, "a_color");
  texCoordLocation    = glGetAttribLocation(texShaderProgram, "a_texcoord");

  // Get the sampler location
  samplerLocation = glGetUniformLocation(texShaderProgram, "s_texture");

  // Clean up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(texFragmentShader);

  return GL_TRUE;
}


/*
 * Draw triangle
 */
void R2D_GLES_DrawTriangle(GLfloat x1, GLfloat y1,
                           GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
                           GLfloat x2, GLfloat y2,
                           GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
                           GLfloat x3, GLfloat y3,
                           GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3) {

  GLfloat vertices[] =
    { x1, y1, 0.f,
      x2, y2, 0.f,
      x3, y3, 0.f };

  GLfloat colors[] =
    { r1, g1, b1, a1,
      r2, g2, b2, a2,
      r3, g3, b3, a3 };

  glUseProgram(shaderProgram);

  // Load the vertex position
  glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  glEnableVertexAttribArray(positionLocation);

  // Load the colors
  glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, colors);
  glEnableVertexAttribArray(colorLocation);

  // draw
  glDrawArrays(GL_TRIANGLES, 0, 3);
}


/*
 * Draw a texture (New method with vertices pre-calculated)
 */
void R2D_GLES_DrawTexture(GLfloat coordinates[], GLfloat texture_coordinates[], GLfloat color[], int texture_id);
  GLfloat vertices[] =
  //  x, y coords | x, y texture coords
  {
    coordinates[0], coordinates[1], 0.f, texture_coordinates[0], texture_coordinates[1],
    coordinates[2], coordinates[3], 0.f, texture_coordinates[2], texture_coordinates[3],
    coordinates[4], coordinates[5], 0.f, texture_coordinates[4], texture_coordinates[5],
    coordinates[6], coordinates[7], 0.f, texture_coordinates[6], texture_coordinates[7] };

  GLfloat colors[] =
    { color[0], color[1], color[2], color[3],
    color[0], color[1], color[2], color[3],
    color[0], color[1], color[2], color[3],
    color[0], color[1], color[2], color[3] };

  glUseProgram(texShaderProgram);

  // Load the vertex position
  glVertexAttribPointer(texPositionLocation, 3, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), vertices);
  glEnableVertexAttribArray(texPositionLocation);

  // Load the colors
  glVertexAttribPointer(texColorLocation, 4, GL_FLOAT, GL_FALSE, 0, colors);
  glEnableVertexAttribArray(texColorLocation);

  // Load the texture coordinate
  glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), &vertices[3]);
  glEnableVertexAttribArray(texCoordLocation);

  // Bind the texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  // Set the sampler texture unit to 0
  glUniform1i(samplerLocation, 0);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}


#endif
