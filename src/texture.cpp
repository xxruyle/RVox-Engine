// stb image 
#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h" 

#include "texture/texture.h" 

Texture::Texture(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glGenTextures(1, &ID); 
}



void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, ID); 
}

void Texture::setParameters() 
{
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::Generate(const char* file_string, int width, int height) 
{
    int nrChannels; 
    stbi_set_flip_vertically_on_load(true);  
    unsigned char* data = stbi_load(file_string, &width, &height, &nrChannels, 0);  
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // generates textures 
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl; 
    }
 
    stbi_image_free(data); // free the image memory now that we have generated the texture 
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID); 
}



