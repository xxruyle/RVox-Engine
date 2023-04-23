// stb image 
#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h" 

#include "texture/texture.h" 

Texture::Texture(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glGenTextures(1, &ID); 
}



void Texture::Bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID); 
}

void Texture::setParameters(GLint param) 
{
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::Generate(const char* file_string, int width, int height, GLenum format, bool flip) 
{
    int nrChannels; 
    stbi_set_flip_vertically_on_load(flip);  

    unsigned char* data = stbi_load(file_string, &width, &height, &nrChannels, 0);  
    if (data)
    {
        // GL_RGBA in 7th parameter if it is in png
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data); // generates textures 
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl; 
    }
 
    stbi_image_free(data); // free the image memory now that we have generated the texture 
}


void Texture::Unbind()  
{
    glBindTexture(GL_TEXTURE_2D, 0); 
}


void Texture::Delete()
{
    glDeleteTextures(1, &ID); 
}



