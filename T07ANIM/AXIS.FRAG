/* Filename: PLANET.FRAG: */

// ������ ����� �������
#version 400

// �������� ��������� - ���� ���������
out vec4 OutColor;

// ������� ��������� �� ���������� �������
in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

/* �������� */
uniform float IsTextureUse;
uniform sampler2D DrawTexture;


void main( void )
{
  vec4 tc = vec4(1, 1, 1, 1);
  if (IsTextureUse > 0)
    tc = texture2D(DrawTexture, DrawTexCoord);    
  OutColor = tc * DrawColor;
}


