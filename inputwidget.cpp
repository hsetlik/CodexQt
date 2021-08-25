#include "inputwidget.h"
#include "ui_inputform.h"

PhraseWidget::PhraseWidget(const QString& fullPhrase, PhrasePair* p, QWidget *parent) :
    QWidget(parent),
    linkedPair(p)
{
    auto sPhrase = fullPhrase.toStdString();
    auto exp = std::regex("\\w+");
    auto words = stdu::matchesAsVector(sPhrase, exp);
    int x = 5;
    int y = 5;
    for(auto& word : words)
    {
        auto label = new WordLabel(word, linkedPair, this);
        addedLabels.push_back(label);
        label->move(x, y);
        label->show();
        label->setAttribute(Qt::WA_DeleteOnClose);

        x += label->width() + 2;
    }
    setAcceptDrops(true);
}

WordLabel* PhraseWidget::labelWidget(std::string inputWord)
{
    for(auto w : addedLabels)
    {
        auto word = dynamic_cast<WordLabel*>(w);
        if(word->text().toStdString() == inputWord)
            return word;

    }
    return nullptr;
}
bool PhraseWidget::containsWord(std::string input)
{
    for(auto w : addedLabels)
    {
        auto word = dynamic_cast<WordLabel*>(w);
        if(word->text().toStdString() == input)
            return true;

    }
    return false;
}
void PhraseWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("word")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}
void PhraseWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("word")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}
//==========================================================================================
NPhraseWidget::NPhraseWidget(const QString& fullPhrase, PhrasePair* p, QWidget *parent) :
    PhraseWidget(fullPhrase, p, parent)
{

}

void NPhraseWidget::mousePressEvent(QMouseEvent *event)
{
    WordLabel* child = dynamic_cast<WordLabel*>(childAt(event->pos()));
    if(child == nullptr)
        return;
    if(event->button() == Qt::RightButton)
    {
        auto nativeWord = child->text().toStdString();
        linkedPair->removeNtaPairByNative(nativeWord);
    }
    else
    {
        auto clickSpot = event->pos() - child->pos();

        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << child->text() << QPoint(clickSpot);

        QMimeData *mimeData = new QMimeData;
        mimeData->setData("word", itemData);
        mimeData->setText(child->text());

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(child->pixmap(Qt::ReturnByValue));
        drag->setHotSpot(clickSpot);
        drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
    }

}
void NPhraseWidget::dropEvent(QDropEvent *event)
{
    WordLabel* child = dynamic_cast<WordLabel*>(childAt(event->pos()));
    if(child == nullptr)
        return;

    auto sNative = child->text().toStdString();
    auto sTarget = event->mimeData()->text().toStdString();

    printf("Adding target: %s, native: %s\n", sTarget.c_str(), sNative.c_str());
    linkedPair->addNtaPair(sNative, sTarget);
}
QPointF NPhraseWidget::getConnectionPointFor(std::string word)
{
    auto label = labelWidget(word);
    float x0 = (float)label->x() + label->width() / 2;
    float y0 = (float)label->y() + label->height() * 1.05f;
    return QPointF(x0, y0) + QPointF(pos());
}
//==========================================================================================
TPhraseWidget::TPhraseWidget(const QString& fullPhrase, PhrasePair* p, QWidget *parent) :
    PhraseWidget(fullPhrase, p, parent)
{

}
void TPhraseWidget::mousePressEvent(QMouseEvent *event)
{
    WordLabel* child = dynamic_cast<WordLabel*>(childAt(event->pos()));
    if(child == nullptr)
        return;
    if(event->button() == Qt::RightButton)
    {
        auto nativeWord = child->text().toStdString();
        linkedPair->removeNtaPairByNative(nativeWord);
    }
    else
    {
        auto clickSpot = event->pos() - child->pos();

        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << child->text() << QPoint(clickSpot);

        QMimeData *mimeData = new QMimeData;
        mimeData->setData("word", itemData);
        mimeData->setText(child->text());

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(child->pixmap(Qt::ReturnByValue));
        drag->setHotSpot(clickSpot);
        drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
    }
}
void TPhraseWidget::dropEvent(QDropEvent *event)
{
    WordLabel* child = dynamic_cast<WordLabel*>(childAt(event->pos()));
    if(child == nullptr)
        return;
    auto sTarget = child->text().toStdString();
    auto sNative = event->mimeData()->text().toStdString();
    printf("Adding target: %s, native: %s\n", sTarget.c_str(), sNative.c_str());
    linkedPair->addNtaPair(sNative, sTarget);
}
QPointF TPhraseWidget::getConnectionPointFor(std::string word)
{
    auto label = labelWidget(word);
    float x0 = (float)label->x() + label->width() / 2;
    float y0 = (float)label->y() - label->height() * 0.05f;
    return QPointF(x0, y0) + QPointF(pos());
}
//==========================================================================================
PhrasePairWidget::PhrasePairWidget(PhrasePair* pair, QWidget* parent) :
    QWidget(parent),
    linkedPair(pair),
    nativePhrase(nullptr),
    targetPhrase(nullptr)
{
    pair->addListener(this);
    QString nativeQ = linkedPair->nativePhrase.fullPhrase.c_str();
    QString targetQ = linkedPair->targetPhrase.fullPhrase.c_str();
    nativePhrase = new NPhraseWidget(nativeQ, linkedPair, this);
    targetPhrase = new TPhraseWidget(targetQ, linkedPair, this);
    auto layout = new QVBoxLayout;
    layout->addWidget(nativePhrase);
    layout->addWidget(targetPhrase);
    setLayout(layout);
}

void PhrasePairWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    QPen pen(Qt::blue, 1.8f, Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin);
    painter.setPen(pen);
    auto& pairs = linkedPair->ntaPairs;
    for(auto& p : pairs)
    {
         auto p1 = nativePhrase->getConnectionPointFor(p.first);
         auto p2 = targetPhrase->getConnectionPointFor(p.second);
         auto centerY = std::min(p1.y(), p2.y()) + (std::abs(p1.y() - p2.y()) / 2);
         auto centerX = std::min(p1.x(), p2.x()) + (std::abs(p1.x() - p2.x()) / 2.0f);
         auto c1 = QPointF(p1.x(), centerY);
         auto c2 = QPointF(p2.x(), centerY);
         QPainterPath path;
         path.moveTo(p1);
         path.quadTo(c1, QPointF(centerX, centerY));
         path.quadTo(c2, p2);
         painter.drawPath(path);
    }
}


//==========================================================================================

InputWidget::InputWidget(QWidget *parent) :
    QWidget(parent),
    currentPhrasePair(nullptr),
    pairIndex(0),
    ui(new Ui::InputForm)
{
    ui->setupUi(this);
    pairIndex = -1;
    advancePhrasePair();
}

void InputWidget::advancePhrasePair()
{
     ++pairIndex;
    if(pairIndex >= (int)allPairs.size())
        return;
    ui->verticalLayout->removeWidget(&*currentPhrasePair);
    printf("Displaying Pair Number %d\n", pairIndex);
    auto& pairToAdd = allPairs[pairIndex];
    currentPhrasePair.reset(new PhrasePairWidget(&pairToAdd, this));
    ui->verticalLayout->addWidget(&*currentPhrasePair);
}
void InputWidget::prepareEditorsFor(std::vector<PhrasePair>& pairs)
{
    allPairs.clear();
    for(auto& pair : pairs)
        allPairs.push_back(pair);
    printf("Preparing editors. . .\n");
    pairIndex = -1;
    advancePhrasePair();
}
void InputWidget::on_prevButton_clicked()
{
    if(pairIndex < 1)
        return;
    --pairIndex;
    ui->verticalLayout->removeWidget(&*currentPhrasePair);
    printf("Displaying Pair Number %d\n", pairIndex);
    auto& pairToAdd = allPairs[pairIndex];
    currentPhrasePair.reset(new PhrasePairWidget(&pairToAdd, this));
    ui->verticalLayout->addWidget(&*currentPhrasePair);
}
void InputWidget::on_nextButton_clicked()
{
    advancePhrasePair();
}
