#version 330 core
layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

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

void main(void)
{
    int i;

    for (i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
		fs_out.TexCoords = gs_in[i].TexCoords;
		fs_out.fragPosVec = gs_in[i].fragPosVec;
		fs_out.normalVec = gs_in[i].normalVec;
        EmitVertex();
    }
    EndPrimitive();
}