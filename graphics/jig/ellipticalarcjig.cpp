#include "ellipticalarcjig.h"
#include "jighandle.h"
#include "jighandlemoveevent.h"
#include <QEvent>
#include <QtMath>
#include <QPen>
#include <cmath>

EllipticalArcJig::EllipticalArcJig(QGraphicsItem* parent) :
    ElementJig(parent)
{
    // Default position: X = 100, Y = 50, Begin = 1/3pi, End = 5/3pi
    m_halfWidth = 50;
    m_halfHeight = 100;
    m_angleBegin = (0.3) * M_PI;
    m_angleEnd = (1.7) * M_PI;

    m_handleBegin = JigHandle::Make(this, 0);
    m_handleEnd = JigHandle::Make(this, 1);
    m_handleCenter = JigHandle::Make(this, 2);

    m_resultArc = new QGraphicsEllipseItem(this);

    QPen p;
    p.setStyle(Qt::DashLine);
    m_resultArc->setPen(p);

    invalidateGraphics();
}

void EllipticalArcJig::setRect(QRect rect)
{
    m_halfWidth = rect.width() / 2;
    m_halfHeight = rect.height() / 2;
    m_handleCenter->setCenter(rect.center());
    invalidateGraphics();
}

void EllipticalArcJig::setBeginEndAngles(double begin, double end)
{
    m_angleBegin = begin;
    m_angleEnd = end;

    invalidateGraphics();
}

std::tuple<int, int> EllipticalArcJig::qtArcStartSpanAngles()
{
    return { m_resultArc->startAngle(), m_resultArc->spanAngle() };
}

std::tuple<double, double> EllipticalArcJig::rmArcBeginEndAnglesRadian()
{
    return { m_angleBegin, m_angleEnd };
}

void EllipticalArcJig::remember()
{
    m_rememberedState = {
                         m_angleBegin, m_angleEnd, m_halfWidth, m_halfHeight, m_handleCenter->center()
    };
}

void EllipticalArcJig::restore()
{
    m_angleBegin = m_rememberedState.angleBegin;
    m_angleEnd = m_rememberedState.angleEnd;
    m_halfWidth = m_rememberedState.halfWidth;
    m_halfHeight = m_rememberedState.halfHeight;
    m_handleCenter->setCenter(m_rememberedState.center);

    invalidateGraphics();

    setEnabled(false);
    setEnabled(true);
}

void EllipticalArcJig::forget()
{
    remember();
}

bool EllipticalArcJig::sceneEvent(QEvent *e)
{
    switch(int(e->type())) {
    case JigHandleMoveEvent::type: {
        auto me = (JigHandleMoveEvent*)e;
        handleMoved(me->handle()->id(), me->oldPos(), me->newPos());
        return ElementJig::sceneEvent(e);
    }
    default:
        return ElementJig::sceneEvent(e);
    }
}

void EllipticalArcJig::invalidateGraphics()
{
    m_handleBegin->setCenter(desiredHandlePos(m_angleBegin));
    m_handleEnd->setCenter(desiredHandlePos(m_angleEnd));
    m_resultArc->setRect(boundingRekt());

    // Working with Qt arcs with our data is pesky, needs some care
    auto spanAngle = (m_angleEnd < m_angleBegin ? (2 * M_PI - m_angleBegin + m_angleEnd) : (m_angleEnd - m_angleBegin));

    m_resultArc->setStartAngle((2.5 * M_PI - m_angleEnd) / (2 * M_PI) * 360 * 16);
    m_resultArc->setSpanAngle(spanAngle / (2 * M_PI) * 360 * 16);
}

QPoint EllipticalArcJig::desiredHandlePos(double angle)
{
    // Calculate an Y, making (X, Y) on a circle with radius = X
    auto relPos = QPointF(m_halfWidth * sin(angle), m_halfWidth * -cos(angle));

    // Scale Y to make the point fall on our ellipse
    relPos.ry() *= ((double)m_halfHeight / m_halfWidth);

    // Transform the point relative to the center
    return (relPos + m_handleCenter->center()).toPoint();
}

double EllipticalArcJig::desiredHandleAngle(QPoint point)
{
    // Get relative pos to the center
    auto relPos = point - m_handleCenter->center();

    // Pin X and stretch Y to make the relPos on a circle
    relPos.ry() *= ((double)m_halfWidth / m_halfHeight);

    double angle = screenAtan2(relPos.y(), relPos.x());

    return angle;
}

void EllipticalArcJig::handleMoved(int id, QPoint oldPos, QPoint newPos)
{
    double angle = 0.0;
    switch (id) {
    case 0:
        angle = desiredHandleAngle(newPos);
        m_angleBegin = angle;
        break;
    case 1:
        m_angleEnd = desiredHandleAngle(newPos);
        break;
    case 2:
        m_handleCenter->setCenter(newPos);
        break;
    default: break;
    }

    invalidateGraphics();
}

QRectF EllipticalArcJig::boundingRekt()
{
    auto c = m_handleCenter->center();
    return QRectF(c.x() - m_halfWidth, c.y() - m_halfHeight, m_halfWidth * 2, m_halfHeight * 2);
}

double EllipticalArcJig::screenAtan2(double Y, double X)
{
    // Determine angle based on coordinates
    // Note the coordinate system atan2 works is different from our desired system
    // through a combination of conversions we conclude that:
    double angle = atan2(Y, X) + M_PI / 2;
    if (angle < 0) angle += M_PI * 2;

    // This way angle became: 0 in screen system -Y axis, range [0, 2pi), clockwise
    return fmod(angle, 2 * M_PI);
}
