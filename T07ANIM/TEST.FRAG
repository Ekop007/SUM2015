/* Filename: TEST.FRAG: */

// версия языка шейдера
#version 330

// время
uniform float Time;
uniform float PartNo;
uniform float TotalParts;

uniform mat4 MatrView;

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
  vec3 L = normalize(vec3(1, 1, 1));
  vec3 V = -vec3(-MatrView[3][0], -MatrView[3][1], -MatrView[3][2]);

  // разворачиваем нормаль, если смотрим в "тыл" треугольнику
  //N = faceforward(N, V, N);
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

  // диффузное отражение
  float nl = dot(N, L); 
  if (nl > 0)
  {
    if (IsTextureUse > 0)
    {
      vec4 tc = texture2D(DrawTexture, DrawTexCoord);
      a = tc.a;
      color += Kd * tc.rgb * nl;
    }
    else
      color += Kd * nl;
  }

  // зеркальное отражение
  vec3 R = reflect(V, N);
  float rl = dot(R, L); 
  if (rl > 0)
    ;//color += Ks * pow(rl, Kp);
  return vec4(0.6 * color, (Kt * a));
}

void main( void )
{
 /* if (PartNo)
    discard;    */
  OutColor = DrawColor * Shade(DrawPosition.xyz, DrawNormal);
}


