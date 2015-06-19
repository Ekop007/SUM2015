/* Filename: PLANET.FRAG: */

// версия языка шейдера
#version 330

// время
uniform float Time;
uniform float PartNo;
uniform float TotalParts;

uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform mat4 MatrWInverse;


// выходные параметры - цвет рисования
out vec4 OutColor;

// входные параметры от вершинного шейдера
in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

/*** Освещение ***/
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Kp;
uniform float Kt;

/* Текстура */
uniform float IsTextureUse;
uniform sampler2D DrawTexture;


/* Функция вычисления освещенности */
vec4 Shade( vec3 P, vec3 N )
{
  float a = 1;
  vec3 color = vec3(0, 0, 0);
  vec3 L[3], LC[3];
  vec3 V = normalize(vec3(MatrWVInverse * vec4(0, 0, 0, 1)) - P);

  L[0] = normalize(vec3( 0, 100, 0));
 /* L[1] = normalize(vec3(0, 15, sin(Time * 1.2 + 3) * 8) - P);
  L[2] = normalize(vec3(sin(Time * 2.4 + 3) * 5, 15, sin(Time * 1.2 + 3) * 8) - P);
   */
  LC[0] = vec3(1, 1, 1) * 0.85;
 // LC[1] = vec3(1, 1, 1) * 0.930;
 // LC[2] = vec3(1, 1, 1) * 0.930;
    // разворачиваем нормаль, если смотрим в "тыл" треугольнику
  N = faceforward(N, V, N);
  N = normalize(N);

  /*
  vec3
    Ka = vec3(0, 0, 0),
    Kd = vec3(0.6, 0.6, 0.6),
    Ks = vec3(0.5, 0.5, 0.5);
  float Kp = 1;
    */

  // рассеянный (ambient)
  color = Ka;
  vec3 R = reflect(V, N);
  for (int i = 0; i < 1; i++)
  {
    // диффузное отражение
    float nl = dot(N, L[i]); 
    if (nl > 0)
    {
      if (IsTextureUse > 0)
      {
        vec4 tc = texture2D(DrawTexture, vec2(1, -1) * DrawTexCoord);
        a = tc.a;
        color += LC[i] * Kd * tc.rgb * nl;
      }
      else
        color += LC[i] * Kd * nl;
    }

  // зеркальное отражение
    float rl = dot(R, L[i]); 
    if (rl > 0)
      color += LC[i] * Ks * pow(rl, Kp);
  }
  return vec4(0.6 * color, (Kt * a));
}

void main( void )
{
 /*if (PartNo == 11)
    discard;*/   
  OutColor = DrawColor * Shade(DrawPosition.xyz, DrawNormal);
}


