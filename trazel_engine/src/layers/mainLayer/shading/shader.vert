#version 450

vec2 positions[6] = vec2[]
(
    // the cordianates are going clockwise
    //  (x, y)
	vec2(0.0, -1.0),    // this is dot has to be closest to top right corner
	vec2(-1.0, 1.0),   // this is dot has to be closest to bottom of the screen
	vec2(-1.0, -1.0), // this is dot has to be closest to top left corner
	vec2(0.0, -1.0),
	vec2(0.0, 1.0),
	vec2(-1.0, 1.0)
);

vec3 colors[6] = vec3[]
(
	vec3(1.0, 0.0, 0.5),
	vec3(1.0, 0.5, 0.0),
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 0.5, 1.0),
	vec3(0.5, 0.0, 1.0),
	vec3(0.0, 0.0, 1.0)
);

layout(location = 0) out vec3 fragColor;

void main() 
{
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
	fragColor = colors[gl_VertexIndex];
}