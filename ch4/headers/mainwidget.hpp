#ifndef __MAIN_WIDGET_HPP__
#define __MAIN_WIDGET_HPP__
#include <QWidget>
#include <glm/glm.hpp>
class TowerWindow;
class QVBoxLayout;
class MarcoSlider;
class MainWidget : public QWidget {
    Q_OBJECT
public:
    explicit MainWidget(QWidget* parent = nullptr);
    ~MainWidget();
public slots:
    void setColor(float etc);
signals:
    void colorChanged(const glm::vec3& color);
public:
    void signalHandle();
private:
    TowerWindow* m_towerwindow;
    QVBoxLayout* m_mainLayout;
    MarcoSlider* m_rFactor;
    MarcoSlider* m_gFactor;
    MarcoSlider* m_bFactor;
    glm::vec3 m_color;
    void renderRedSlider();
};
#endif  /*__MAIN_WIDGET_HPP__*/