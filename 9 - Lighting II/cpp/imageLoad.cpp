#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <iostream>
#include "../header/functionDefines.h"
#include "../external/stb_image.h"

unsigned int loadTexture(const char* file){
    unsigned int texture; //the actual texture id
    glGenTextures(1, &texture); //first param is how many to make, second param is a single int or array to store the generated texture IDs in

    stbi_set_flip_vertically_on_load(true); //flips the image on load

    int width, height, nrChannels; //nrChannels 
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    //stores the width and height in those vars
    //8-bit components per pixel is stored in nrChannels
    //for final parameter, you can replace '0' with '1' or more to force that many bits per pixel

    if(data){ //data will be more than 0 if it loaded anything, and more than 0 is not false, so the next piece of code happens
        glBindTexture(GL_TEXTURE_2D, texture); //sets as the currently active texture object, so subsequent texture operations are done on this

        //basically checks the channels and sets the respective format, using an enum
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 2)
            format = GL_RG;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        //1st param is the type, 2nd is the mipmap level if you want to do it manually, but we don't so set it to 0, 3rd param is what kind of format to store 
        //the image as, we'll store as RGB so GL_RGB, 4th and 5th are the width and height, 6th is the border, which is a legacy parameter, so set it to 0,
        //the 7th value is what formate we loaded the image as, it will probably be RGB so set it as GL_RGB, and 8th is what we stored the image as, it was stored
        //as chars (bytes), so we set this as GL_UNSIGNED_BYTE, and then pass the actual data (a pointer to it anyway)
        glGenerateMipmap(GL_TEXTURE_2D); //generates mipmaps for it

        //for texture s,t,r is like x,y,z (you can have 3D textures)
        //so the below 2 texture wrap options are essentially in the x and y axis
        //as you may expect, GL_REPEAT just repeats it as expected, as normal
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //GL_TEXTURE_MIN_FILTER is how to sample when minifying a texture, GL_TEXTURE_MAG_FILTER is how to sample when magnifying a texture
        //GL_LINEAR gets the average colour from a pixel and surrounding pixels so looks smoother, GL_NEAREST is precisely the colour of that pixel, so very sharp
        //and as such GL_NEAREST is possibly the effect you want in something like an 8-bit game
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0); //safely unbinds texture

    stbi_image_free(data); //free the image memory once we're done with it

    return texture;
}

#endif