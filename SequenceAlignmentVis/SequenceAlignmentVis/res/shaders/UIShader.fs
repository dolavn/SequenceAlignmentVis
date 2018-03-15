#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 drawColor;
uniform vec3 objId;

uniform sampler2D tex;

const float BORDER_SIZE = 0.01f;

void main()
{
	vec3 color=drawColor;
	if(texCoord0.x<BORDER_SIZE || texCoord0.x>1.0f-BORDER_SIZE){
		color = vec3(0,0,0);
	}
	if(drawColor==vec3(0,0,0)){
		color = texture(tex,texCoord0).xyz;
	}
	vec3 tmp = dot(-lightDirection, normal0) *color ;
	gl_FragColor =  clamp(vec4(tmp,1.0), 0.0, 1.0);
}
