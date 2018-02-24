#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 drawColor;
uniform vec3 objId;

uniform sampler2D tex;

void main()
{
	vec3 color=vec3(objId.r,objId.g,objId.b);
	gl_FragColor =  clamp(vec4(color,1.0), 0.0, 1.0);
}
