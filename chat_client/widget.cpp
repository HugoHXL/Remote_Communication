#include "widget.h"
#include "ui_widget.h"
#include"chatlist.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //��ʼ��socket����
    socket = new QTcpSocket();

    //��ʼ���ı������
    ui->passWordEdit->setEchoMode(QLineEdit::Password);

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
    QByteArray ba;
    client_recv_data(ba);

    QJsonObject obj = QJsonDocument::fromJson(ba).object();

    QString cmd = obj.value("cmd").toString();

    if(cmd=="register_reply")
    {
        client_register_handler(obj);
    }
    else if (cmd == "login_reply")
    {
        client_login_handler(obj);
    }
}


void Widget::on_registerButton_clicked()
{
    QString username = ui->userEdit->text();
    QString password = ui->passWordEdit->text();

    QJsonObject obj;
    obj.insert("cmd","register");
    obj.insert("username",username);
    obj.insert("password",password);

    client_send_data(obj);
}

void Widget::client_send_data(QJsonObject &obj)
{
    QByteArray ba = QJsonDocument(obj).toJson();

    int size = ba.size();
    QByteArray sendData;
    sendData.insert(0,(char*)&size,4);
    sendData.append(ba);

    socket->write(sendData);
}

void Widget::client_recv_data(QByteArray &ba)
{
    char buf[1024] = {0};
    int size,sum=0;
    bool flag = true;

    socket->read(buf,4);
    memcpy(&size,buf,4);

    qDebug()<<"get data len : "<<size;

    while(flag)
    {
        memset(buf,0,1024);
        sum+=socket->read(buf,size-sum);
        if(sum>=size)
        {
            flag = false;
        }
        ba.append(buf);
    }
    qDebug()<<"data : "<<ba;

}

void Widget::client_register_handler(QJsonObject &obj)
{
    QString result = obj.value("result").toString();

    if(result=="user_exist")
    {
        QMessageBox::warning(this,u8"ע����ʾ",u8"�û��Ѵ���");
    }
    else if(result == "success")
    {
        QMessageBox::warning(this,u8"ע����ʾ",u8"ע��ɹ�");
    }
}

void Widget::client_login_handler(QJsonObject &obj)
{
    QString result = obj.value("result").toString();

    if(result == "not_exist")
    {
        QMessageBox::warning(this,u8"��¼��ʾ",u8"�û�������");
    }
    else if(result == "password_error")
    {
        QMessageBox::warning(this,u8"��¼��ʾ",u8"�������");
    }
    else if(result == "success")
    {
        //��ת�½���
        QString friendlist = obj.value("friendlist").toString();
        QString grouplist = obj.value("grouplist").toString();
        QString username = ui->userEdit->text();

        this->hide();
        socket->disconnect(SIGNAL(readyRead()));
        socket->disconnect(SIGNAL(disconnected()));

        Chatlist* c = new Chatlist(socket,friendlist,grouplist,username);//�ڶѿռ�����
        c->setWindowTitle(username);
        c->show();
    }
}


void Widget::on_loginButton_clicked()
{
    QString username = ui->userEdit->text();
    QString password = ui->passWordEdit->text();

    QJsonObject obj;
    obj.insert("cmd","login");
    obj.insert("username",username);
    obj.insert("password",password);

    client_send_data(obj);
}

