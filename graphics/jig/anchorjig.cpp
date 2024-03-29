#include "anchorjig.h"
#include "jighandlemoveevent.h"
#include "qpen.h"

AnchorJig::AnchorJig(QGraphicsItem *parent) :
    ElementJig(parent)
{
    m_boundingRect = new QGraphicsRectItem(this);
    m_boundingRect->setPen(QPen(Qt::DashLine));
    m_handle = JigHandle::Make(this);
}

void AnchorJig::setRect(QRect rect)
{
    m_boundingRect->setRect(rect);

    invalidateHandlePositions();
}

QPoint AnchorJig::anchor()
{
    return m_handle->center();
}

void AnchorJig::restore()
{
    m_boundingRect->setRect(m_rememberedRect);
    invalidateHandlePositions();

    // Forcibly loses mouse grab
    setEnabled(false);
    setEnabled(true);
}

void AnchorJig::invalidateHandlePositions()
{
    m_handle->setCenter(m_boundingRect->rect().topLeft().toPoint());
}

bool AnchorJig::sceneEvent(QEvent *e)
{
    switch (int(e->type())) {
    case JigHandleMoveEvent::type: {
        auto me = (JigHandleMoveEvent*)e;
        jigHandleMoved(me->handle()->id(), me->oldPos(), me->newPos());
        return ElementJig::sceneEvent(e);
    }
    default:
        return ElementJig::sceneEvent(e);
    }
}

void AnchorJig::jigHandleMoved(int id, QPoint oldPos, QPoint newPos)
{
    auto rect = m_boundingRect->rect();
    rect.moveTopLeft(newPos);
    m_boundingRect->setRect(rect);
    invalidateHandlePositions();
}
