#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QChartView>
#include <QVBoxLayout>
#include <QLineSeries>
#include <QValueAxis>
#include <QTimer>
#include <QTime>
#include <QList>
#include <QPointF>
#include <QDebug>
#include "qmath.h"
#include <sys/sysinfo.h>
#include <QDir>
#include <QStringList>
#include <QListWidgetItem>
#include <signal.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    int pid = 0;
    int old_total_time=0;
    int old_use_time=0;
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private slots:
    void on_pushButton_4_clicked();
    void sortTableByCol(int col);
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    int row;
    bool del_flag = false;
    Ui::Widget *ui;
    void createCanvas();
    void setSysInfo();
    void updateCurTime();
    void updateCpuUsg();
    void updateMemUsg();
    void updateSwapUsg();
    void updateProUsg();
};

#endif // WIDGET_H
