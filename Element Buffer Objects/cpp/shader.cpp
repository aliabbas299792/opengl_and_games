#include <glad/glad.h>
#include <glad/glad.c>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "../header/functionDefines.h"

unsigned int loadShaders(const char* v_file_path, const char* f_file_path){
    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::stringstream vertexShaderBuffer; //buffer for holding GLSL code which was read from the files, and which will be turned into a string for further processing
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream(v_file_path, std::ios::in);
    
    if(vertexShaderStream.is_open()){
        vertexShaderBuffer << vertexShaderStream.rdbuf();
        vertexShaderCode = vertexShaderBuffer.str();
        vertexShaderStream.close();
    }else{
        printf("Unable to open: %s", v_file_path);
        getchar();
        return -1;
    }

    std::stringstream fragmentShaderBuffer; //buffer for holding GLSL code which was read from the files, and which will be turned into a string for further processing
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream(f_file_path, std::ios::in);

    if(fragmentShaderStream.is_open()){
        fragmentShaderBuffer << fragmentShaderStream.rdbuf();
        fragmentShaderCode = fragmentShaderBuffer.str();
        fragmentShaderStream.close();
    }else{
        printf("Unable to open: %s", f_file_path);
        return -1;
    }

    //above is just retrieving the contents of the files

    int result = GL_FALSE;
    int infoLogLength;

    //compile vertex shader
    printf("Compiling shader: %s\n", v_file_path);
    const char* vertexSourcePointer = vertexShaderCode.c_str(); //stores the vertex shader inside of a const char
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
    //1st param is the handle of the shader in which to put the source code
    //2nd param is the number of elements in params 3 and 4, if they are arrays
    //3rd param is an array of pointers, or a single pointer, to the shader source code
    //4th param is, when not NULL, a pointer to an array of lengths corresponding to the array of pointers from param 3, so is NULL when just one pointer in param 3
    glCompileShader(vertexShaderID); //shader is compiled into machine code

    //compile fragment shader
    printf("Compiling shader: %s\n", f_file_path);
    const char* fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
    glCompileShader(fragmentShaderID);

    //check vertex shader for errors
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	//param 1 -> specifies shader object to be queried
	//param 2 -> what you are looking for related to it
	//param 3 -> store it in the location of `result`
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	//same thing but for GL_INFO_LOG_LENGTH and in location infoLogLength

    if(infoLogLength > 0){
        std::vector<char> VertexShaderErrorMessage(infoLogLength + 1); //makes a vector of that name with a size of 1 greater than infologlength, for the log char array
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		//1st param -> the object to query
		//2nd param -> the size of the buffer to store returning infromation
		//3rd param -> the length of the log isn't required beforehand, so you can pass NULL
		//4th param -> specifies an array of characters that is used to store the returning data
		printf("%s%s\n\n", &VertexShaderErrorMessage[0], v_file_path); //print the error
    }

    //checl fragment shader for errors
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0){
        std::vector<char> FragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s%s\n\n", &FragmentShaderErrorMessage[0], f_file_path);
    }

    //link the shader program itself
    printf("Linking program...\n");
	unsigned int programID = glCreateProgram(); //creates empty program project, to which shaders are attatched
	glAttachShader(programID, vertexShaderID); //attatches the vertex shader
	glAttachShader(programID, fragmentShaderID); //attatches the fragment shader
	glLinkProgram(programID); //makes shader program objects of the shaders which work on their respective processors (vector, fragment, etc...)

    //check program for errors
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0){
        std::vector<char> ProgramErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(programID, infoLogLength + 1, NULL, &ProgramErrorMessage[0]);
		//1st param -> the object to query
		//2nd param -> the size of the buffer to store returning infromation
		//3rd param -> the length of the log isn't required beforehand, so you can pass NULL
		//4th param -> specifies an array of characters that is used to store the returning data
		printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(programID, vertexShaderID); //detaches vertex shader objectfrom OpenGL program object
    glDetachShader(programID, fragmentShaderID); //detaches fragment shader object from OpenGL program object

    glDeleteShader(fragmentShaderID); //deletes vertex shader object
    glDeleteShader(vertexShaderID); //deletes fragment shader object

    return programID; //returns the compiled program's ID
}