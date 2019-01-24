#ifndef __TOWER_WINDOW_HPP__
#define __TOWER_WINDOW_HPP__
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <glm/glm.hpp>
class QTimer;
class TowerWindow : public QOpenGLWidget,protected QOpenGLFunctions {
   Q_OBJECT
public:
    explicit TowerWindow(QWidget* parent = nullptr);
    ~TowerWindow();
    virtual void initializeGL();
    virtual void resizeGL(int width,int height);
    virtual void paintGL();
public slots:
   void setAmbientColor(const glm::vec3& ambientColor);
private:
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_worldMatrixLocation;
    GLuint m_texture;
    GLuint m_textureLocation;
    GLuint m_ambientColorLocation;
    bool m_needSetColor;
    float m_rotateAngle;
    QTimer* m_timer;
    QOpenGLTexture* mq_texture;
    void initBuffer();
    GLuint initVertexShader();
    GLuint initFragmentShader();
    GLuint initProgram();
    void setWorldMatrix();
    void loadTexture(const char* fileName);
    void loadTextureByStb(const char* fileName);
    glm::vec3 m_ambientColor;
};
#endif /*__TOWER_WINDOW_HPP__*/