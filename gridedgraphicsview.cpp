#include "gridedgraphicsview.h"
#include <QMouseEvent>
#include <QScrollBar>
#include <QStyleHints>
#include <QApplication>

GridedGraphicsView::GridedGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    m_clientBackground = QPixmap(":/res/client_background.png");

    setBackgroundBrush(QBrush(Qt::black));
}

void GridedGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    // Initially created by ChatGPT
    QGraphicsView::drawBackground(painter, rect);

    // Paint background picture
    // painter->drawPixmap({0, 0, 1920, 1080}, m_clientBackground);

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
    painter->setPen(QColor(220, 220, 220, 30));
    painter->drawLines(lines.data(), lines.size());

    // Numbers on axis
    const int labelStep = 50;
    qreal bottom = int(rect.bottom()) -  (int(rect.bottom()) % gridStep);
    QFont font = painter->font() ;
    font.setPointSize(8);
    painter->setFont(font);
    painter->setPen(QColor(220, 220, 220, 120));

    qreal labelTop = (rect.top() < 0 ? 0 : rect.top()) + 15;
    qreal labelLeft = (rect.left() < 0 ? 0 : rect.left()) + 5;

    for (qreal x = int(left - int(left) % labelStep); x < rect.right(); x += labelStep)
        painter->drawText(QPointF(x, labelTop), QString::number(x));
    for (qreal y = int(bottom - int(bottom) % labelStep - (labelStep - 1080 % labelStep)); y > rect.top(); y -= labelStep)
        painter->drawText(QPointF(labelLeft, y), QString::number(1080 - y));

    // Axis
    painter->setPen(QPen(QColor(255, 150, 150, 70), 1));
    painter->drawLine(QLineF(0, 0, rect.right(), 0));
    painter->setPen(QPen(QColor(150, 255, 150, 70), 1));
    painter->drawLine(QLineF(0, 0, 0, rect.bottom()));
}

void GridedGraphicsView::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton) {
        m_backgroundDragging = DraggingPrepare;
        m_backgroundDraggingMouseStartPos = e->globalPos();
        m_backgroundDraggingViewStartPos = {horizontalScrollBar()->value(), verticalScrollBar()->value()};
    } else if (m_backgroundDragging != DraggingConfirmed) {
        QGraphicsView::mousePressEvent(e);
    }
}

void GridedGraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    if (!e->buttons().testFlag(Qt::RightButton)) {
        m_backgroundDragging = DraggingNone;
    }
    QGraphicsView::mouseReleaseEvent(e);
}

void GridedGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    if (m_backgroundDragging != DraggingNone) {
        auto delta = e->globalPos() - m_backgroundDraggingMouseStartPos;

        if (m_backgroundDragging == DraggingPrepare &&
            QLineF({}, delta).length() > qApp->styleHints()->startDragDistance()) {
            m_backgroundDragging = DraggingConfirmed;
        }

        if (m_backgroundDragging == DraggingConfirmed) {
            auto newViewPos = m_backgroundDraggingViewStartPos - delta;
            horizontalScrollBar()->setValue(newViewPos.x());
            verticalScrollBar()->setValue(newViewPos.y());
        }
    }
    QGraphicsView::mouseMoveEvent(e);
}

void GridedGraphicsView::resizeEvent(QResizeEvent *e)
{
    if (m_alwaysZoomToFit) {
        zoomToFit();
    } else {
        QGraphicsView::resizeEvent(e);
    }
}

void GridedGraphicsView::zoomToFit()
{
    fitInView({0, 0, 1920, 1080}, Qt::KeepAspectRatio);
}
