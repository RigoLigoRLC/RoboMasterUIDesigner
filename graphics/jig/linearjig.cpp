#include "linearjig.h"
#include "jighandlemoveevent.h"
#include "qpen.h"

LinearJig::LinearJig(QGraphicsItem *parent) :
    ElementJig(parent)
{
    m_indicatorLine = new QGraphicsLineItem(this);
    m_indicatorLine->setPen(QPen(Qt::DashLine));
    m_handleP1 = new JigHandle(this, 1);
    m_handleP2 = new JigHandle(this, 2);
}

void LinearJig::setLine(QLine line)
{
    m_indicatorLine->setLine(line);

    invalidateHandlePositions();
}

QLine LinearJig::line()
{
    return m_indicatorLine->line().toLine();
}

void LinearJig::restore()
{
    m_indicatorLine->setLine(m_rememberedState);
    invalidateHandlePositions();

    // Forcibly loses mouse grab
    setEnabled(false);
    setEnabled(true);
}

void LinearJig::invalidateHandlePositions()
{
    m_handleP1->setCenter(m_indicatorLine->line().p1().toPoint());
    m_handleP2->setCenter(m_indicatorLine->line().p2().toPoint());
}

bool LinearJig::sceneEvent(QEvent *e)
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

void LinearJig::jigHandleMoved(int id, QPoint oldPos, QPoint newPos)
{
    QLine line = m_indicatorLine->line().toLine();
    if (id == 1) {
        line.setP1(newPos);
        m_handleP1->setCenter(newPos);
    } else if (id == 2) {
        line.setP2(newPos);
        m_handleP2->setCenter(newPos);
    }
    m_indicatorLine->setLine(line);
}
