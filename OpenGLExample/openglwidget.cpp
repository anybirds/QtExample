#include <GL/glew.h>

#include "openglwidget.h"

#include <fstream>
#include <sstream>
#include <string>

#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget() {

}

void OpenGLWidget::initializeGL() {
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
    {
        qDebug() << "GLEW Error: " << glewGetErrorString(glew_error);
        exit(1);
    }

    GLint framebuffer;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &framebuffer);
    qDebug() << framebuffer;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);

    // create triangle vbo, vao
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 36, tri, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, nullptr);
    glEnableVertexAttribArray(0);

    // complie shader
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    std::ifstream fpv("C:\\Users\\river\\Documents\\QTProject\\OpenGLExample\\vert.glsl");
    std::stringstream bufv;
    bufv << fpv.rdbuf();
    std::string sourcev = bufv.str();
    const char *sourcev_cstr = sourcev.c_str();
    glShaderSource(vert, 1, &sourcev_cstr, NULL);
    glCompileShader(vert);
    GLint status = 0;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* infoLog = (GLchar*)malloc(sizeof(GLchar) * (infoLogLength + 1));
        glGetShaderInfoLog(vert, infoLogLength, NULL, infoLog);
        qDebug() << "Shader could not compile: " << infoLog;
        throw std::exception();

        free(infoLog);
    } else {
        qDebug() << "everything fine";
    }

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream fpf("C:\\Users\\river\\Documents\\QTProject\\OpenGLExample\\frag.glsl");
    std::stringstream buff;
    buff << fpf.rdbuf();
    std::string sourcef = buff.str();
    const char *sourcef_cstr = sourcef.c_str();
    glShaderSource(frag, 1, &sourcef_cstr, NULL);
    glCompileShader(frag);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* infoLog = (GLchar*)malloc(sizeof(GLchar) * (infoLogLength + 1));
        glGetShaderInfoLog(frag, infoLogLength, NULL, infoLog);
        qDebug() << "Shader could not compile: " << infoLog;
        throw std::exception();

        free(infoLog);
    } else {
        qDebug() << "everything fine";
    }
    program = glCreateProgram();
    glUseProgram(program);
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    GLint GLerror;
    glGetProgramiv(program, GL_LINK_STATUS, &GLerror);
    if (GLerror == GL_FALSE)
    {
        qDebug() << "Error linking program";
    } else {
        qDebug() << "everything fine";
    }
}

void OpenGLWidget::paintGL() {
    // bind vao, program
    // draw call
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glUseProgram(program);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    update();
}

void OpenGLWidget::resizeGL(int w, int h) {

}


