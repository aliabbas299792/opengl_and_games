#ifndef CLASS_DEFINES
#define CLASS_DEFINES

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Shader{
    public:
        unsigned int ID;

        Shader(const char* v_file_path, const char* f_file_path);
        
        void use(); //for using the shader in the program

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMatrix4(const std::string &name, glm::mat4 &matrix) const;
        void set3Float(const std::string &name, float r, float g, float b) const;
        void setVec3(const std::string &name, glm::vec3 &vector) const;
        void setVec3_v2(const std::string &name, glm::vec3 vector) const;
        void setVec4(const std::string &name, glm::vec4 &vector) const;

        /*
        When an object is defined as const in the main program, they can only call const member functions like those above.
        It is also considered good practice to make as many member functions const as possible to prevent accidental changes to objects.
        */
};

class Camera{
    private:
        int windowWidth = 0;
        int windowHeight = 0;

        float lastX = windowWidth/2;
        float lastY = windowHeight/2;
        float yaw = 0;
        float pitch = 0;
    public:
        GLFWwindow* window = NULL;

        float fov = 45.0f;

        glm::mat4 projection = glm::mat4(1.0f);

        glm::mat4 view = glm::mat4(1.0f);
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

        Camera(int width, int height, float fov);
        void keyboard_movement();
        void scroll_callback_zoom(double xOffset, double yOffset);
        void mouse_callback(double xPos, double yPos);
        void liveUpdate();
};

#endif