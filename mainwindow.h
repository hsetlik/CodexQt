#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "masterstackedwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CodexQt; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::CodexQt *ui;
};
#endif // MAINWINDOW_H
