#version 330 core
layout (points) in;
layout (points, max_vertices = 1) out;

in VS_OUT {
	vec2 TexCoords;
	vec3 fragPosVec;
	vec3 normalVec;
} gs_in[];

out FS_OUT {
	vec2 TexCoords;
	vec3 fragPosVec;
	vec3 normalVec;
} fs_out;

void main() {    
    gl_Position = gl_in[0].gl_Position;

	fs_out.TexCoords = gs_in[0].TexCoords;
	fs_out.fragPosVec = gs_in[0].fragPosVec;
	fs_out.normalVec = gs_in[0].normalVec;

    EmitVertex();
    EndPrimitive();
} 