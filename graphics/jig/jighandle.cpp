#include "jighandle.h"
#include "jighandlemoveevent.h"
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QStyleHints>
#include <QGraphicsScene>

QColor JigHandle::HandleColor = QColor(Qt::cyan);

JigHandle::JigHandle(QGraphicsItem *parent) :
    QGraphicsRectItem(QRect(-HandleWidth / 2, -HandleWidth / 2, HandleWidth, HandleWidth), parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setBrush(HandleColor);

    m_moveConfirmed = false;

}

JigHandle::JigHandle(QRect rect, QGraphicsItem *parent) :
    QGraphicsRectItem(rect, parent)
{

}

void JigHandle::setCenter(QPoint p)
{
    auto r = rect();
    r.moveCenter(p);
    setRect(r);
}

void JigHandle::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsRectItem::mousePressEvent(e);
    m_scenePressPos = e->scenePos();
    m_beginDragRectCenter = rect().center();
}

void JigHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    JigHandleMoveEvent::NewEditingState state = JigHandleMoveEvent::DoingEdit;

    if (!m_moveConfirmed) {
        if (QLineF(e->scenePos(), m_scenePressPos).length() > qApp->styleHints()->startDragDistance()) {
            m_moveConfirmed = true;
            state = JigHandleMoveEvent::BeginEdit;
        } else {
            return;
        }
    }

    scene()->sendEvent(parentItem(),
                       new JigHandleMoveEvent(data(HandleIndex).toInt(),
                                              m_beginDragRectCenter.toPoint(),
                                              e->scenePos().toPoint(),
                                              state)
                       );
}

void JigHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    // This can be false if the scene interrupted editing
    if (!m_moveConfirmed)
        return;

    m_moveConfirmed = false;

    scene()->sendEvent(parentItem(),
                       new JigHandleMoveEvent(data(HandleIndex).toInt(),
                                              m_beginDragRectCenter.toPoint(),
                                              e->scenePos().toPoint(),
                                              JigHandleMoveEvent::CommitEdit)
                       );
}
