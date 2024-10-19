#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../header/classDefines.h"

Shader::Shader(const char* v_file_path, const char* f_file_path){
    //retrieve code
    std::stringstream vertexShaderBuffer; //buffer for holding GLSL code which was read from the files, and which will be turned into a string for further processing
    std::string vertexShaderCode;
    std::ifstream vertexShaderStream;

    std::stringstream fragmentShaderBuffer; //buffer for holding GLSL code which was read from the files, and which will be turned into a string for further processing
    std::string fragmentShaderCode;
    std::ifstream fragmentShaderStream;

    fragmentShaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vertexShaderStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    //it'll throw an exception from either of them if their is a logical error in the reading of the stream (failbit)
    //or if there is a more severe error (badbit)

    try{
        vertexShaderStream.open(v_file_path);
        vertexShaderBuffer << vertexShaderStream.rdbuf();
        vertexShaderCode = vertexShaderBuffer.str();
        vertexShaderStream.close();

        fragmentShaderStream.open(f_file_path);
        fragmentShaderBuffer << fragmentShaderStream.rdbuf();
        fragmentShaderCode = fragmentShaderBuffer.str();
        fragmentShaderStream.close();
    }
    catch(std::ifstream::failure error){ //catch failures
        std::cout << "SHADER_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vertShaderSourcePtr = vertexShaderCode.c_str();
    const char* fragShaderSourcePtr = fragmentShaderCode.c_str();
    //the above two store the shader code inside of a const char

    //actual shaders
    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    //variables for error correction
    int result; //success or not
    int infoLogLength;
    char infoLog; //the log of any error info

    glShaderSource(vertexShaderID, 1, &vertShaderSourcePtr, NULL);
    //1st param is the handle of the shader in which to put the source code
    //2nd param is the number of elements in params 3 and 4, if they are arrays
    //3rd param is an array of pointers, or a single pointer, to the shader source code
    //4th param is, when not NULL, a pointer to an array of lengths corresponding to the array of pointers from param 3, so is NULL when just one pointer in param 3
    glCompileShader(vertexShaderID); //shader is compiled into machine code

    glShaderSource(fragmentShaderID, 1, &fragShaderSourcePtr, NULL);
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

    //check fragment shader for errors
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0){
        std::vector<char> FragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s%s\n\n", &FragmentShaderErrorMessage[0], f_file_path);
    }

    printf("Linking program...");

    ID = glCreateProgram();

    glAttachShader(ID, vertexShaderID);
    glAttachShader(ID, fragmentShaderID);
    glLinkProgram(ID);
    
    //check program for errors
    glGetProgramiv(ID, GL_LINK_STATUS, &result);
    glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0){
        std::vector<char> ProgramErrorMessage(infoLogLength + 1);
        glGetProgramInfoLog(ID, infoLogLength + 1, NULL, &ProgramErrorMessage[0]);
		//1st param -> the object to query
		//2nd param -> the size of the buffer to store returning infromation
		//3rd param -> the length of the log isn't required beforehand, so you can pass NULL
		//4th param -> specifies an array of characters that is used to store the returning data
		printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ID, vertexShaderID); //detaches vertex shader objectfrom OpenGL program object
    glDetachShader(ID, fragmentShaderID); //detaches fragment shader object from OpenGL program object

    glDeleteShader(fragmentShaderID); //deletes vertex shader object
    glDeleteShader(vertexShaderID); //deletes fragment shader object
}

void Shader::use(){
    glUseProgram(ID); //use this shader program
}

/*
glUniform1i/glUniform1f sets a uniform value of the currently active shader program, as in, it sets the value of the uniform name provided in the first 
param, which is supposed to be from vertex shader code/fragment shader code
however it can alternatively not be passed, and instead glGetUniformLocationID(...) can be used to retrieve the location of that uniform in the shader program
the two params taken by it are: firstly the ID of the shader program itself, and secondly the c stlye const char 'string' name of the unifrom we are
querying for in the shader program
*/

void Shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), (float)value);
}

void Shader::setMatrix4(const std::string &name, glm::mat4 &matrix) const{
    unsigned int transformLocation = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    //first param is the location
    //2nd param is how many matrices we'd like to send, so 1
    //3rd is if we want to swap the rows and columns, for a different matrix layout, no we don't so GL_FALSE
    //4th is the actual matrix data, which is converted from the GLM code into the actual OpenGL compatible stuff using glm::value_ptr([a matrix])
}

void Shader::set3Float(const std::string &name, float r, float g, float b) const{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}

void Shader::setVec3(const std::string &name, glm::vec3 &vector) const{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vector[0]);
    //the second parameter is how many elements (vectors in this case), will be passed, with the final being a reference to the first value of the vector array thing
}

void Shader::setVec3_v2(const std::string &name, glm::vec3 vector) const{ //this one is so you can define a vec3 in there
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vector[0]);
    //the second parameter is how many elements (vectors in this case), will be passed, with the final being a reference to the first value of the vector array thing
}

void Shader::setVec4(const std::string &name, glm::vec4 &vector) const{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &vector[0]);
    //the second parameter is how many elements (vectors in this case), will be passed, with the final being a reference to the first value of the vector array thing
}