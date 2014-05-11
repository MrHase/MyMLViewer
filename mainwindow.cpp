#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <thread>

#include "../xml-message-passing/xmplib.hpp"

#include <iostream>
using namespace std;

QStandardItem* MyMLtoModel(MyML& myml,QString name){
    QStandardItem* item=new QStandardItem(name);
    for(auto a:myml.Attributes()){
        QString k=a.first.c_str();
        QString v=a.second.c_str();
        item->appendRow(new QStandardItem(k+QString(" = ")+v));
    }
    int rows=item->rowCount();
    for(auto e:myml.Elements()){
        item->setChild(rows,MyMLtoModel(e.second,QString(e.first.c_str())));
        rows++;
    }

    return item;
}



void XMPReceiver::process(){
    xmp::XmpConnector xmp("127.0.0.1",30000);
    xmp.Register("FM","MyMLViewer");

    while(true){
        cout<<"Waiting for messages"<<endl;
        xmp.Receive();
        //cout<<"MSG: "<<xmp.GetData()<<endl;
        cout<<"dATA: "<<xmp.GetMsg()<<endl;

        MyML msg;
        XML(msg).In(xmp.GetData());
//        cout<<msg.Info()<<endl;
//        list.push_back(MyMLItem("TTTT",msg));
        emit new_msg(msg);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->listView,SIGNAL(clicked(const QModelIndex)),this,SLOT(ListItemClicked(QModelIndex)));

/*
    QStandardItemModel* listitems=new QStandardItemModel();
    for(auto i:items){
        listitems->appendRow(new QStandardItem(i.title));
    }

    ui->listView->setModel(listitems);
*/

    thread = new QThread();
    receiver = new XMPReceiver();
    receiver->moveToThread(thread);
    connect(receiver, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), receiver, SLOT(process()));
    connect(receiver,SIGNAL(new_msg(MyML)),this,SLOT(msg_received(MyML)));
    connect(receiver, SIGNAL(finished()), thread, SLOT(quit()));
    connect(receiver, SIGNAL(finished()), receiver, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    emit(RefreshItems());

}
void MainWindow::ListItemClicked(QModelIndex i){
    //cout<<""<<i.data().toString().toStdString()<<" wurde gedrückt"<<endl;
    //cout<<"Row: "<<i.row()<<endl;
    RefreshTree(i.row());
}

void MainWindow::RefreshItems()
{
    /*
    MyML myml("alter=4;name=peter;kinder;kinder.nr1;kinder.nr1.name=anna;kinder.nr2;kinder.nr2.name=klaus");
    items.push_back(MyMLItem("Eintrag1",myml));
    items.push_back(MyMLItem("Eintrag2",MyML("alter=70;name=hans;kinder;kinder.nr1;kinder.nr1.name=lara;kinder.nr2;kinder.nr2.name=klaus")));
    */

    QStandardItemModel* listitems=new QStandardItemModel();
    for(auto i:items){
        listitems->appendRow(new QStandardItem(i.title));
    }

    ui->listView->setModel(listitems);

}

void MainWindow::msg_received(MyML myml)
{
    cout<<"Msg received: "<<endl<<myml.Info()<<endl;
    QString name=myml.A("MyML.name").c_str();
    MyML data=myml.E("MyML.data");

    vector<MyMLItem> newlist;
    bool existing=false;
    for(auto i:items){
        if(i.title==name){
            newlist.push_back(MyMLItem(name,data));
            existing=true;
        }else{
            newlist.push_back(i);
        }
    }

    if(!existing){
        newlist.push_back(MyMLItem(name,data));
    }
    items=newlist;
    emit RefreshItems();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RefreshTree(int index)
{
    bool always_expand=true;
    QStandardItemModel *model2=new QStandardItemModel();
    model2->appendRow(MyMLtoModel(items[index].myml,"hallo"));
    ui->treeView->setModel(model2);
    if(always_expand){
        ui->treeView->expandAll();
    }

}
