#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "../fm/myml.h"
#include <thread>
#include <QThread>


namespace Ui {
class MainWindow;
}


class XMPReceiver : public QObject {
    Q_OBJECT

public:
    XMPReceiver(){}
    ~XMPReceiver(){}

public slots:
    void process();

signals:
    void new_msg(MyML myml);
    void finished();
    void error(QString err);

private:
    // add your variables here
};
class MyMLItem{
public:
    MyMLItem(QString title, MyML myml){
        this->title=title;
        this->myml=myml;
    }

    QString title;
    MyML myml;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void ListItemClicked(QModelIndex i);
public slots:
    void RefreshItems();
    void msg_received(MyML myml);
public:
    explicit MainWindow(QWidget *parent = 0);


    ~MainWindow();

private:
    XMPReceiver* receiver;
    QThread* thread;
    void RefreshTree(int index);
    Ui::MainWindow *ui;
    vector<MyMLItem> items;




};

#endif // MAINWINDOW_H
