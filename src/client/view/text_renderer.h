#ifndef CLIENT_VIEW_TEXT_RENDERER_H
#define CLIENT_VIEW_TEXT_RENDERER_H

#include <string>
#include <GL/glew.h>

#include "common/google.h"
#include "client/view/color.h"
#include "client/view/shader_program.h"

class TextRenderer {
public:
  TextRenderer();

  void Init(GLuint);
  void DrawText(float, float, float, const std::string&, const Color&) const;

private:
  DISALLOW_COPY_AND_ASSIGN(TextRenderer);
  ShaderProgram sp_;
  size_t vertex_elem_size_;
  GLint bg_color_uni_;
  Color* bg_color_;
};

#endif