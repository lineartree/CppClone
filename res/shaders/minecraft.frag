#version 330 core
in vec3 TexCoord;
in float shading_value;
out vec4 FragColor;

uniform sampler2DArray texture_sampler;

void main()
{
  vec4 texColor = texture(texture_sampler, TexCoord);
  FragColor = texColor * shading_value;
  if (texColor.a == 0.0)
  {
    discard;
  }
}
