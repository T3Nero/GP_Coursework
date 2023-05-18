#version 420 core

layout(location = 0) out vec4 FragColor;

in Vertex_DATA{
    vec2 tC;
    vec3 Normal;
    vec3 Position;
} fs_In;

uniform vec3 camPos;
uniform samplerCube skybox;
layout (binding = 0) uniform sampler2D diffuse;

uniform float time;
uniform float maxDist; //fog max distance
uniform float minDist; //fog min distance
uniform float fogDensity;
uniform vec3 fogColor;

uniform sampler2D noiseTexture; // noise 
uniform sampler2D skyTexture; // sky
uniform sampler2D lavaTexture; // lava

void main() 
{
    vec3 I = -normalize(fs_In.Position + camPos);
    vec3 R = reflect(I, normalize(fs_In.Normal));

    // environment map colour
    //vec4 envColour = vec4(texture(skybox, R).rgb, 1.0);
	vec4 envColour = texture(skybox, R);

    // diffuse texture colour
    vec4 diffuseColour = texture2D(diffuse, fs_In.tC);

    // used to control which colour has more strength(alpha) when mixing
    float mixFactor = 0.9;
    mixFactor = clamp(mixFactor, 0.0, 1.0);

    // Calculate the final colour by mixing the diffuse colour and the environment colour
    vec4 finalColour = mix(diffuseColour, envColour, mixFactor);


	//Noise

	vec4 noise = texture2D( noiseTexture, fs_In.tC ); //noise texture

	vec4 noiseMix = mix(finalColour, noise, 0.2);

	vec2 T1 = fs_In.tC + vec2( 1.5, - 1.5 ) * time * 2.0; // "time" 'animates' the texture
	
	vec2 T2 = fs_In.tC + vec2( - 0.5, 2.0 ) * time * 2.0; // as above

	T1.x -= noiseMix.x * 0.2; //offset change these 4 values to see the change in frequency, see notes at end of shader.
	T1.y += noiseMix.y * 0.2;
	T2.x -= noiseMix.x * 0.2; //this just offsets the texture coordinates
	T2.y += noiseMix.y * 0.2; // but allows us to offset y&z in opposite directions


	float p = texture2D( noiseTexture, T1).a; //get the alpha from the noise texture

	vec4 color = texture2D( skyTexture, T2); //coloured texture offset can here or above
	vec4 color2 = texture2D( lavaTexture, T2); //coloured texture offset can here or above
				
	vec4 temp = color * envColour * ( vec4(p) * 2.0 ) + color2; //add/remove the last colour

	if( temp.r > 1.0 ) { temp.bg += clamp( temp.r - 2.0, 0.0, 10.0 ); } // again play about with these
	if( temp.g > 1.0 ) { temp.rb += temp.g - 1.0; }
	if( temp.b > 1.0 ) { temp.rg += temp.b - 1.0; } // = vec2(0.0,0.0)

	float dist = abs( fs_In.Position.z ); //absolute value
	float fogFactor = (maxDist - dist)/(maxDist - minDist);
	fogFactor = clamp( fogFactor, 0.0, 1.0 ); // constrain range

	vec4 noiseColor = mix( temp, vec4( fogColor, temp.w ), 0.2 );

    // Output the final colour
	//FragColor = noiseColor;
    FragColor = mix(finalColour, noiseColor, 0.5);

}
