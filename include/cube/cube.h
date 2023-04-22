#pragma once 
#include <vector> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Cube
{
    public: 
        std::vector<glm::vec3> positions; 
        void generatePlane(); 

}; 