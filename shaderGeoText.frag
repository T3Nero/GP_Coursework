//Version number
#version 400
//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;

//Unfrom variable
uniform float randColourX;
uniform float randColourY;
uniform float randColourZ;

uniform sampler2D diffuseT;

in GS_OUT
{
	vec2 texCoords;
} fs_in;

 
void main()
{
//Setting each vector component to uniform varaible then setting final colour
	vec4 color;

	//color = vec4(randColourX,randColourY,randColourZ,1.0);
	color = texture2D(diffuseT, fs_in.texCoords);

    fragcolor = color;
}