#include <marcoslider.hpp>
#include <QLabel>
#include <QSlider>
#include <QHBoxLayout>
#include <utils.hpp>
#include <QDebug>
#include <QObject>
MarcoSlider::MarcoSlider(const char* hints,enum Utils::SliderGrooveType type,Qt::Orientation orientation,QWidget* parent) 
    :QWidget(parent)
    ,m_lower(nullptr)
    ,m_higher(nullptr)
    ,m_slider(nullptr)
    ,m_currentPos(0.0f)
{
    m_lower = new QLabel("0.0");
    m_higher = new QLabel("1.0");
    m_slider = new QSlider(orientation);
    QLabel* hintLabel = new QLabel(QString(hints));
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(hintLabel);
    layout->addWidget(m_lower);
    layout->addWidget(m_slider);
    layout->addWidget(m_higher);
    setLayout(layout);
    loadStyleSheet(type);
    //增量为0.1f
    m_slider->setMinimum(0);
    m_slider->setMaximum(1000);
    m_slider->setSingleStep(1);
    connect(m_slider,SIGNAL(valueChanged(int)),this,SLOT(sliderMoved(int)));
}
MarcoSlider::~MarcoSlider() {

}
void MarcoSlider::loadStyleSheet(enum Utils::SliderGrooveType type) {
    switch (type) {
    case Utils::SliderGrooveType::GROOVE_RED :
        setStyleSheet(Utils::readFile("../res/RedSlider.qss"));
    break;
    case Utils::SliderGrooveType::GROOVE_BLUE :
        setStyleSheet(Utils::readFile("../res/BlueSlider.qss"));
    break;
    case Utils::SliderGrooveType::GROOVE_GREEN :
        setStyleSheet(Utils::readFile("../res/GreenSlider.qss"));
    break;
    default:
    return;
    }
}

void MarcoSlider::sliderMoved(int value) {
    int minimum = m_slider->minimum();
    int maximum = m_slider->maximum();
    float pos = static_cast<float>(value) / static_cast<float>(maximum - minimum);
    if (pos != m_currentPos) {
        m_currentPos = pos;
        emit posChanged(pos);
    }
}

float MarcoSlider::getCurrentPos() const {
    return m_currentPos;
}