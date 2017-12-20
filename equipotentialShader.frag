#define PI 3.14159265
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec3 chargeArray[NUMBER_OF_CHARGES];
//every element in chargeArray is of the format vec3(pos.x, pos.y, q)


void main( void ) {
	vec2 position = gl_TexCoord[0].xy;
	float potential = 0.0;
	int i;
	for(i = 0; i < NUMBER_OF_CHARGES; i++){
		vec2 chargePos = chargeArray[i].xy/u_resolution.y;
		vec2 r = position - chargePos;
		potential += 0.005 * chargeArray[i].z/length(r);
	}
	potential = clamp(potential, -1.0, 1.0);
  float cutoff = sin(2.0*PI*10.0*potential);
  cutoff = smoothstep(1.0,0.0, 0.5*abs(cutoff)/fwidth(cutoff));
	//Maps negative potential to blue
	// and positive potential to red
	gl_FragColor =  cutoff * vec4(potential, 0.0, -potential, 1.0);
}
