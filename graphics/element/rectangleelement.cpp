
#include <QPen>
#include <QDataStream>
#include <QBuffer>
#include "rectangleelement.h"
#include "elementjig.h"
#include "rectangularjig.h"

RectangleElement::RectangleElement(QGraphicsItem *parent) :
    QGraphicsRectItem(parent),
    UiElement(RectangleElementType, this)
{

}

void RectangleElement::remember()
{
    m_rememberedShape = rect();
}

void RectangleElement::restore()
{
    setRect(m_rememberedShape);
}

void RectangleElement::forget()
{
    m_rememberedShape = rect().normalized();
    restore();
}

void RectangleElement::morphWithJigShape(ElementJig *jig)
{
    switch (jig->jigType()) {
    case RectangularJigType:
        setRect(((RectangularJig*)jig)->jigRect());
        break;
    case AnchorJigType:
        // TODO
        break;
    case NullJigType:
    default:
        break;
    }
}

void RectangleElement::initializeJigShape(ElementJig *jig)
{
    switch (jig->jigType()) {
    case RectangularJigType:
        ((RectangularJig*)jig)->setJigRect(rect().toAlignedRect());
        break;
    case AnchorJigType:
        // TODO
        break;
    case NullJigType:
    default:
        break;
    }
}

int RectangleElement::applicableJigs()
{
    return RectangularJigType | AnchorJigType;
}

QByteArray RectangleElement::dumpState()
{
    QBuffer buf;
    QDataStream ds(&buf);
    ds << rect();
    QByteArray ret = buf.readAll();
    return ret;
}

void RectangleElement::undumpState(QByteArray state)
{
    QBuffer buf(&state);
    QDataStream ds(&buf);
    QRect rect;
    ds >> rect;
    setRect(rect);
}

QString RectangleElement::descriptiveText()
{
    return QObject::tr("Rectangle (%1 x %2)").arg(rect().width()).arg(rect().height());
}
