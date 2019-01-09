#pragma once
unsigned int loadShaders(const char* vertex_file_path, const char* fragments_file_path){
  //the file paths have to be pointers to const char, i.e read only

  unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  //now to extract file info
  std::string vertexShaderCode;

  std::ifstream VertexShaderSteam(vertex_file_path, std::ios::in);
  //param 1 = file location
  //param 2 = for reading operations

  if(VertexShaderSteam.is_open()){
    std::stringstream sstr; //allows for stream like manipulation (useful here)
    sstr << VertexShaderSteam.rdbuf(); //the buffer is outputted to sstr
    vertexShaderCode = sstr.str(); //converts to string and stores in var
    VertexShaderSteam.close(); //closes stream
  }else{
    printf("Impossible to open %s.\n", vertex_file_path);
    getchar();
    return 0;
  }

  //same for fragment shader
  //now to extract file info
  std::string fragmentShaderCode;

  std::ifstream FragmentShaderSteam(vertex_file_path, std::ios::in);
  //param 1 = file location
  //param 2 = for reading operations

  if(FragmentShaderSteam.is_open()){
    std::stringstream sstr; //allows for stream like manipulation (useful here)
    sstr << FragmentShaderSteam.rdbuf(); //the buffer is outputted to sstr
    fragmentShaderCode = sstr.str(); //converts to string and stores in var
    FragmentShaderSteam.close(); //closes stream
  }else{
    printf("Impossible to open %s.\n", vertex_file_path);
    getchar();
    return 0;
  }


}
