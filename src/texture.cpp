// stb image 
#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h" 
#include "texture/texture.h" 

Texture::Texture(GLenum textureUnit)  
{
    glActiveTexture(textureUnit);
    glGenTextures(1, &ID); 
}


void Texture::Bind(GLenum textureType, GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(textureType, ID); 
}


void Texture::GenerateCubeMap(std::vector<std::string> textures_faces, int width, int height, GLenum format, bool flip)
{
    int nrChannels; 
    stbi_set_flip_vertically_on_load(flip);  

    unsigned char* data;
    for (unsigned int i = 0; i < textures_faces.size(); i++)
    {
        data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0); 

        if (data)
        {
            // GL_RGBA in 7th parameter if it is in png
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data); // generates textures 
            // std::cout << "TEXTURE LOADED: " << textures_faces[i].c_str() << std::endl; 
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap tex failed to load at path" << std::endl; 
            stbi_image_free(data); // free the image memory now that we have generated the texture 
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
}


void Texture::Generate(const char* file_string, int width, int height, GLenum format, bool flip) 
{
    int nrChannels; 
    stbi_set_flip_vertically_on_load(flip);  

    unsigned char* data = stbi_load(file_string, &width, &height, &nrChannels, 0);  
    if (data)
    {
        // std::cout << "TEXTURE LOADED: " << file_string << std::endl; 
        // GL_RGBA in 7th parameter if it is in png
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data); // generates textures 
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl; 
    }



    stbi_image_free(data); // free the image memory now that we have generated the texture 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Texture::Unbind()  
{
    glBindTexture(GL_TEXTURE_2D, 0); 
}


void Texture::Delete()
{
    glDeleteTextures(1, &ID); 
}



