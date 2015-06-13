/* Filename: TEST.FRAG: */

#version 330

uniform float Time;

out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawPosition;

void main( void )
{
  OutColor = DrawColor;
}