#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include "jighandlemoveevent.h"
#include "rectangularjig.h"

RectangularJig::RectangularJig(QGraphicsItem* parent) :
    ElementJig(parent)
{
    // Create boundary representation
    m_boundary = new QGraphicsRectItem(QRectF(0, 0, 100, 100), this); // Sane default
    m_boundary->setPen(QPen(Qt::DashLine));
    m_boundary->setBrush(Qt::NoBrush);

    // Create all 8 handles
    // Counted from top-left handle, rotating clockwise, indexed from 0~7
    for (auto i = 0; i < 8; i++) {
        auto handle = new JigHandle(this, i);
        m_handles.append(handle);
    }
    m_centerHandle = new JigHandle(this);
    m_centerHandle->setData(JigHandle::HandleIndex, 8);

    // Set handles' positions
    invalidateHandlePositions();
}

RectangularJig::~RectangularJig()
{
    // Destroy all handles
    foreach(auto i, m_handles) {
        if (i)
            delete i;
    }
    m_handles.clear();
}

QRect RectangularJig::jigRect()
{
    return m_boundary->rect().toAlignedRect();
}

void RectangularJig::setJigRect(QRect rect)
{
    m_boundary->setRect(rect);
    invalidateHandlePositions();
}

void RectangularJig::remember()
{
    m_rememberedBoundary = m_boundary->rect();
}

void RectangularJig::restore()
{
    m_boundary->setRect(m_rememberedBoundary);
    invalidateHandlePositions();

    // Forcibly loses mouse grab
    setEnabled(false);
    setEnabled(true);
}

void RectangularJig::forget()
{
    m_rememberedBoundary = m_boundary->rect().normalized();
    restore();
}

void RectangularJig::jigHandleMoved(int id, QPointF oldPos, QPointF newPos)
{
    auto ret = newPos;

    switch (id) {
    case 1:
    case 5: ret = { oldPos.x(), newPos.y() }; break;
    case 3:
    case 7: ret = { newPos.x(), oldPos.y() }; break;
    }

    setPointAtIndex(id, ret.toPoint());
}

QPoint RectangularJig::pointAtIndex(int i)
{
    auto r = m_boundary->rect();

    switch (i) {
    case 0: return r.topLeft().toPoint();
    case 1: return { (int) r.center().x(), (int) r.top() };
    case 2: return r.topRight().toPoint();
    case 3: return { (int) r.right(), (int) r.center().y() };
    case 4: return r.bottomRight().toPoint();
    case 5: return { (int) r.center().x(), (int) r.bottom() };
    case 6: return r.bottomLeft().toPoint();
    case 7: return { (int) r.left(), (int) r.center().y() };
    default: return {};
    }
}

void RectangularJig::setPointAtIndex(int i, QPoint p)
{
    auto r = m_boundary->rect();

    switch(i) {
    case 0:
        r.setTopLeft(p); m_boundary->setRect(r);
        break;
    case 1:
        r.setTop(p.y()); m_boundary->setRect(r);
        break;
    case 2:
        r.setTopRight(p); m_boundary->setRect(r);
        break;
    case 3:
        r.setRight(p.x()); m_boundary->setRect(r);
        break;
    case 4:
        r.setBottomRight(p); m_boundary->setRect(r);
        break;
    case 5:
        r.setBottom(p.y()); m_boundary->setRect(r);
        break;
    case 6:
        r.setBottomLeft(p); m_boundary->setRect(r);
        break;
    case 7:
        r.setLeft(p.x()); m_boundary->setRect(r);
        break;
    case 8:
        r.moveCenter(p); m_boundary->setRect(r);
        break;
    default: return;
    }

    invalidateHandlePositions();
}

QPoint RectangularJig::handleCenter(int i)
{
    if (i > m_handles.size()) return {};

    return m_handles[i]->rect().center().toPoint();
}

void RectangularJig::setHandleCenter(int i, QPoint p)
{
    if (i > m_handles.size()) return;

    m_handles[i]->setCenter(p);
}

void RectangularJig::invalidateHandlePositions()
{
    for (int i = 0; i < 8; i++) {
        setHandleCenter(i, pointAtIndex(i));
    }
    m_centerHandle->setCenter(m_boundary->rect().center().toPoint());
}

void RectangularJig::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    e->ignore();
}

bool RectangularJig::sceneEvent(QEvent *e)
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
