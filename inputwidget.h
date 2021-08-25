#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QLabel>
#include <QWidget>
#include "cardstructure.h"

class WordLabel : public QLabel
{
    Q_OBJECT
public:
    WordLabel(std::string txt, PhrasePair* parentPair, QWidget* parentWidget) :
        QLabel(parentWidget),
        linkedPair(parentPair),
        backgroundColor(QColor(80, 80, 80)),
        hoverBackground(QColor(100, 100, 100)),
        mouseIsOver(false)
    {
        QString label = txt.c_str();
        setFrameStyle(QFrame::Panel);
        setText(label);
    }
    PhrasePair* const linkedPair;
    bool operator==(const WordLabel& other) {return (text() == other.text() && linkedPair == other.linkedPair); }
    bool fromSamePhrase(const WordLabel& other) {return (linkedPair == other.linkedPair); }
    void paintEvent(QPaintEvent*) override;
    void setBackgroundColor(QColor newColor) {backgroundColor = newColor; }
private:
    void enterEvent(QEvent*) override {mouseIsOver = true; repaint(); }
    void leaveEvent(QEvent*) override {mouseIsOver = false; repaint(); }
    QColor backgroundColor;
    QColor hoverBackground;
    bool mouseIsOver;
};
//==========================================================================================
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
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
signals:

};
//==========================================================================================
//Subclasses for native & target phrases. Constructors don't change, only the 3 virtual functions are different
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
    void mouseDoubleClickEvent(QMouseEvent*) override;
    QPointF getConnectionPointFor(std::string word) override;
};
//==========================================================================================
class PhrasePairWidget :
        public QWidget,
        public PhrasePairListener
{
    Q_OBJECT
public:
    explicit PhrasePairWidget(PhrasePair* pair, QWidget* parent);
    void phraseChanged(PhrasePair*) override {repaint(); }
    void paintEvent(QPaintEvent* event) override;
    PhrasePair* const linkedPair;
private:
    NPhraseWidget* nativePhrase;
    TPhraseWidget* targetPhrase;
};
//==========================================================================================
namespace Ui
{
class InputForm;
};

class InputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InputWidget(QWidget *parent = nullptr);
    void advancePhrasePair();
public slots:
    void prepareEditorsFor(std::vector<PhrasePair>& pairs);


signals:

private slots:
    void on_prevButton_clicked();

    void on_nextButton_clicked();

    void on_fullBox_stateChanged(int arg1);

private:
    std::vector<PhrasePair> allPairs;
    std::unique_ptr<PhrasePairWidget> currentPhrasePair;
    int pairIndex;
    int totalCardsAdded;
    Ui::InputForm* ui;
};

#endif // INPUTWIDGET_H
