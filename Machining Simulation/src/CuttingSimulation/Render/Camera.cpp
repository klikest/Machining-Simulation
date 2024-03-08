#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init(GLFWwindow* window, glm::vec3 campos_, float yaw_, float pitch_)
{
    cameraPos = campos_;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;

    yaw = yaw_;
    pitch = pitch_;
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);


    pMat = glm::perspective(1.0472f, aspect, 0.1f, 10000.0f); // 1.0472 radians == 60 degrees
    vMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    lastX = (GLfloat)(width / 2.0);
    lastY = (GLfloat)(height / 2.0);
}


void Camera::MoveCamera(GLFWwindow* window, GLfloat deltaTime)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Camera controls
    GLfloat cameraSpeed = cam_speed * deltaTime;;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {


    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (firstMouse)
        {
            lastX = (GLfloat)xpos;
            lastY = (GLfloat)ypos;
            firstMouse = false;
        }

        GLfloat xoffset = (GLfloat)(xpos - lastX);
        GLfloat yoffset = (GLfloat)(lastY - ypos); // Reversed since y-coordinates go from bottom to left
        lastX = (GLfloat)xpos;
        lastY = (GLfloat)ypos;

        GLfloat sensitivity = 0.05f;	// Change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);

    }

    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstMouse = true;
    }

    vMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


}


void Camera::SetCamMatrixToShader(GLuint ShaderProg)
{
    glUseProgram(ShaderProg);

    unsigned int viewLoc = glGetUniformLocation(ShaderProg, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(vMat));

    unsigned int projLoc = glGetUniformLocation(ShaderProg, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
}


void Camera::SetCamPosToShader(GLuint ShaderProg)
{
    glUseProgram(ShaderProg);
    glUniform3fv(glGetUniformLocation(ShaderProg, "cam_pos"), 1, glm::value_ptr(cameraPos) );
}



void Camera::UpdateAspectRate(float aspect)
{
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 10000.0f);
}


glm::mat4 Camera::GetViewMatrix()
{
    return vMat;
}

glm::mat4 Camera::GetProjMatrix()
{
    return pMat;
}