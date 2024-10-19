GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) { //the shader file paths are the params of the function
	//const char* is a pointer to the memory location with the data in it, read only

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	//makes a vertex shader object (Empty)
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	//makes a fragment shader object (empty)

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode; //makes a string to hold the vertex shader stuff
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in); 
	//makes an object VertexShaderStream, from which the vertex shader file can be read
		//param 1 specifies the location of the file
		//param 2 means that this is going to be for reading operations

	if (VertexShaderStream.is_open()) {
		std::stringstream sstr; //allows for stream like manipulations (good for this purpose)
		sstr << VertexShaderStream.rdbuf(); //the buffer is read, and stored in sstr
		VertexShaderCode = sstr.str(); //converts it to string and stores it in that string
		VertexShaderStream.close(); //closes the stream
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;

		//else gives an error in the command console, and waits for user input to console
	}

	// Read the Fragment Shader code from the file
	//Same thing as for the vertex shader
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE; //equivalent to 0/false
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();  //makes the vertex shader code string into `const char*`
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		//param 1 -> the object in which to store the shader code
		//param 2 -> the amount of elements in the array which will be provided
		//param 3 -> specifies the pointer/array of pointers to string shader source code
		//param 4 -> if not null, points to an array of the string lengths
	glCompileShader(VertexShaderID); //compiles shader code into machine code

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		//param 1 -> specifies shader object to be queried
		//param 2 -> what you are looking for related to it
		//param 3 -> store it in the location of `Result`
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		//same thing but for GL_INFO_LOG_LENGTH and in location &InfoLogLength
	if (InfoLogLength > 0) {	//if there are errors
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1); //makes a vector of that name with a size of 1 greater than infologlength
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			//1st param -> the object to query
			//2nd param -> the size of the buffer to store returning infromation
			//3rd param -> returns the length of the string returned in infolog (next param)
			//4th param -> specifies an array of characters that is used to store the returning data
		printf("%s\n", &VertexShaderErrorMessage[0]); //print the error
	}

	// Compile Fragment Shader
	//the same as for vertex shader, but for fragment shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram(); //creates empty program project, to which shaders are attatched
	glAttachShader(ProgramID, VertexShaderID); //attatches the vertex shader
	glAttachShader(ProgramID, FragmentShaderID); //attatches the fragment shader
	glLinkProgram(ProgramID); //makes executables of the shaders which work on their respective processors (geometry, vector or fragment)

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result); //gets the GL_LINK_STATUS for the program and stores it in &Result (GL_TRUE or GL_FALSE)
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	//same thing but for GL_INFO_LOG_LENGTH and in location &InfoLogLength
	if (InfoLogLength > 0) { //if longer than 0, means there are errors
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1); //makes a vector of size one greater than infologlength
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		//1st param -> the object to query
		//2nd param -> the size of the buffer to store returning infromation
		//3rd param -> returns the length of the string returned in infolog (next param)
		//4th param -> specifies an array of characters that is used to store the returning data
		printf("%s\n", &ProgramErrorMessage[0]); //print the error
	}

	glDetachShader(ProgramID, VertexShaderID); //detatches the vertex shader
	glDetachShader(ProgramID, FragmentShaderID); //detatches the fragment shader

	glDeleteShader(VertexShaderID); //deletes the vertex shader
	glDeleteShader(FragmentShaderID); //deletes the fragment shader
	
	return ProgramID; //the program id is returned, which the shaders (which are exectuables) can be called by
}