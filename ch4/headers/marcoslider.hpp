#ifndef __MARCO_SLIDER_HPP__
#define __MARCO_SLIDER_HPP__
#include <QWidget>
#include <utils.hpp>
class QLabel;
class QSlider;
class MarcoSlider : public QWidget {
    Q_OBJECT
public:
    explicit MarcoSlider(const char* hints,enum Utils::SliderGrooveType type = Utils::SliderGrooveType::GROOVE_RED,Qt::Orientation orientation = Qt::Vertical,QWidget* parent = 0);
    ~MarcoSlider();
    float getCurrentPos() const;
public slots:
    void sliderMoved(int value);
signals:
    void posChanged(float pos);
private:
    QLabel* m_lower;
    QLabel* m_higher;
    QSlider* m_slider;
    float m_currentPos;
    void loadStyleSheet(enum Utils::SliderGrooveType type);
};
#endif /*__MARCO_SLIDER_HPP__*/