#version 400

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 normalMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

out vec3 fNormal;
out vec3 fPosition;

uniform float time;

const float pi=3.14159;
out vec3 modelX;
out vec3 modelN;
out vec3 rawX;

vec2 Rotate2D(vec2 vec_in, float angle)
{
  vec2 vec_out;
  vec_out.x=cos(angle)*vec_in.x-sin(angle)*vec_in.y;
  vec_out.y=sin(angle)*vec_in.x+cos(angle)*vec_in.y;
  return vec_out;
}

void main()
{
  modelX=position;
  rawX=position;
  modelN=normal;  
  
  // Comment these lines out to stop twisting
  modelX.xz = Rotate2D(modelX.xz,0.5*pi*modelX.y*sin(10.0*time)); // Try commenting out *just* this line :)
  modelN.xz = Rotate2D(modelN.xz,0.5*pi*modelX.y*sin(10.0*time)); // This is simple as that only since the transform is rotation
  
  //fNormal = normalize(normalMatrix * modelN);
  //fNormal = inverse(transpose(mat3(modelViewMatrix))) * normalize(normalMatrix * modelN);

  fNormal = mat3(transpose(inverse(normalMatrix))) * normal;

  vec4 pos = modelViewMatrix * vec4(modelX, 1.0);
  fPosition = pos.xyz;
  gl_Position = projectionMatrix * pos;
}