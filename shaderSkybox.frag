#version 330 core
out vec4 FragColor;

in vec3 skyboxTC;
in vec4 v_pos;
in vec2 noiseTc;

uniform samplerCube skybox;

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
    // Sample the skybox texture
    vec4 sky = texture(skybox, skyboxTC);

	// Sample noise texture2D
	vec4 noise = texture2D( noiseTexture, noiseTc );

	vec4 noiseMix = mix(sky, noise, 0.5);

	vec2 T1 = noiseTc + vec2( 1.5, - 1.5 ) * time * 2.0; // "time" 'animates' the texture
	
	vec2 T2 = noiseTc + vec2( - 0.5, 2.0 ) * time * 2.0; // as above

	T1.x -= noiseMix.x * 0.2; //offset change these 4 values to see the change in frequency
	T1.y += noiseMix.y * 0.2;
	T2.x -= noiseMix.x * 0.2; //this just offsets the texture coordinates
	T2.y += noiseMix.y * 0.2; 


	float noiseAlpha = texture2D( noiseTexture, T1).a; //get the alpha from the noise texture

	vec4 skyColor = texture2D( skyTexture, T2); //coloured texture offset
	vec4 lavaColor = texture2D( lavaTexture, T2); 
				
	vec4 colourTint = skyColor * sky * ( vec4(noiseAlpha) * 2.0 ) + lavaColor;

	float dist = abs( v_pos.z ); //absolute value
	float fogFactor = (maxDist - dist)/(maxDist - minDist);
	fogFactor = clamp( fogFactor, 0.0, 1.0 ); // constrain range

	vec4 noiseColor = mix( colourTint, vec4( fogColor, fogDensity ), fogFactor );

	FragColor = mix(noiseMix, noiseColor, 0.5);
}
