#ifndef CLASS_DEFINES
#define CLASS_DEFINES

#include <glm/glm.hpp>

class Shader{
    public:
        unsigned int ID;

        Shader(const char* v_file_path, const char* f_file_path);
        
        void use(); //for using the shader in the program

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMatrix4(const std::string &name, glm::mat4 matrix) const;

        /*
        When an object is defined as const in the main program, they can only call const member functions like those above.
        It is also considered good practice to make as many member functions const as possible to prevent accidental changes to objects.
        */
};

#endif