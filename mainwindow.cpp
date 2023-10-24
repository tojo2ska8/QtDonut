#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    iniciar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iniciar()
{
    QChartView *chartView = new QChartView;
    chartView->setRenderHint(QPainter::Antialiasing);
    QChart *chart = chartView->chart();
    chart->legend()->setVisible(false);

    chart->setTitle("VENTOR");
    chart->setAnimationOptions(QChart::AllAnimations);

    qreal minSize = 0.1;
    qreal maxSize = 1.0;
    int sliceCount=10;//cantidad de porciones por dona

    int totalPorciones=17; //cantidad de productos menor a 10->2 donas ,mayor a 10 ->3 donas, mayor a 20->4 donas etc. etc.
    int cantidadDeDonas=(totalPorciones / 10)+2;
    //QMessageBox::information(this,"",QString::number(cantidadDeDonas));
    int contador=0;

    int donutCount=cantidadDeDonas;// 2;//cantidad de donas



    for (int i = 0; i < donutCount; i++)
    {
        QPieSeries *donut = new QPieSeries;
        //int sliceCount =  3 + QRandomGenerator::global()->bounded(3);
        int j=0;
        while (j < sliceCount)
        {

            if(contador<totalPorciones)
            {
                qreal value = 100 + QRandomGenerator::global()->bounded(100);
                donut->setHoleSize(minSize + i * (maxSize - minSize) / donutCount);
                donut->setPieSize(minSize + (i + 0.6) * (maxSize - minSize) / donutCount);
                QPieSlice *slice;
                slice= donut->append(QString::number(value), value);
                slice->setLabelVisible();
                slice->setLabelColor(Qt::white);
                slice->setLabelPosition(QPieSlice::LabelInsideTangential);
                connect(slice, &QPieSlice::hovered, this, &MainWindow::explodeSlice);
                j++;
            }

            else
            {
                if(contador==totalPorciones)
                {
                    j=10;
                }
            }


            if(i+1==donutCount)
            {
                qreal value = 100 + QRandomGenerator::global()->bounded(100);
                donut->setHoleSize(minSize + i * (maxSize - minSize) / donutCount);
                donut->setPieSize(minSize + (i + 0.6) * (maxSize - minSize) / donutCount);
                QPieSlice *slice;
                slice= donut->append("VENTOR", value);
                slice->setLabelVisible();
                slice->setLabelColor(Qt::white);
                slice->setLabelPosition(QPieSlice::LabelInsideTangential);
                connect(slice, &QPieSlice::hovered, this, &MainWindow::explodeSlice);
                j++;
            }
            contador++;
        }

        m_donuts.append(donut);
        chartView->chart()->addSeries(donut);

    }
    ui->gridLayoutDonut->addWidget(chartView,1,1);

    updateTimer = new QTimer(this);
    updateTimer->start(1250);
}

void MainWindow::updateRotation()
{
    for (int i = 0; i < m_donuts.count(); i++)
    {
        QPieSeries *donut = m_donuts.at(i);
        qreal phaseShift =  -50 + QRandomGenerator::global()->bounded(100);
        donut->setPieStartAngle(donut->pieStartAngle() + phaseShift);
        donut->setPieEndAngle(donut->pieEndAngle() + phaseShift);
    }
}

void MainWindow::explodeSlice(bool exploded)
{
    QPieSlice *slice = qobject_cast<QPieSlice *>(sender());
    qreal sliceStartAngle = slice->startAngle();
    qreal sliceEndAngle = slice->startAngle() + slice->angleSpan();

    QPieSeries *donut = slice->series();
    qreal seriesIndex = m_donuts.indexOf(donut);
    if (exploded)
    {
        updateTimer->stop();

        for (int i = seriesIndex +1; i < m_donuts.count(); i++)
        {
            m_donuts.at(i)->setPieStartAngle(sliceEndAngle);
            m_donuts.at(i)->setPieEndAngle(350 + sliceStartAngle);
            slice->setLabel("name:"+QString::number(slice->value()));
            slice->setLabelColor(Qt::black);
            slice->setLabelPosition(QPieSlice::LabelOutside);
        }
    }
    else
    {
        for (int i = 0; i < m_donuts.count(); i++)
        {
            m_donuts.at(i)->setPieStartAngle(0);
            m_donuts.at(i)->setPieEndAngle(360);
            QString label=slice->label();
            if(label!="VENTOR")
            {
                label=QString::number(slice->value());
                slice->setLabel(label);
                slice->setLabelColor(Qt::white);
                slice->setLabelPosition(QPieSlice::LabelInsideTangential);
            }
        }
        updateTimer->start();
    }
    slice->setExploded(exploded);
}

