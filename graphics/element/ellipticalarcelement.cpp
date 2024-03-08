
#include <QPen>
#include <QDataStream>
#include <QBuffer>
#include <QtMath>
#include "anchorjig.h"
#include "ellipticalarcelement.h"
#include "elementjig.h"
#include "ellipticalarcjig.h"
#include "rectangularjig.h"

EllipticalArcElement::EllipticalArcElement(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent),
    UiElement(ArcElementType, this)
{
    m_beginAngle = 0;
    m_endAngle = M_PI_2;
    setStartAngle(0);
    setSpanAngle(90 * 16);
}

QRect EllipticalArcElement::rmRect()
{
    auto r = rect();
    r.setTop(1080 - r.top());
    r.setBottom(1080 - r.bottom());
    return r.toAlignedRect();
}

std::tuple<int, int> EllipticalArcElement::degrees()
{
    return {(int)(m_beginAngle / M_PI * 180), (int)(m_endAngle / M_PI * 180)};
}

void EllipticalArcElement::remember()
{
    m_rememberedShape = rect();
    m_rememberedBeginAngle = m_beginAngle;
    m_rememberedEndAngle = m_endAngle;
    m_rememberedStartAngle = startAngle();
    m_rememberedSpanAngle = spanAngle();
}

void EllipticalArcElement::restore()
{
    setRect(m_rememberedShape);

    m_beginAngle = m_rememberedBeginAngle;
    m_endAngle = m_rememberedEndAngle;
    setStartAngle(m_rememberedStartAngle);
    setSpanAngle(m_rememberedSpanAngle);
}

void EllipticalArcElement::forget()
{
    m_rememberedShape = rect().normalized();
    m_rememberedBeginAngle = m_beginAngle;
    m_rememberedEndAngle = m_endAngle;
    m_rememberedStartAngle = startAngle();
    m_rememberedSpanAngle = spanAngle();
    restore();
}

void EllipticalArcElement::morphWithJigShape(ElementJig *jig)
{
    switch (jig->jigType()) {
    case RectangularJigType:
        setRect(((RectangularJig*)jig)->jigRect());
        break;
    case EllipticalArcJigType: {
        auto angles = ((EllipticalArcJig*)jig)->qtArcStartSpanAngles();
        setStartAngle(std::get<0>(angles));
        setSpanAngle(std::get<1>(angles));
        auto angles2 = ((EllipticalArcJig*)jig)->rmArcBeginEndAnglesRadian();
        m_beginAngle = std::get<0>(angles2);
        m_endAngle = std::get<1>(angles2);
        setRect(((EllipticalArcJig*)jig)->jigRect());
        break;
    }
    case AnchorJigType: {
        auto r = rect();
        r.moveTopLeft(((AnchorJig*)jig)->anchor());
        setRect(r);
        break;
    }
    case NullJigType:
    default:
        break;
    }
}

void EllipticalArcElement::initializeJigShape(ElementJig *jig)
{
    switch (jig->jigType()) {
    case RectangularJigType:
        ((RectangularJig*)jig)->setJigRect(rect().toAlignedRect());
        break;
    case EllipticalArcJigType:
        ((EllipticalArcJig*)jig)->setRect(rect().toAlignedRect());
        ((EllipticalArcJig*)jig)->setBeginEndAngles(m_beginAngle, m_endAngle);
        break;
    case AnchorJigType:
        ((AnchorJig*)jig)->setRect(rect().toAlignedRect());
        break;
    case NullJigType:
    default:
        break;
    }
}

int EllipticalArcElement::applicableJigs()
{
    return RectangularJigType | EllipticalArcJigType | AnchorJigType;
}

QByteArray EllipticalArcElement::dumpState()
{
    QBuffer buf;
    QDataStream ds(&buf);
    ds << rect();
    QByteArray ret = buf.readAll();
    return ret;
}

void EllipticalArcElement::undumpState(QByteArray state)
{
    QBuffer buf(&state);
    QDataStream ds(&buf);
    QRect rect;
    ds >> rect;
    setRect(rect);
}

QString EllipticalArcElement::descriptiveText()
{
    return QObject::tr("Arc (%1 x %2)").arg(rect().width()).arg(rect().height());
}
