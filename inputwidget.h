#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QLabel>
#include <QWidget>
#include "cardstructure.h"

class WordLabel : public QLabel
{
    Q_OBJECT
public:
    WordLabel(std::string txt, PhrasePair* parentPair, QWidget* parentWidget) : QLabel(parentWidget), linkedPair(parentPair)
    {
        QString label = txt.c_str();
        setText(label);
    }
    PhrasePair* const linkedPair;
    bool operator==(const WordLabel& other) {return (text() == other.text() && linkedPair == other.linkedPair); }
    bool fromSamePhrase(const WordLabel& other) {return (linkedPair == other.linkedPair); }
};

class PhraseWidget : public QWidget
{
public:
    explicit PhraseWidget(const QString& fullPhrase, PhrasePair* p, QWidget *parent = nullptr);
    virtual ~PhraseWidget() {}
    const QString content;
    PhrasePair* const linkedPair;
    WordLabel* labelWidget(std::string word);
    bool containsWord(std::string word);
    // this is pure virtual because words on the upper row connect from the bottom, lower row connect from the top
    virtual QPointF getConnectionPointFor(std::string word)=0;
private:
    std::vector<QLabel*> addedLabels;
protected:
    void dragEnterEvent(QDragEnterEvent *event) override {}
    void dragMoveEvent(QDragMoveEvent *event) override {}
    void dropEvent(QDropEvent *event) override {}
    void mousePressEvent(QMouseEvent *event) override {}
signals:

};

class NPhraseWidget : public PhraseWidget
{
public:
    NPhraseWidget(const QString& fullPhrase, PhrasePair* p, QWidget *parent = nullptr);
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QPointF getConnectionPointFor(std::string word) override;
};

class TPhraseWidget : public PhraseWidget
{
public:
    TPhraseWidget(const QString& fullPhrase, PhrasePair* p, QWidget *parent = nullptr);
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QPointF getConnectionPointFor(std::string word) override;
};

class PhrasePairWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PhrasePairWidget(PhrasePair* pair, QWidget* parent);
    PhrasePair* const linkedPair;
private:
    NPhraseWidget* nativePhrase;
    TPhraseWidget* targetPhrase;

};

namespace Ui
{
class InputForm;
};

class InputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InputWidget(QWidget *parent = nullptr);
public slots:
    void prepareEditorsFor(std::vector<PhrasePair>& pairs);


signals:

private:
    std::vector<PhrasePair> allPairs;
    Ui::InputForm* ui;
};

#endif // INPUTWIDGET_H
