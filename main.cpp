#include "torneo.cpp"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <QBarSet>
#include <QBarSeries>
QT_CHARTS_USE_NAMESPACE
using namespace std;
int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    //Clase para graficar
    srand(time(NULL));
    int gn=0;
    cout<<"Ingresa el numero de generaciones: ";
    cin>>gn;
    pTorneo b(16,4,gn,2,5);  //(int miembros,int genes,int generaciones,int pcruza,int mutar)
    for(int i=0;i<b.generaciones;i++){
        b.creaIndividuos();
        b.calcAptitud();

        b.calcProbabilidad();
        if(i==0 || i==(b.generaciones-1))
            b.Tabla1(i+1);
        b.barajeo();
        if(i==0 || i==(b.generaciones-1))
            b.Tabla2(i+1);
        b.cruza();
        if(i==0 || i==(b.generaciones-1))
            b.Tabla3(i+1);
        b.Tabla4(i+1);
        b.maxmin();
        b.reasignar();
    }

    cout<<"Archivos generados correctamente con los nombres: Tabla1.txt, Tabla2.txt, Tabla3.txt y Tabla4.txt";
    QLineSeries *series = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();
    cout<<endl<<"Maximos"<<endl<<endl;
    for(int i=1;i<=b.generaciones;i++){
        cout<<b.getMayores(i-1)<<endl;
        series->append(i,b.getMayores(i-1));
        series2->append(i,b.getMenores(i-1));
    }
    cout<<endl<<"Minimos"<<endl<<endl;
    for(int i=0;i<b.generaciones;i++)
        cout<<b.getMenores(i)<<endl;

    //Para mostrar los datos necesitamos una instancia de QChart
    QChart *chart = new QChart();
    QChartView *chartView = new QChartView(chart);
    chart->setTitle("Práctica 4: Selección por torneo");

    chart->legend()->show(); //muestra la linea del color cuando ponemos show para saber cual se está referenciando

    series->setName("Máximos");
    series2->setName("Mínimos");

    chartView->chart()->addSeries(series);
    chartView->chart()->addSeries(series2);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(1, b.generaciones);
    axisX->setTickCount(10);
    chartView->chart()->setAxisX(axisX, series);

    QValueAxis *axisX2 = new QValueAxis;
    axisX2->setRange(1, b.generaciones);
    axisX2->setTickCount(10);
    chartView->chart()->setAxisX(axisX2, series2);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 1);
    axisY->setTickCount(20);
    chartView->chart()->setAxisY(axisY, series);



    QValueAxis *axisY2 = new QValueAxis;
    axisY2->setRange(0, 1);
    axisY2->setTickCount(20);
    chartView->chart()->setAxisY(axisY2,series2);

    //Agregamos a un chartview la instancia de qchart

    chartView->setRenderHint(QPainter::Antialiasing);
    //Para mostrarlo en ventana
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(600, 400);
    window.show();
    return a.exec();
}
