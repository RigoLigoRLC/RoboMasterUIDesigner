#include "lineelement.h"
#include "anchorjig.h"
#include "elementjig.h"
#include "linearjig.h"
#include "qbuffer.h"
#include <QPen>

LineElement::LineElement(QGraphicsItem *parent) :
    QGraphicsLineItem(parent),
    UiElement(LineElementType, this)
{
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    setPen(pen);
}

QLine LineElement::rmLine()
{
    auto l = line().toLine();
    return { l.x1(), 1080 - l.y1(), l.x2(), 1080 - l.y2() };
}

void LineElement::remember()
{
    m_rememberedState = line().toLine();
}

void LineElement::restore()
{
    setLine(m_rememberedState);
}

void LineElement::forget()
{
    remember();
}

void LineElement::morphWithJigShape(ElementJig *jig)
{
    switch (jig->jigType()) {
    case LinearJigType: {
        setLine(((LinearJig*)jig)->line());
        break;
    }
    case AnchorJigType: {
        QLine l = line().toLine();
        QPoint delta = ((AnchorJig*)jig)->anchor() - l.p1();
        setLine({ l.p1() + delta, l.p2() + delta });
        break;
    }
    default:
        break;
    }
}

void LineElement::initializeJigShape(ElementJig *jig)
{
    switch (jig->jigType()) {
    case LinearJigType: {
        ((LinearJig*)jig)->setLine(line().toLine());
        break;
    }
    case AnchorJigType: {
        ((AnchorJig*)jig)->setRect(QRectF(line().p1(), line().p1()).toRect());
        break;
    }
    default:
        break;
    }
}

int LineElement::applicableJigs()
{
    return LinearJigType | AnchorJigType;
}

QByteArray LineElement::dumpState()
{
    QBuffer buf;
    QDataStream ds(&buf);
    ds << line();
    QByteArray ret = buf.readAll();
    return ret;
}

void LineElement::undumpState(QByteArray state)
{
    QBuffer buf(&state);
    QDataStream ds(&buf);
    QLine line;
    ds >> line;
    setLine(line);
}

QString LineElement::descriptiveText()
{
    auto l = line().toLine();
    return QObject::tr("Line (%1, %2) - (%3, %4)")
                       .arg(l.p1().x())
                       .arg(1080 - l.p1().y())
                       .arg(l.p2().x())
                       .arg(1080 - l.p2().y());
}

QPainterPath LineElement::shape() const
{
    // Make 1px line easier to select
    //https://www.qtcentre.org/threads/48304-QGraphicsLineItem-how-to-make-it-easier-to-select-move-the-line?p=217518#post217518
    QPainterPath path;
    QPainterPathStroker stroker;
    stroker.setWidth(2 + lineWidth());
    path.moveTo(line().p1());
    path.lineTo(line().p2());
    return stroker.createStroke(path);
}
