
#include <QPen>
#include <QDataStream>
#include <QBuffer>
#include "anchorjig.h"
#include "ellipseelement.h"
#include "elementjig.h"
#include "ellipticalarcjig.h"
#include "rectangularjig.h"

EllipseElement::EllipseElement(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent),
    UiElement(EllipseElementType, this)
{

}

QRect EllipseElement::rmRect()
{
    auto r = rect();
    r.setTop(1080 - r.top());
    r.setBottom(1080 - r.bottom());
    return r.toAlignedRect();
}

void EllipseElement::remember()
{
    m_rememberedShape = rect();
}

void EllipseElement::restore()
{
    setRect(m_rememberedShape);
}

void EllipseElement::forget()
{
    m_rememberedShape = rect().normalized();
    restore();
}

void EllipseElement::morphWithJigShape(ElementJig *jig)
{
    switch (jig->jigType()) {
    case RectangularJigType:
        setRect(((RectangularJig*)jig)->jigRect());
        break;
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

void EllipseElement::initializeJigShape(ElementJig *jig)
{
    switch (jig->jigType()) {
    case RectangularJigType:
        ((RectangularJig*)jig)->setJigRect(rect().toAlignedRect());
        break;
    case AnchorJigType:
        ((AnchorJig*)jig)->setRect(rect().toAlignedRect());
        break;
    case NullJigType:
    default:
        break;
    }
}

int EllipseElement::applicableJigs()
{
    return RectangularJigType | AnchorJigType;
}

QByteArray EllipseElement::dumpState()
{
    QBuffer buf;
    QDataStream ds(&buf);
    ds << rect();
    QByteArray ret = buf.readAll();
    return ret;
}

void EllipseElement::undumpState(QByteArray state)
{
    QBuffer buf(&state);
    QDataStream ds(&buf);
    QRect rect;
    ds >> rect;
    setRect(rect);
}

QString EllipseElement::descriptiveText()
{
    return QObject::tr("Ellipse (%1 x %2)").arg(rect().width()).arg(rect().height());
}
