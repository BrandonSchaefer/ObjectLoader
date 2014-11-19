#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "Obj.h"
#include "ObjLexer.h"
#include "Matrix4x4.h"
#include "SDLTimer.h"
#include "Shaders.h"

#include <SDL2/SDL.h>
#include <GLES2/gl2.h>

#define GL_CHECK(x) \
        x; \
        { \
          GLenum glError = glGetError(); \
          if(glError != GL_NO_ERROR) { \
            SDL_Log("glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__); \
          } \
        }

namespace
{
  int const WIDTH  = 800;
  int const HEIGHT = 600;
  int const FRAMES_PER_SECOND = 60;
  float const ONE_SECOND      = 1000.0f;
}

std::string ReadInFile(char const* path)
{
  std::string str;
  std::ifstream is(path, std::ifstream::binary);

  if (is)
  {
    is.seekg(0, is.end);
    int length = (int)is.tellg();
    is.seekg(0, is.beg);

    char* buff = new char[length];

    is.read(buff, length);
    is.close();

    buff[length] = '\0';

    str = std::string(buff);
  }
  else
  {
    fprintf(stderr, "Failed to load file: %s\n", path);
  }

  return str;
}

GLuint LoadShader(char const* src, GLenum type)
{
  GLuint shader = glCreateShader(type);
  if (shader)
  {
    GLint compiled;
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
      GLint info_len;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);

      if (info_len > 1)
      {
        char* info_log = (char*)malloc(sizeof(char) * info_len);

        glGetShaderInfoLog(shader, info_len, NULL, info_log);
        std::cerr << "Failed to comiple shader:\n" << info_log << std::endl;

        free(info_log);
      }

      glDeleteShader(shader);

      return 0;
    }
  }

  return shader;
}

GLint LoadDefaultProgram()
{
  GLint program;
  GLuint vertex, fragment;

  vertex   = LoadShader(VERTEX_SHADER_SRC,   GL_VERTEX_SHADER);
  fragment = LoadShader(FRAGMENT_SHADER_SRC, GL_FRAGMENT_SHADER);

  if (!vertex || !fragment)
    return 0;

  program = glCreateProgram();

  if (program == 0)
    return 0;

  glAttachShader(program, vertex);
  glAttachShader(program, fragment);

  glLinkProgram(program);

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);

  if (!linked)
  {
    GLint info_len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);

    if (info_len > 1)
    {
      char* info_log = (char*)malloc(sizeof(char) * info_len);
      glGetProgramInfoLog(program, info_len, NULL, info_log);

      std::cerr << "Failed to link shaders:\n" << info_log << std::endl;
      free(info_log);
    }

    glDeleteProgram(program);
    return 0;
  }

  return program;
}

void InitSDL()
{
  try
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
      throw std::runtime_error("Failed to INIT SDL");
  }
  catch (std::runtime_error const& e)
  {
    std::cerr << "Caught runtime error: " << e.what() << std::endl;
  }
}

int main()
{
  InitSDL();

  std::string raw_object = ReadInFile("teapot.obj");
  obj_loader::Obj obj(raw_object);

  bool done = false;
  obj_loader::SDLTimer fps;

  SDL_Window*   window;
  SDL_GLContext context;

  // FIXME move to a different function with a try/catch
  window = SDL_CreateWindow("Object Loader",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

  if (!window)
    throw std::runtime_error("Failed to create SDL_Window");

  context = SDL_GL_CreateContext(window);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

  GLint program = LoadDefaultProgram();
  glUseProgram(program);

  SDL_GL_MakeCurrent(window, context);

  // Shader specific code
  GLint attr_position = glGetAttribLocation(program, "v_position");
  GLint attr_color    = glGetAttribLocation(program, "a_color");
  GLint attr_mvp      = glGetUniformLocation(program, "u_mvp");

  float max_vertex = obj.MaxVertex();

  GL_CHECK(glEnableVertexAttribArray(attr_position));

  GL_CHECK(glVertexAttribPointer(attr_position, 3, GL_FLOAT, GL_FALSE, 0, obj.RawFaces()));

  GLfloat color[4] = {1.0, 0.0f, 0.0f, 1.0f};
  GL_CHECK(glVertexAttrib4fv(attr_color, color));

  GL_CHECK(glEnable(GL_DEPTH_TEST));
  GL_CHECK(glDisable(GL_CULL_FACE));

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  // TODO CLEAN THIS UP :) Proof of conecpt
  float angle_x    = 0.0f;
  float angle_y    = 50.0f;
  float zoom_diff  = 0.0f;
  bool x_down_w = false;
  bool x_down_s = false;
  bool y_down_a = false;
  bool y_down_d = false;
  bool zoom_in  = false;
  bool zoom_out = false;

  while (!done)
  {
    fps.start();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_KEYDOWN:
        {
          if (event.key.keysym.sym == SDLK_w)
          {
            x_down_w = true;
          }
          else if (event.key.keysym.sym == SDLK_s)
          {
            x_down_s = true;
          }
          else if (event.key.keysym.sym == SDLK_a)
          {
            y_down_a = true;
          }
          else if (event.key.keysym.sym == SDLK_d)
          {
            y_down_d = true;
          }
          else if (event.key.keysym.sym == SDLK_UP)
          {
            zoom_in = true;
          }
          else if (event.key.keysym.sym == SDLK_DOWN)
          {
            zoom_out = true;
          }
        }
          break;
        case SDL_KEYUP:
        {
          if (event.key.keysym.sym == SDLK_w)
          {
            x_down_w = false;
          }
          else if (event.key.keysym.sym == SDLK_s)
          {
            x_down_s = false;
          }
          else if (event.key.keysym.sym == SDLK_a)
          {
            y_down_a = false;
          }
          else if (event.key.keysym.sym == SDLK_d)
          {
            y_down_d = false;
          }
          else if (event.key.keysym.sym == SDLK_UP)
          {
            zoom_in = false;
          }
          else if (event.key.keysym.sym == SDLK_DOWN)
          {
            zoom_out = false;
          }
          else if (event.key.keysym.sym == SDLK_ESCAPE)
          {
            done = true;
          }
        }
          break;
        case SDL_QUIT:
          return false;
        default:
          break;
      }
    }

    if (x_down_w)
    {
      angle_x -= 1.0f;
    }
    if(x_down_s)
    {
      angle_x += 1.0f;
    }

    if (y_down_a)
    {
      angle_y -= 1.0f;
    }
    if (y_down_d)
    {
      angle_y += 1.0f;
    }

    if (zoom_in)
    {
      zoom_diff -= 2.0f;
    }
    if (zoom_out)
    {
      zoom_diff += 2.0f;
    }

    // Render function
    matrix::Matrix4x4 mat_rot, mat_persp, mat_model, mat_mvp;

    mat_model.Rotate(angle_x, 1.0f, 0.0f, 0.0f);
    mat_rot.Rotate  (angle_y, 0.0f, 1.0f, 0.0f);

    mat_model = mat_model * mat_rot;

    mat_rot.Rotate(50, 0.0f, 1.0f, 0.0f);

    mat_model = mat_model * mat_rot;

    /* Pull the camera back from the cube */
    mat_model.matrix_.m[3][2] -= (max_vertex * 2) + zoom_diff;

    mat_persp.Perspective(45.0f, (float)WIDTH/HEIGHT, 0.01, 500);
    mat_mvp = mat_model * mat_persp;

    mat_mvp.Scale(0.3, 0.3, 0.3);

    GL_CHECK(glUniformMatrix4fv(attr_mvp, 1, GL_FALSE, (GLfloat*)mat_mvp.GetGLMatrix().m));

    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, obj.NumOfFaces() * 3));

    // End of render

    SDL_GL_SwapWindow(window);

    if (fps.ticks() < ONE_SECOND / FRAMES_PER_SECOND)
      SDL_Delay((ONE_SECOND / FRAMES_PER_SECOND) - fps.ticks());
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);

  SDL_Quit();
}
