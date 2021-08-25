#ifndef MASTERSTACKEDWIDGET_H
#define MASTERSTACKEDWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include "inputwidget.h"
#include "phraseinputform.h"
/*
 * Pages and indeces:
 * 0: phrase input
 * 1: card setup
 */

class MasterStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MasterStackedWidget(QWidget *parent = nullptr);
private slots:
    void switchToCardEditors();


signals:

};

#endif // MASTERSTACKEDWIDGET_H
