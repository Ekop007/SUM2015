/* Filename: TEST.VERT */

// версия языка шейдера
#version 400

// кто куда приходит
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// глобальные переменные
uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform mat4 MatrWInverse;
uniform float Time;

// выходные параметры
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPosition;
out vec2 DrawTexCoord;

void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawNormal = mat3(MatrWInverse) * InNormal;
  DrawPosition = vec3(MatrWorld * vec4(InPosition, 1));
  DrawColor = InColor;
  DrawTexCoord = InTexCoord;
}
