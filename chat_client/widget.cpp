#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //��ʼ��socket����
    socket = new QTcpSocket();

    //���������������
    socket->connectToHost(QHostAddress(IP),PORT);

    //�����ź����
    connect(socket,&QTcpSocket::connected,this,&Widget::client_connect_success);
    connect(socket,&QTcpSocket::disconnected,this,&Widget::client_disconnect);
    connect(socket,&QTcpSocket::readyRead,this,&Widget::client_reply_info);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::client_connect_success()
{
    QMessageBox::information(this,u8"������ʾ",u8"���ӷ������ɹ�");
}

void Widget::client_disconnect()
{
    QMessageBox::information(this,u8"������ʾ",u8"�������Ͽ�����");
}


void Widget::client_reply_info()
{

}

