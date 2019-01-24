#include <mainwidget.hpp>
#include <towerwindow.hpp>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <marcoslider.hpp>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <glm/glm.hpp>
MainWidget::MainWidget(QWidget* parent)
    :QWidget(parent)
    ,m_towerwindow(nullptr)
    ,m_mainLayout(nullptr)
    ,m_rFactor(nullptr)
{
    setWindowTitle("光照");
    m_towerwindow = new TowerWindow();
    setFixedSize(600,600);
    m_mainLayout = new QVBoxLayout();
    QHBoxLayout* towerLayout = new QHBoxLayout();
    towerLayout->addStretch();
    towerLayout->addWidget(m_towerwindow);
    towerLayout->addStretch();
    towerLayout->setContentsMargins(QMargins(0,50,0,0));
    m_mainLayout->addLayout(towerLayout);
    renderRedSlider();
    //m_mainLayout->setSpacing(50);
    setLayout(m_mainLayout);
}
MainWidget::~MainWidget() {

}

void MainWidget::renderRedSlider() {
    m_rFactor = new MarcoSlider("红色: ",Utils::SliderGrooveType::GROOVE_RED,Qt::Horizontal);
    m_gFactor = new MarcoSlider("绿色: ",Utils::SliderGrooveType::GROOVE_GREEN,Qt::Horizontal);
    m_bFactor = new MarcoSlider("蓝色: ",Utils::SliderGrooveType::GROOVE_BLUE,Qt::Horizontal);
    QHBoxLayout* layout = new QHBoxLayout();
    QVBoxLayout* sliderLayout = new QVBoxLayout();
    sliderLayout->addWidget(m_rFactor);
    sliderLayout->addWidget(m_gFactor);
    sliderLayout->addWidget(m_bFactor);
    layout->addLayout(sliderLayout);
    layout->addStretch();
    layout->setContentsMargins(QMargins(10,0,0,40));
    m_mainLayout->addLayout(layout);
    signalHandle();
}

//环境光
void MainWidget::setColor(float etc) {
    Q_UNUSED(etc);
    float r = m_rFactor->getCurrentPos();
    float g = m_gFactor->getCurrentPos();
    float b = m_bFactor->getCurrentPos();
    glm::vec3 temp = glm::vec3(r,g,b);
    if (temp != m_color)
        emit colorChanged(temp);
}
//信号处理注册
void MainWidget::signalHandle() {
    //slider    ->  this
    connect(m_rFactor,SIGNAL(posChanged(float)),this,SLOT(setColor(float)));
    connect(m_gFactor,SIGNAL(posChanged(float)),this,SLOT(setColor(float)));
    connect(m_bFactor,SIGNAL(posChanged(float)),this,SLOT(setColor(float)));
    //this      ->  towerwindow
    connect(this,SIGNAL(colorChanged(const glm::vec3&)),m_towerwindow,SLOT(setAmbientColor(const glm::vec3&)));
}