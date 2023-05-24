#include "render/render.h"

void Render::viewProject(Camera& camera)
{
    projection = glm::mat4(1.0f);  // projection matrix 
    view = camera.getViewMatrix();  
    projection = camera.getProjectionMatrix();
}

void Render::viewOrtho(OrthoCamera& orthoCamera) 
{
    projection = glm::mat4(1.0f);  // projection matrix 
    view = orthoCamera.getViewMatrix();  
    projection = orthoCamera.getOrthoMatrix();  
}

void Render::setShaders(Shader& shader) 
{
    shader.setMat("view", 1, GL_FALSE, view); 
    shader.setMat("projection", 1, GL_FALSE, projection);  
}

void Render::drawVoxelCubeMap(Shader& shader, Texture& texture, glm::vec3 position, float scale)  
{ // render a voxel to the screen in 3D 
    // texture.Bind(GL_TEXTURE_CUBE_MAP, GL_TEXTURE0); // binding the given texture so that we can draw other voxels 

    // setting up the model matrix 
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(scale, scale, scale));  
    shader.setMat("model", 1, GL_FALSE, model); 

    // rendering the triangles for the cube  
    glDrawArrays(GL_TRIANGLES, 0, 36); 
    texture.Unbind(); 
}


void Render::drawVoxel(Shader& shader, glm::vec3 position, glm::vec3 color, float scale)
{ // TO DO: make a frustum voxel in frustum function 
    if (frustum.pointInFrustum(position)) // if point is in frustum 
    { 
        model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(scale, scale, scale));  
        shader.setMat("model", 1, GL_FALSE, model); 
        shader.setVec3("voxelColor", color.x, color.y, color.z);

        // rendering the triangles for the cube  
        glDrawArrays(GL_TRIANGLES, 0, 36); 
    }
     /* else { // for debugging purposes, shows culled voxels as red 
        model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(scale, scale, scale));  
        shader.setMat("model", 1, GL_FALSE, model); 
        shader.setVec3("voxelColor", 1.0, 0.0, 0.0); 

        // rendering the triangles for the cube  
        glDrawArrays(GL_TRIANGLES, 0, 36); 
    } */
}



void Render::drawRotatingVoxel(Shader& shader, glm::vec3 position, float scale, float rotation, glm::vec3 axisRotations)
{ // allow use of drawing a voxel but have the ability to rotate 
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(scale, scale, scale));  
    model = glm::rotate(model, rotation, axisRotations); 
    shader.setMat("model", 1, GL_FALSE, model); 
    glDrawArrays(GL_TRIANGLES, 0, 36); 
}

void Render::draw2D(Shader& shader, glm::vec2 position, float scale)
{ // render 2D hud like images to screen 

    projection = glm::mat4(1.0f); 
    projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);  
    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model, glm::vec3(position, 0.0f)); 
    model = glm::scale(model, glm::vec3(scale, scale, scale));

    shader.setMat("model", 1, GL_FALSE, model); 
    shader.setMat("projection", 1, GL_FALSE, projection);  

    glDrawArrays(GL_TRIANGLES, 0, 6); 
}