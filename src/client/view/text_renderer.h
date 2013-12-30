#pragma once

#include <string>
#include <GL/glew.h>

#include "client/view/color.h"
#include "client/view/shader.h"

class TextRenderer : public Shader
{
public:
  TextRenderer(GLuint);

  void On() const;
  void DrawText(float, float, float, const std::string&, const Color&) const;

private:
  GLuint m_font_texture;
  size_t vertex_elem_size_;
  GLint bg_color_uni_;
  Color* bg_color_;
};
