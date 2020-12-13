#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <list>
#include <map>
#include <random>
#include <ctime>
#include <cmath>

#include <Shader.h>

#include <SOIL2/SOIL2.h>

const int pointsCount = 50;

GLfloat firstWall[] =
{
    -2.0f, -1.5f, -2.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -2.0f, 1.5f, -2.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    2.0f, 1.5f, -2.0f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    2.0f, -1.5f, -2.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f
};

GLfloat secondWall[] =
{
    2.0f, 1.5f, -2.0f,      -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    2.0f, -1.5f, -2.0f,     -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    2.0f, -1.5f, 2.0f,      -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    2.0f, 1.5f, 2.0f,       -1.0f, 0.0f, 0.0f,   1.0f, 1.0f
};

GLfloat thirdWall[] =
{
    -2.0f, 1.5f, 2.0f,      0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
    -2.0f, -1.5f, 2.0f,     0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
    2.0f, -1.5f, 2.0f,      0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
    2.0f, 1.5f, 2.0f,       0.0f, 0.0f, -1.0f,   0.0f, 1.0f
};

GLfloat fourthWall[] =
{
    -2.0f, -1.5f, 2.0f,      1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -2.0f, 1.5f, 2.0f,       1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    -2.0f, 1.5f, -2.0f,      1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    -2.0f, -1.5f, -2.0f,     1.0f, 0.0f, 0.0f,   1.0f, 0.0f
};

GLfloat flour[] =
{
    -2.0f, -1.5f, 2.0f,      0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    -2.0f, -1.5f, -2.0f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    2.0f, -1.5f, -2.0f,      0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    2.0f, -1.5f, 2.0f,       0.0f, 1.0f, 0.0f,   1.0f, 0.0f
};

GLfloat ceiling[] =
{
    -2.0f, 1.5f, -2.0f,      0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
    2.0f, 1.5f, -2.0f,       0.0f, -1.0f, 0.0f,    1.0f, 1.0f,
    2.0f, 1.5f, 2.0f,        0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
    -2.0f, 1.5f, 2.0f,       0.0f, -1.0f, 0.0f,    0.0f, 0.0f
};

GLfloat BSpline[] =
{
    0.0f, 0.0f, 0.0f,
    -0.1f, 0.4f, 0.0f,
    -0.1f, 0.4f, 0.0f,
    -0.1f, 0.4f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.1f, 0.6f, 0.0f,
    -0.1f, 0.6f, 0.0f,
    -0.1f, 0.6f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.1f, 0.6f, 0.0f,
    0.1f, 0.6f, 0.0f,
    0.1f, 0.6f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.1f, 0.4f, 0.0f,
    0.1f, 0.4f, 0.0f,
    0.1f, 0.4f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f
};

GLfloat nodes[] =
{
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f / 21.0f, 2.0f / 21.0f, 3.0f / 21.0f, 4.0f / 21.0f, 5.0f / 21.0f, 6.0f / 21.0f, 7.0f / 21.0f,
    8.0f / 21.0f, 9.0f / 21.0f, 10.0f / 21.0f, 11.0f / 21.0f, 12.0f / 21.0f, 13.0f / 21.0f, 14.0f / 21.0f, 15.0f / 21.0f,
    16.0f / 21.0f, 17.0f / 21.0f, 18.0f / 21.0f, 19.0f / 21.0f, 20.0f / 21.0f, 1.0f, 1.0f, 1.0f, 1.0f
};

GLfloat wall[] =
{
    0.5f, -0.5f, 10.0f,    1.0f, 1.0f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    0.5f, 0.5f, 10.0f,    1.0f, 0.0f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    -0.5f, 0.5f, 10.0f,   0.0f, 0.0f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 10.0f,   0.0f, 1.0f,    0.0f, 0.0f, -1.0f,    1.0f, 0.0f, 0.0f,    0.0f, -1.0f, 0.0f
};

GLfloat points[60 * pointsCount];
GLfloat normals[60 * pointsCount];

const float pi = 3.1415926535;

bool firstMouse = true;
GLfloat lastX;
GLfloat lastY;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

glm::mat4x4 mvp;
glm::mat4x4 proj;
glm::mat4x4 view;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -11.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

std::vector<Shader> shaders;
std::vector<GLuint> vboIds;
std::vector<GLuint> vaoIds;
std::vector<GLuint> texs;

int id;

float calcBSpline(float param, int currentExp, int number)
{
    if (currentExp == 0)
    {
        if (param < nodes[number + 1] && param >= nodes[number])
        {
            return 1.0f;
        }
        else
        {
            return 0.0f;
        }
    }
    else
    {
        if (nodes[number + currentExp] == nodes[number] && nodes[number + currentExp + 1] == nodes[number + 1])
        {
            return 0;
        }
        else if (nodes[number + currentExp + 1] == nodes[number + 1])
        {
            return (param - nodes[number]) * calcBSpline(param, currentExp - 1, number) / (nodes[number + currentExp] - nodes[number]);
        }
        else if (nodes[number + currentExp] == nodes[number])
        {
            return (nodes[number + currentExp + 1] - param) * calcBSpline(param, currentExp - 1, number + 1) / (nodes[number + currentExp + 1] - nodes[number + 1]);
        }
        else
        {
            return (param - nodes[number]) * calcBSpline(param, currentExp - 1, number) / (nodes[number + currentExp] - nodes[number]) +
                (nodes[number + currentExp + 1] - param) * calcBSpline(param, currentExp - 1, number + 1) / (nodes[number + currentExp + 1] - nodes[number + 1]);
        }
    }
}

void createArrays(void)
{
    GLuint vertexBuffer, arrayBuffer;

    //Points vertex array #0
    glGenBuffers(1, &vertexBuffer);
    glGenVertexArrays(1, &arrayBuffer);
    glBindVertexArray(arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);

    vaoIds.push_back(arrayBuffer);

    //Wall vertex array #1
    glGenBuffers(1, &vertexBuffer);
    glGenVertexArrays(1, &arrayBuffer);
    glBindVertexArray(arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall), wall, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);

    vaoIds.push_back(arrayBuffer);
}

void createShaders(void)
{
    Shader roomShader("Shaders/roomVertShader.vert", "Shaders/roomFragShader.frag");
    shaders.push_back(roomShader);
}

void createTextures(void)
{
    GLuint tex;
    int width, height;
    unsigned char* image;

    //Diffuse texture #0
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image = SOIL_load_image("Textures/diffuse.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

    texs.push_back(tex);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Normal texture #1
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image = SOIL_load_image("Textures/normal.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

    texs.push_back(tex);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Depth texture #2
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image = SOIL_load_image("Textures/depth.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

    texs.push_back(tex);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void createBSpline(void)
{
    for (int i = 0; i < pointsCount; ++i)
    {
        int i2 = (i != pointsCount - 1) ? (i + 1) : (0) ;
        for (int k = 0; k < 10; ++k)
        {
            glm::vec3 res = glm::vec3(0.0f, 0.0f, 0.0f);
            float p = 1;
            float x1, y1, z1, x2, y2, z2;
            x1 = y1 = z1 = x2 = y2 = z2 = 0;
            for (int j = 0; j < 24; ++j)
            {
                float t = i;
                float t2 = i2;
                float p = calcBSpline(t / pointsCount, 3, j);
                x1 += BSpline[3 * j] * p;
                y1 += BSpline[3 * j + 1] * p;
                z1 += BSpline[3 * j + 2] * p;
                p = calcBSpline(t2 / pointsCount, 3, j);
                x2 += BSpline[3 * j] * p;
                y2 += BSpline[3 * j + 1] * p;
                z2 += BSpline[3 * j + 2] * p;
            }
            points[60 * i + 6 * k] = x1;
            points[60 * i + 6 * k + 1] = y1;
            points[60 * i + 6 * k + 2] = z1 + 0.1f * k;
            points[60 * i + 6 * k + 3] = x2;
            points[60 * i + 6 * k + 4] = y2;
            points[60 * i + 6 * k + 5] = z2 + 0.1f * k;
            for (int m = 0; m < 2; ++m)
            {
                GLfloat tempX, tempY, tempZ;
                if (m == 0)
                {
                    tempX = points[60 * i + 6 * k];
                    tempY = points[60 * i + 6 * k + 1];
                    tempZ = points[60 * i + 6 * k + 2];
                }
                else
                {
                    tempX = points[60 * i + 6 * k + 3];
                    tempY = points[60 * i + 6 * k + 4];
                    tempZ = points[60 * i + 6 * k + 5];
                }
                if (tempX > 0.0f)
                {
                    if (tempY > 0.5f)
                    {
                        if (tempY >= 0.6f)
                        {
                            normals[60 * i + 6 * k + 3 * m] = 0.4f;
                            normals[60 * i + 6 * k + 4 * m] = 0.1f;
                            normals[60 * i + 6 * k + 5 * m] = 0.0f;
                        }
                        else
                        {
                            normals[60 * i + 6 * k + 3 * m] = 0.1f;
                            normals[60 * i + 6 * k + 4 * m] = 0.4f;
                            normals[60 * i + 6 * k + 5 * m] = 0.0f;
                        }
                    }
                    else
                    {
                        if (tempY <= 0.4f)
                        {
                            normals[60 * i + 6 * k + 3 * m] = 0.4f;
                            normals[60 * i + 6 * k + 4 * m] = -0.1f;
                            normals[60 * i + 6 * k + 5 * m] = 0.0f;
                        }
                        else
                        {
                            normals[60 * i + 6 * k + 3 * m] = 0.1f;
                            normals[60 * i + 6 * k + 4 * m] = -0.4f;
                            normals[60 * i + 6 * k + 5 * m] = 0.0f;
                        }
                    }
                }
                else
                {
                    if (tempY > 0.5f)
                    {
                        if (tempY >= 0.6f)
                        {
                            normals[60 * i + 6 * k + 3 * m] = -0.4f;
                            normals[60 * i + 6 * k + 4 * m] = 0.1f;
                            normals[60 * i + 6 * k + 5 * m] = 0.0f;
                        }
                        else
                        {
                            normals[60 * i + 6 * k + 3 * m] = -0.1f;
                            normals[60 * i + 6 * k + 4 * m] = 0.4f;
                            normals[60 * i + 6 * k + 5 * m] = 0.0f;
                        }
                    }
                    else
                    {
                        if (tempY > 0.4f)
                        {
                            normals[60 * i + 6 * k + 3 * m] = -0.1f;
                            normals[60 * i + 6 * k + 4 * m] = -0.4f;
                            normals[60 * i + 6 * k + 5 * m] = 0.0f;
                        }
                        else
                        {
                            normals[60 * i + 6 * k + 3 * m] = 0.4f;
                            normals[60 * i + 6 * k + 4 * m] = -0.1f;
                            normals[60 * i + 6 * k + 5 * m] = 0.0f;
                        }
                    }
                }
            }
        }
    }
}

void reshape(int width, int height)
{
    proj = glm::perspectiveFovRH(55.0f, 1024.0f, 600.0f, 0.1f, 20.0f);
    glViewport(0, 0, width, height);
}

void display(void)
{
    glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    mvp = proj * view;
    glBindVertexArray(vaoIds[0]);
    glUniformMatrix4fv(glGetUniformLocation(shaders[0].Program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
    glUniform3fv(glGetUniformLocation(shaders[0].Program, "cameraPos"), 1, glm::value_ptr(cameraPos));
    id = 0;
    glUniform1i(glGetUniformLocation(shaders[0].Program, "id"), id);
    shaders[0].Use();
    glDrawArrays(GL_QUAD_STRIP, 0, 60 * pointsCount);
    glBindVertexArray(vaoIds[1]);
    id = 1;
    shaders[0].Use();
    glUniform1i(glGetUniformLocation(shaders[0].Program, "id"), id);
    glUniform1i(glGetUniformLocation(shaders[0].Program, "diffuseTex"), 0);
    glUniform1i(glGetUniformLocation(shaders[0].Program, "normalTex"), 1);
    glUniform1i(glGetUniformLocation(shaders[0].Program, "depthTex"), 2);
    glUniform3fv(glGetUniformLocation(shaders[0].Program, "cameraPos"), 1, glm::value_ptr(cameraPos));
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texs[0]);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, texs[1]);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, texs[2]);

    const float speed = 2 * pi / 10000;
    int time = glutGet(GLUT_ELAPSED_TIME) % 10000;
    float angle = time * speed;
    glm::mat4x4 model = glm::mat4x4(cos(angle), -sin(angle), 0.0f, 0.0f, sin(angle), cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    mvp = proj * view * model;
    glUniformMatrix4fv(glGetUniformLocation(shaders[0].Program, "mvp"), 1, GL_FALSE, &mvp[0][0]);

    glDrawArrays(GL_QUADS, 0, 4);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glutSwapBuffers();
    glFinish();
}

void idle(void)
{
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    GLfloat cameraSpeed = 0.1f;
    if (key == 'w' || key == 'W')
    {
        cameraPos += cameraSpeed * cameraFront;
    }
    else if (key == 's' || key == 'S')
    {
        cameraPos -= cameraSpeed * cameraFront;
    }
    else if (key == 'a' || key == 'A')
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    else if (key == 'd' || key == 'D')
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    glutPostRedisplay();
}

void mouse(int x, int y)
{
    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    GLfloat xoffset = x - lastX;
    GLfloat yoffset = lastY - y;
    lastX = x;
    lastY = y;

    GLfloat sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
    if (x < 10 or (glutGet(GLUT_WINDOW_WIDTH) - x) < 10)
    {
        lastX = glutGet(GLUT_WINDOW_WIDTH) / 2;
        lastY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
        glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Scene");
    glewInit();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glViewport(0, 0, 1024, 600);
    createBSpline();
    createShaders();
    createTextures();
    createArrays();

    glutPassiveMotionFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}