#include "hud/hud.h" 

GLfloat crosshairVertices[] = {
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};

void Hud::DrawCrosshair(Shader& shader) 
{
    texture.Bind(GL_TEXTURE_2D, GL_TEXTURE1);  
    shader.Activate(); 
    shader.setInt("crosshairSprite", 1); 

    glBindVertexArray(VAO); 

    renderer.draw2D(shader, glm::vec2(((float)1300 / 2) - 5.0f, ((float)1000/ 2) - 5.0f), 10.0f);    
    glBindVertexArray(0);
    texture.Unbind(); 
}

void Hud::crossHairInit(const char* textureFilePath) 
{
    texture.Init(GL_TEXTURE1); 
    texture.Bind(GL_TEXTURE_2D, GL_TEXTURE1);   
    texture.Generate(textureFilePath, 512, 512, GL_RGBA, false);     
    texture.Unbind();  
    setupMesh();   
}

void Hud::setupMesh() 
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);      

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)0);  

    glBindVertexArray(0);
}
