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
	float alpha=1.0f;
	vec3 color=drawColor;
	color = texture(tex,texCoord0).xyz;
	if(color==vec3(0,0,0)){alpha=0.0f;}
	if(color==vec3(1,1,1)){color = drawColor;}
	vec3 tmp = dot(-lightDirection, normal0) *color ;
	gl_FragColor =  clamp(vec4(tmp,1.0), 0.0, alpha);
}
