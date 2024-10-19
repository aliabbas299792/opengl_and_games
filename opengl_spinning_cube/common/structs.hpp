static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
}; //this is a opengl float array, containing 3 points for the backbuffer

// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices


//So (-1,-1) is the bottom left corner of your screen. (1,-1) is the bottom right, (1,1) is bottom left, and (1,-1) is top left [so (0,0) is the centre of the screen]

static GLfloat g_color_buffer_data[12 * 3 * 3];
float increment1 = 0.01;
float increment2 = 0.02;
float increment3 = 0.03;

void randomStructColourGen() {// One color for each vertex. They were generated randomly.
	for (int v = 0; v < 36; v++) {
		if (g_color_buffer_data[v * 3 + 0] >= 0.8) {
			increment1 *= -1;
		}
		else if (g_color_buffer_data[v * 3 + 0] <= 0.2) {
			increment1 *= -1;
		}

		if (g_color_buffer_data[v * 3 + 1] >= 0.8) {
			increment2 *= -1;
		}
		else if (g_color_buffer_data[v * 3 + 1] <= 0.2) {
			increment2 *= -1;
		}

		if (g_color_buffer_data[v * 3 + 2] >= 0.8) {
			increment3 *= -1;
		}
		else if (g_color_buffer_data[v * 3 + 2] <= 0.2) {
			increment3 *= -1;
		}

		g_color_buffer_data[v * 3 + 0]+= increment1;
		g_color_buffer_data[v * 3 + 1]+= increment2;
		g_color_buffer_data[v * 3 + 2]+= increment3;
	}
}