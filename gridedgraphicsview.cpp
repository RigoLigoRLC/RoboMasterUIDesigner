#include "gridedgraphicsview.h"


GridedGraphicsView::GridedGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    m_clientBackground = QPixmap(":/res/client_background.png");

    setBackgroundBrush(QBrush(Qt::black));
}

void GridedGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    // Mostly created by ChatGPT
    QGraphicsView::drawBackground(painter, rect);

    // Paint background picture
    painter->drawPixmap({0, 0, 1920, 1080}, m_clientBackground);

    const int gridStep = 10;

    // Grid initial positions
    qreal left = int(rect.left()) - (int(rect.left()) % gridStep);
    qreal top = int(rect.top()) - (int(rect.top()) % gridStep);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridStep)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridStep)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    // Paint grid
    painter->setPen(QColor(220, 220, 220, 70));
    painter->drawLines(lines.data(), lines.size());

    // Numbers on axis
    const int labelStep = 50;
    QFont font = painter->font() ;
    font.setPointSize(8);
    painter->setFont(font);
    painter->setPen(QColor(180, 180, 180, 120));

    for (qreal x = int(left - int(left) % labelStep); x < rect.right(); x += labelStep)
        painter->drawText(QPointF(x, 0), QString::number(x));
    for (qreal y = int(top - int(top) % labelStep); y < rect.bottom(); y += labelStep)
        painter->drawText(QPointF(0, y), QString::number(y));

    // Axis
    painter->setPen(QPen(QColor(255, 150, 150, 70), 1));
    painter->drawLine(QLineF(0, 0, rect.right(), 0));
    painter->setPen(QPen(QColor(150, 255, 150, 70), 1));
    painter->drawLine(QLineF(0, 0, 0, rect.bottom()));
}
