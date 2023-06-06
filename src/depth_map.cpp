#include "world/depth_map.h" 

DepthMap::DepthMap() 
{
    glGenFramebuffers(1, &depthMapFBO); 

    glGenTextures(1, &depthMap); 
    glBindTexture(GL_TEXTURE_2D, depthMap); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  


    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO); 
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0); 
    glDrawBuffer(GL_NONE); 
    glReadBuffer(GL_NONE); 
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void DepthMap::Render(Shader& shader, glm::vec3 lightPos, glm::vec3 target)     
{
    shader.Activate();
    ConfigureShaderAndMatrices(shader, lightPos, target);  

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT); 
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO); 
    glClear(GL_DEPTH_BUFFER_BIT);     
}


void DepthMap::ConfigureShaderAndMatrices(Shader& shader, glm::vec3 lightPos, glm::vec3 target)
{
    glm::mat4 lightProjection = glm::ortho(-64.0f, 64.0f, -64.0f, 64.0f, nearPlane, farPlane);       

    glm::mat4 lightView = glm::lookAt(lightPos,     
                                  target, 
                                  glm::vec3( 0.0f, 1.0f,  0.0f)); 

    lightSpaceMatrix = lightProjection * lightView;  
    shader.setMat("lightSpaceMatrix", 1, GL_FALSE, lightSpaceMatrix);       
}

void DepthMap::bindTexture()
{
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, depthMap);
}


void DepthMap::DebugRender(Shader& shader)   
{
    shader.Activate(); 

    shader.setInt("depthMap", 0);      
    shader.setFloat("nearD", nearPlane); 
    shader.setFloat("farD", farPlane); 

    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DepthMap::Unbind()  
{
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl; 

    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}