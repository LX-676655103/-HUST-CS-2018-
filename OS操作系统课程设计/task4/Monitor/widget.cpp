#include "widget.h"
#include "ui_widget.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

QT_CHARTS_USE_NAMESPACE
QChart *cpu_chart, *mem_chart, *swap_chart;
QLineSeries *cpu_series, *mem_series, *swap_series;


int cpu_series_Size = 120;
int timeId;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    createCanvas();
    setSysInfo();
    updateProUsg();
    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int )),this, SLOT(sortTableByCol(int)));
    timeId = startTimer(1000);
}

Widget::~Widget(){
    delete ui;
}
void Widget::createCanvas()
{

    /*创建画板*/
    cpu_chart = new QChart;
    mem_chart = new QChart;
    swap_chart = new QChart;

    /*创建曲线*/
    cpu_series = new QLineSeries;
    mem_series = new QLineSeries;
    swap_series = new QLineSeries;

    cpu_chart->addSeries(cpu_series);
    mem_chart->addSeries(mem_series);
    swap_chart->addSeries(swap_series);

    for(int i=0;i<cpu_series_Size;++i)
    {
        cpu_series->append(i,0);
        mem_series->append(i,0);
        swap_series->append(i,0);
    }

    /*创建cpu坐标轴*/
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0,cpu_series_Size);
    axisX->setTickCount(cpu_series_Size/8);
    axisX->setLabelsVisible(false);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 1);
    axisY->setTickCount(5);
    axisY->setLabelFormat("%.2f");

    cpu_chart->setAxisX(axisX,cpu_series);
    cpu_chart->setAxisY(axisY,cpu_series);
    cpu_chart->legend()->hide();

    /*创建mem坐标轴*/
    axisX = new QValueAxis;
    axisX->setRange(0,cpu_series_Size);
    axisX->setTickCount(cpu_series_Size/8);
    axisX->setLabelsVisible(false);

    axisY = new QValueAxis;
    axisY->setRange(0, 1);
    axisY->setTickCount(5);
    axisY->setLabelFormat("%.2f");

    mem_chart->setAxisX(axisX,mem_series);
    mem_chart->setAxisY(axisY,mem_series);
    mem_chart->legend()->hide();

    /*创建swap坐标轴*/
    axisX = new QValueAxis;
    axisX->setRange(0,cpu_series_Size);
    axisX->setTickCount(cpu_series_Size/8);
    axisX->setLabelsVisible(false);

    axisY = new QValueAxis;
    axisY->setRange(0, 1);
    axisY->setTickCount(5);
    axisY->setLabelFormat("%.2f");

    swap_chart->setAxisX(axisX,swap_series);
    swap_chart->setAxisY(axisY,swap_series);
    swap_chart->legend()->hide();

    /*添加到图层*/
    QChartView *chartView = new QChartView(cpu_chart);
    ui->verticalLayout->addWidget(chartView);
    chartView = new QChartView(mem_chart);
    ui->verticalLayout_2->addWidget(chartView);
    chartView = new QChartView(swap_chart);
    ui->verticalLayout_3->addWidget(chartView);
    chartView = new QChartView();
    ui->verticalLayout_4->addWidget(chartView);

}
void Widget::timerEvent(QTimerEvent *event){
    if(event->timerId()==timeId) //定时器到时间
    {
        updateCpuUsg();
        updateMemUsg();
        updateCurTime();
        updateSwapUsg();
    }
}

void Widget::setSysInfo()
{
    QFile tempFile;
    QByteArray allArray;
    QString tempStr;
    /* 设置主机名信息 */
    tempFile.setFileName("/proc/sys/kernel/hostname");
    tempFile.open(QIODevice::ReadOnly);
    allArray = tempFile.readAll();
    tempStr = QString(allArray);
    tempFile.close();
    ui->host_msglabel->setText(tempStr.mid(0,tempStr.length()-1));

    /* 读取时间信息 */
    struct sysinfo info;
    time_t cur_time=0;
    time_t boot_time=0;
    struct tm *ptm=nullptr;
    if(sysinfo(&info)) return;
    time(&cur_time);
    boot_time=cur_time-info.uptime;
    char time[30];

    /* 设置启动时间信息 */
    ptm=localtime(&boot_time);
    sprintf(time,"%d.%d.%d %02d:%02d:%02d",ptm->tm_year+1900,
            ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    ui->start_msglabel->setText(QString(time));

    /*显示系统版本号信息*/
    tempFile.setFileName("/proc/sys/kernel/osrelease");
    tempFile.open(QIODevice::ReadOnly);
    allArray = tempFile.readAll();
    tempStr = QString(allArray);
    tempFile.close();
    ui->ed_msglabel->setText(tempStr.mid(0,tempStr.length()-1));

    /* 打开cpuinfo文件 */
    tempFile.setFileName("/proc/cpuinfo");
    tempFile.open(QIODevice::ReadOnly);
    allArray = tempFile.readAll();
    tempStr = QString(allArray);
    tempFile.close();

    /* 设置cpu型号信息 */
    int from=tempStr.indexOf("model name");
    int to=tempStr.indexOf("stepping");
    ui->cpu_msglabel->setText(tempStr.mid(from+13,to-from-14));

    /*读取cpu主频信息*/
    from=tempStr.indexOf("cpu MHz");
    to=tempStr.indexOf("cache size");
    ui->cpu_fremsglabel->setText(tempStr.mid(from+11,to-from-12)+" MHz");

    /*显示个人说明*/
    ui->label_5->setText("开发者：CS1802 李响\n\nE-mail：676655103@qq.com\n");



}
void Widget::updateCurTime()
{
    /* 设置当前时间信息 */
    time_t cur_time=0;
    struct tm *ptm=nullptr;
    time(&cur_time);
    char time[30];
    ptm=localtime(&cur_time);
    sprintf(time,"%d.%d.%d %02d:%02d:%02d",ptm->tm_year+1900,
            ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    ui->label_3->setText("当前时间："+QString(time));

    /*显示运行时间信息*/
    struct sysinfo info;
    if(sysinfo(&info)) return;
    sprintf(time,"%d天%02d时%02d分%02d秒",info.uptime/3600/24,info.uptime/3600%24,info.uptime/60%60,info.uptime%60);
    ui->run_msglabel->setText(QString(time));
}
void Widget::updateCpuUsg()
{
    /* 将曲线向前移动一个单位 */
    QVector<QPointF> oldPoints = cpu_series->pointsVector();
    QVector<QPointF> points;
    for(int i=1;i<oldPoints.count();++i)
        points.append(QPointF(i-1 ,oldPoints.at(i).y()));

    /*打开/proc/stat文件*/
    QFile tempFile("/proc/stat");
    tempFile.open(QIODevice::ReadOnly);
    QByteArray allArray = tempFile.readLine();
    QString tempStr = QString(allArray);
    tempFile.close();
    QStringList strList = tempStr.split(" ");
    /*读取cpu运行信息*/
    double total_time = 0;
    double use_time = 0;
    for(int i=strList.length()-1;i>1;i--)
        total_time += strList[i].toInt();
    use_time = total_time - strList[5].toDouble();
    double usage = (use_time - old_use_time)/(total_time - old_total_time);
    old_total_time = total_time;
    old_use_time = use_time;

    points.append(QPointF(oldPoints.count()-1 ,usage));
    cpu_series->replace(points);
    ui->label->setText("CPU使用率: " + (new QString("%1"))->arg(100*usage).mid(0,4) + "%");
}
void Widget::updateMemUsg()
{
    /* 将曲线向前移动一个单位 */
    QVector<QPointF> oldPoints = mem_series->pointsVector();
    QVector<QPointF> points;
    for(int i=1;i<oldPoints.count();++i)
        points.append(QPointF(i-1 ,oldPoints.at(i).y()));

    /* 打开/proc/meminfo文件 */
    QFile tempFile("/proc/meminfo");
    tempFile.open(QIODevice::ReadOnly);
    QByteArray allArray = tempFile.readAll();
    QString tempStr = QString(allArray);
    tempFile.close();

    QStringList strList = tempStr.split("\n");
    /*读取内存运行信息*/
    int mem_total, mem_free;

    mem_total = strList[0].mid(10, strList[0].length()-13).trimmed().toInt();
    mem_free = strList[1].mid(9, strList[0].length()-12).trimmed().toInt();

    double usage = 1 - (double)mem_free/(double)mem_total;
    points.append(QPointF(oldPoints.count()-1 ,usage));
    mem_series->replace(points);
    ui->label_2->setText("内存使用率: " + (new QString("%1"))->arg(100*usage).mid(0,4) + "%");
    ui->label_10->setText("总内存：\n" + strList[0].mid(10, strList[0].length()-13).trimmed() + " KB");
    ui->label_11->setText("空闲内存：\n" + strList[1].mid(9, strList[0].length()-12).trimmed() + " KB");

}
void Widget::updateSwapUsg()
{
    /* 将曲线向前移动一个单位 */
    QVector<QPointF> oldPoints = swap_series->pointsVector();
    QVector<QPointF> points;
    for(int i=1;i<oldPoints.count();++i)
        points.append(QPointF(i-1 ,oldPoints.at(i).y()));

    /* 打开/proc/meminfo文件 */
    QFile tempFile("/proc/meminfo");
    tempFile.open(QIODevice::ReadOnly);
    QByteArray allArray = tempFile.readAll();
    QString tempStr = QString(allArray);
    tempFile.close();

    QStringList strList = tempStr.split("\n");
    /*读取内存运行信息*/
    int swap_total, swap_free;

    swap_total = strList[14].mid(10, strList[0].length()-13).trimmed().toInt();
    swap_free = strList[15].mid(9, strList[0].length()-12).trimmed().toInt();

    double usage = 1 - (double)swap_free/(double)swap_total;
    points.append(QPointF(oldPoints.count()-1 ,usage));
    swap_series->replace(points);
    ui->label_14->setText("总内存：\n" + strList[14].mid(10, strList[0].length()-13).trimmed() + " KB");
    ui->label_12->setText("空闲内存：\n" + strList[15].mid(9, strList[0].length()-12).trimmed() + " KB");

}
void Widget::updateProUsg()
{
    /*删除内容*/
    int rowNum = ui->tableWidget->rowCount();
    for(int i = 0;i < rowNum;i++)
        ui->tableWidget->removeRow(0);

    /*显示进程信息*/
    QDir qd("/proc");
    QStringList qsList = qd.entryList();
    QString tempStr;

    QString pro_id;
    QString parent_pro_id;
    QString pro_Name;
    QString pro_Mem;
    QString pro_Priority;
    QString pro_status;

    /* 设置 tableWidget 行列 */
    ui->tableWidget->setColumnCount(6);
    /* 设置 tableWidget 紧密排列 */
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    /* 隐藏 tableWidget 无网格线 */
    ui->tableWidget->setShowGrid(false);
    /* 设置 tableWidget 选择行为与模式 */
    //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止编辑
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);    //行选择模式
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);   //单选模式
    /* 设置 tableWidget 表头 */
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()
            << "PID" << "进程名" << "PPID" << "状态" << "优先级" << "占用内存");

    /* 生成 tableWidget 内容 */
    int find_begin = 2;
    int number_of_pro = 0;
    int number_of_sleep = 0;
    int number_of_run = 0;
    while(true)
    {
        /* 获取进程PID */
        pro_id = qsList[find_begin++];
        /* 进程文件读取完成 */
        int temp_num = pro_id.toInt(NULL, 10);
        if(temp_num == 0) break;

        /* 打开进程状态文件 */
        QFile tempFile("/proc/" + pro_id + "/stat");
        tempFile.open(QIODevice::ReadOnly);
        tempStr = tempFile.readAll();

        int begin_index = tempStr.indexOf("(");
        int end_index = tempStr.indexOf(")");
        pro_Name = tempStr.mid(begin_index+1, end_index-begin_index-1);
        parent_pro_id = tempStr.section(" ", 3, 3);
        pro_Priority = tempStr.section(" ", 17, 17);
        pro_Mem = tempStr.section(" ", 22, 22);
        number_of_pro++;
        pro_status=tempStr.section(' ',2,2);
        switch(pro_status.at(0).toLatin1()){
        case 'S':number_of_sleep++;break;
        case 'Z':break;
        case 'R':number_of_run++;break;
        }

        /* 增加表项 */
        ui->tableWidget->insertRow(0);
        QTableWidgetItem* pItem = new QTableWidgetItem();
        pItem->setData(Qt::EditRole, temp_num);
        ui->tableWidget->setItem(0,0,pItem);
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(pro_Name));
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(parent_pro_id));
        ui->tableWidget->setItem(0,3,new QTableWidgetItem(pro_status));
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(pro_Priority));
        ui->tableWidget->setItem(0,5,new QTableWidgetItem(pro_Mem));
        tempFile.close();
    }
    ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
    ui->label_7->setText("运行数："+(new QString())->number(number_of_run));
    ui->label_8->setText("睡眠数："+(new QString())->number(number_of_sleep));

}
void Widget::on_pushButton_4_clicked()
{
    updateProUsg();
}
void Widget::sortTableByCol(int col)
{
   updateProUsg();
   ui->tableWidget->sortItems(col, Qt::AscendingOrder);
}

void Widget::on_tableWidget_clicked(const QModelIndex &index)
{
    qDebug()<<"row"<<index.row()<<" cloumn"<<index.column();
    row = index.row();
    del_flag = true;
}

void Widget::on_pushButton_clicked()
{
    if(del_flag)
    {
        kill(ui->tableWidget->item(row, 0)->text().toInt(),SIGKILL);
        updateProUsg();
    }
    del_flag = false;
}

void Widget::on_pushButton_2_clicked()
{
    system("shutdown -s");
}

/*进程创建按钮*/
void Widget::on_pushButton_3_clicked()
{
    QString tip_message = "";
    ui->textEdit_2->setPlainText(tip_message);
    QString pathname = ui->textEdit->toPlainText();

    QStringList strList = pathname.split(" ");
    char* path[strList.length()+1];
    for(int i=0;i<strList.length();i++)
        path[i]=strList[i].toLatin1().data();
    path[strList.length()]=NULL;

    if(pid){
        ui->textEdit_2->setPlainText((tip_message+="PID Error!\n"));
        return;
    }
    if((pid = fork()) == 0){
        if(execv(path[0], &path[1])<0)
            ui->textEdit_2->setPlainText((tip_message+="Execv Error!\n"));
        exit(0);
    }
}

void Widget::on_pushButton_5_clicked()
{
    if(pid)
    {
        kill(pid,SIGKILL);
        ui->textEdit_2->setPlainText(("Kill Success!\n"));
        updateProUsg();
    }
    pid = 0;
}
