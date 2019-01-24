#include <headers/towerwindow.hpp>
#include <QDebug>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QTimer>
#include <QImage>
#include <stb_image.hpp>
#include <QObject>
static const char* vertexShaderSourceCode[] = {
    "\
        #version 330\n \
        layout(location = 0) in vec3 Position;\n \
        layout(location = 1) in vec2 texCoord;\n \
        out vec2 outTexCoord;\n \
        uniform mat4 worldMatrix;\n \
        void main() {\n \
            gl_Position = worldMatrix * vec4(Position,1.0f);\n \
            outTexCoord = texCoord;\n \
        }\
    "
};
static const char* fragmentShaderSourceCode[] = {
    "\
        #version 330\n \
        out vec4 fragColor;\n \
        in vec2 outTexCoord;\n \
        uniform sampler2D tex;\n \
        uniform vec3 ambientColor;\n \
        void main() {\n \
           fragColor = texture(tex,outTexCoord) * ambientColor;\n \
        }\
    "
};
TowerWindow::TowerWindow(QWidget* parent)
    : QOpenGLWidget(parent) 
    , m_vbo(0)
    ,m_rotateAngle(0)
    ,m_texture(0)
    ,m_needSetColor(false)
{
    //resize(300,300);
    setFixedSize(300,300);
    m_timer = new QTimer(this);
    QObject::connect(m_timer,SIGNAL(timeout()),this,SLOT(update()));
    m_timer->start(1000);
}
TowerWindow::~TowerWindow() {

}

void TowerWindow::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_TEXTURE_2D);
    //glClearColor(0.0f,0.0f,0.0f,1.0f);
    initBuffer();
    initProgram();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    /*glEnable(GL_CULL_FACE);
     glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);*/
}

void TowerWindow::resizeGL(int width,int height) {

}

void TowerWindow::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,width(),height());
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(const void*)(3 * sizeof(float)));
    //glBindTexture(GL_TEXTURE_2D,m_texture);
    if (m_needSetColor) {
        glUniform3fv(m_ambientColorLocation,1,&m_ambientColor[0]);
        m_needSetColor = false;
    }
    mq_texture->bind();
    glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_INT,0);
    //glDrawArrays(GL_TRIANGLES,0,6);
    setWorldMatrix();
    //setAmbientColor(m_ambientColor);
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    glFlush();
}

void TowerWindow::initBuffer() {
    float vertices[] = {
        //正面
        -0.5f,0.5f,0.5f,    0.0f,1.0f,
        -0.5f,-0.5f,0.5f,    0.0f,0.0f,
        0.5f,-0.5f,0.5f,     1.0f,0.0f,
        //底部
        -0.5f,-0.5f,-0.5f,   0.0f,1.0f,
        -0.5f,-0.5f,0.5f,    0.0f,0.0f,
        0.5f,-0.5f,0.5f,     1.0f,0.0f,
        //左侧
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
        -0.5f,-0.5f,0.5f,   1.0f,0.0f,
        -0.5f,0.5f,0.5f,    1.0f,1.0f,
        //右侧
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
        0.5f,-0.5f,0.5f,    1.0f,0.0f,
        -0.5f,0.5f,0.5f,    1.0f,1.0f
    };
    unsigned int indicates[] = {
        
        0,1,2,
        3,4,5,
        6,7,8,
        9,10,11
    };
    glGenBuffers(1,&m_vbo);
    glGenBuffers(1,&m_ibo);
   /* glGenTextures(1,&m_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_texture);
    //设置过滤方式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //设置环绕模式
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    //设置mipmap生成层范围
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,4);
    loadTextureByStb("../sky.jpg");*/
    mq_texture = new QOpenGLTexture(QImage("../floor.jpg"));
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indicates),indicates,GL_STATIC_DRAW);
}
GLuint TowerWindow::initVertexShader() {
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    assert(vertexShaderId != 0);
    GLint success;
    GLint size = strlen(vertexShaderSourceCode[0]);
    char errText[4092];
    glShaderSource(vertexShaderId,1,vertexShaderSourceCode,&size);
    glCompileShader(vertexShaderId);
    glGetShaderiv(vertexShaderId,GL_COMPILE_STATUS,&success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderId,sizeof(errText) - 1,NULL,errText);
        qDebug() << "Vertex Shader Compile Failed";
        qDebug() << tr(errText);
        exit(-1); 
    }
    return vertexShaderId;
}

GLuint TowerWindow::initFragmentShader() {
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragmentShaderId != 0);
    GLint size = strlen(fragmentShaderSourceCode[0]);
    GLint success;
    char errText[4092];
    glShaderSource(fragmentShaderId,1,fragmentShaderSourceCode,&size);
    glCompileShader(fragmentShaderId);
    glGetShaderiv(fragmentShaderId,GL_COMPILE_STATUS,&success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderId,sizeof(errText) - 1,NULL,errText);
        qDebug() << tr("Fragment Shader Compile Failed");
        qDebug() << tr(errText);
        exit(-1);
    }
    return fragmentShaderId;
}
GLuint TowerWindow::initProgram() {
    GLuint programId = glCreateProgram();
    assert(programId != 0);
    GLuint vertexShaderId = initVertexShader();
    GLuint fragmentShaderId = initFragmentShader();
    glAttachShader(programId,vertexShaderId);
    glAttachShader(programId,fragmentShaderId);
    GLint success;
    char errText[4092];
    glLinkProgram(programId);
    glGetProgramiv(programId,GL_LINK_STATUS,&success);
    if (!success) {
        glGetProgramInfoLog(programId,sizeof(errText) - 1,NULL,errText);
        qDebug() << tr("Program Link Failed");
        qDebug() << tr(errText);
        exit(-1);
    }
    glUseProgram(programId);
    m_worldMatrixLocation = glGetUniformLocation(programId,"worldMatrix");
    assert(m_worldMatrixLocation != 0xFFFFFFFF);
   // m_textureLocation = glGetUniformLocation(programId,"tex");
    //assert(m_textureLocation != 0xFFFFFFFF);
    //glUniform1i(m_textureLocation,0);
    setWorldMatrix();
    m_ambientColorLocation = glGetUniformLocation(programId,"ambientColor");
    assert(m_ambientColorLocation != 0xFFFFFFFF);
    //int ambientColor
    setAmbientColor(glm::vec3(1.0f,1.0f,1.0f));
    return programId;
}

void TowerWindow::setWorldMatrix() {
    m_rotateAngle += 1.0f;
    if (m_rotateAngle >= 360.0f) {
        m_rotateAngle = 0.0f;
    }
    //qDebug() << m_rotateAngle;
    glm::mat4 model = glm::scale(glm::mat4(1.0f),glm::vec3(1.5,1.5,1.5));
    model = glm::rotate(glm::mat4(model),glm::pi<float>() * m_rotateAngle / 180,glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 viewPort = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-3.0f));
    glm::mat4 projection = glm::perspective(glm::pi<float>() / 4,1.0f,1.0f,100.0f);
    glm::mat4 world = projection * viewPort * model;
    glUniformMatrix4fv(m_worldMatrixLocation,1,GL_FALSE,&world[0][0]);
}

void TowerWindow::loadTexture(const char* fileName) {
   QImage img;
   bool load = img.load(QString(fileName));
   assert(load);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.width(),img.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,img.mirrored().bits());
   //glGenerateMipmap(GL_TEXTURE_2D);
}

void TowerWindow::loadTextureByStb(const char* fileName) {
    int width,height,nChannels;
    unsigned char* data = stbi_load(fileName,&width,&height,&nChannels,0);
    assert(data != 0);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    stbi_image_free(data);
}

//环境光处理
void TowerWindow::setAmbientColor(const glm::vec3& color) {
    qDebug() << color[0] << " " << color[1] << " "<< color[2];
    m_ambientColor = color;
    m_needSetColor = true;
    //重新绘制
    update();
}