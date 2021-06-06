#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QApplication>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#define BUFFER_SIZE 1024


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButton_3->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    int from_fd,to_fd;
    int bytes_read,bytes_write;
    char buffer[BUFFER_SIZE];

    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_2->setEnabled(false);

    QString from_name = ui->textEdit->toPlainText();
    QString to_name = ui->textEdit_2->toPlainText();
    QString tip_message = "";

    ui->textEdit_3->setPlainText(tip_message);
    if((from_fd=open(from_name.toLatin1().data(),O_RDONLY))==-1){
        ui->textEdit_3->setPlainText((tip_message+="From File Open Error!\n"));
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        return;
    }
    else ui->textEdit_3->setPlainText((tip_message+="From File Open Success!\n"));
    if((to_fd=open(to_name.toLatin1().data(),O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1){
        ui->textEdit_3->setPlainText((tip_message+="To File Open Error!\n"));
        ::close(from_fd);
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        return;
    }
    else ui->textEdit_3->setPlainText((tip_message+="To File Open Success!\n"));

    int file_size = lseek(from_fd, 0, SEEK_END);
    int read_byte = 0; cancel_flag = false;
    lseek(from_fd, 0, SEEK_SET);

    /* 拷贝文件 */
    while(bytes_read=read(from_fd,buffer,BUFFER_SIZE))
    {
        if(cancel_flag) break;
        if((bytes_read==-1)&&(errno!=EINTR)) break;
        else if(bytes_read>0)
        {
            char *ptr=buffer;
            read_byte += bytes_read;
            while(bytes_write=write(to_fd,ptr,bytes_read))
            {
                if((bytes_write==-1)&&(errno!=EINTR)) break;
                else if(bytes_write==bytes_read) break;
                else if(bytes_write>0){
                    ptr+=bytes_write;
                    bytes_read-=bytes_write;
                }
            }
            if(bytes_write==-1) break;
        }
        ui->progressBar->setValue(read_byte/(file_size/100));
    }
    ::close(from_fd);
    ::close(to_fd);

    printf("%d %d\n",bytes_read,bytes_write);
    if(cancel_flag) ui->textEdit_3->setPlainText((tip_message+="Copy Cancel!\n"));
    else if(bytes_read==-1||bytes_write==-1)
        ui->textEdit_3->setPlainText((tip_message+="Copy Error!\n"));
    else ui->textEdit_3->setPlainText((tip_message+="Copy Success!\n"));
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(false);
}

void Widget::on_pushButton_2_clicked()
{
    exit(0);
}

void Widget::on_pushButton_3_clicked()
{
    cancel_flag = true;
    ui->pushButton_3->setEnabled(false);
    ui->pushButton->setEnabled(true);
}
