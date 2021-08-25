#ifndef DECKWIDGET_H
#define DECKWIDGET_H

#include <QWidget>

namespace Ui {
class DeckWidget;
}

class DeckWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeckWidget(QWidget *parent = nullptr);
    ~DeckWidget();
signals:
    void goToInputScreen();


private slots:
    void on_addButton_clicked();

    void on_studyButton_clicked();

private:
    Ui::DeckWidget *ui;
};

#endif // DECKWIDGET_H
