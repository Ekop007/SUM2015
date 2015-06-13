/* Filename: TEST.VERT */

#version 330

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec4 InColor;

uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform float Time;

out vec4 DrawColor;
out vec3 DrawPosition;

void main( void )
{
  gl_Position = MatrProj * MatrView * MatrWorld;
  // gl_Position = vec4(0.3, 0, 0.3, 1);
  DrawColor = InColor;
  DrawPosition = InPosition;
}