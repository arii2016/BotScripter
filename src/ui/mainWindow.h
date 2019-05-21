#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "data/dataManager.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    int dataInitialize();

public slots:
    void findScript();
    void loadScript();
    void saveScript();

    void updateKeyword();

    void parsing();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    DataManager* m_dataMgr;

};

#endif // MAINWINDOW_H
