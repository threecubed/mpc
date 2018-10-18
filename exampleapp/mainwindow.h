#ifndef MPCEXAMPLEAPP_MAINWINDOW_H
#define MPCEXAMPLEAPP_MAINWINDOW_H

// c++ standard
//#include <memory>

#include <QMainWindow>
#include <QSharedPointer>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void OnHomePage();
    void OnMixingLawsView();

private:
    QSharedPointer<QStackedWidget> stacked_widget;
    QSharedPointer<QStatusBar> status_bar;
};


#endif //MPCEXAMPLEAPP_MAINWINDOW_H
