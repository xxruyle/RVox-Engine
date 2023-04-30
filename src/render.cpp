#include "render/render.h"


void Render::viewProject(Camera& camera)
{
    projection = glm::mat4(1.0f);  // projection matrix 
    view = camera.getViewMatrix();  
    projection = glm::perspective(glm::radians(camera.mFov), (float)(SCR_WIDTH/SCR_HEIGHT), 0.1f, 200.0f);
}

void Render::setShaders(Shader& shader) 
{
    shader.setMat("view", 1, GL_FALSE, view); 
    shader.setMat("projection", 1, GL_FALSE, projection);  
}

void Render::drawVoxel(Shader& shader, glm::vec3 position, float scale)  
{
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(scale, scale, scale));  
    shader.setMat("model", 1, GL_FALSE, model); 
    glDrawArrays(GL_TRIANGLES, 0, 36); 
}