#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 drawColor;

uniform sampler2D tex;

void main()
{
	vec3 color=drawColor;
	if(drawColor==vec3(0,0,0)){
		color = texture(tex,texCoord0).xyz;
	}
	vec3 tmp = dot(-lightDirection, normal0) *color ;
	gl_FragColor =  clamp(vec4(tmp,1.0), 0.0, 1.0);
}
