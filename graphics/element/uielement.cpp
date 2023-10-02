
#include <QApplication>
#include <QStyleHints>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include "elementselectionevent.h"

#include "ellipseelement.h"
#include "ellipticalarcelement.h"
#include "lineelement.h"
#include "uielement.h"
#include "rectangleelement.h"

size_t UiElement::m_uidTop = 0;
QColor elementColorList[] = {
    {0, 0, 0}, // Team
    {255, 238, 70}, // Yellow
    {168, 255, 45}, // Green
    {255, 163, 8}, // Orange
    {240, 41, 246}, // Purple
    {255, 100, 142}, // Pink
    {69, 255, 242}, // Cyan
    {255, 255, 255}, // Black
    {0, 0, 0}, // White
    {47, 168, 223}, // Blue
    {255, 69, 70}, // Red
};

UiElement::UiElement(ElementType type, QGraphicsItem *item) :
    m_type(type),
    m_item(item)
{
    m_mousePressing = false;
    m_uid = m_uidTop++;
}

UiElement::~UiElement()
{

}

void UiElement::setLineWidth(int px)
{
    QPen pen;

    m_lineWidth = px;

    // TODO: Implement all types of elements
    switch (m_type) {
    case NoneElementType:
        break;
    case LineElementType:
        pen = ((LineElement*)m_item)->pen();
        pen.setWidth(px);
        ((LineElement*)m_item)->setPen(pen);
        break;
    case RectangleElementType:
        pen = ((RectangleElement*)m_item)->pen();
        pen.setWidth(px);
        ((RectangleElement*)m_item)->setPen(pen);
        break;
    case CircleElementType:
        break;
    case EllipseElementType:
        pen = ((EllipseElement*)m_item)->pen();
        pen.setWidth(px);
        ((EllipseElement*)m_item)->setPen(pen);
        break;
    case ArcElementType:
        pen = ((EllipticalArcElement*)m_item)->pen();
        pen.setWidth(px);
        ((EllipticalArcElement*)m_item)->setPen(pen);
        break;
    case FloatingPointTextElementType:
        break;
    case IntegerTextElementType:
        break;
    case StringElementType:
        break;
    }
}

void UiElement::setColor(ElementColor storedColor, ElementColor color)
{
    QPen pen;
    m_color = storedColor;

    // TODO: Implement all types of elements
    switch (m_type) {
    case NoneElementType:
        break;
    case LineElementType:
        pen = ((LineElement*)m_item)->pen();
        pen.setColor(elementColorList[color]);
        ((LineElement*)m_item)->setPen(pen);
        break;
    case RectangleElementType:
        pen = ((RectangleElement*)m_item)->pen();
        pen.setColor(elementColorList[color]);
        ((RectangleElement*)m_item)->setPen(pen);
        break;
    case CircleElementType:
        break;
    case EllipseElementType:
        pen = ((EllipseElement*)m_item)->pen();
        pen.setColor(elementColorList[color]);
        ((EllipseElement*)m_item)->setPen(pen);
        break;
    case ArcElementType:
        pen = ((EllipticalArcElement*)m_item)->pen();
        pen.setColor(elementColorList[color]);
        ((EllipticalArcElement*)m_item)->setPen(pen);
        break;
    case FloatingPointTextElementType:
        break;
    case IntegerTextElementType:
        break;
    case StringElementType:
        break;
    }
}

void UiElement::ElementMousePressEvent(QGraphicsSceneMouseEvent *e)
{
    m_mousePressing = true;
    m_mouseDragConfirmed = false;
    m_mousePressPoint = e->pos();
    m_mousePressModifiers = e->modifiers();
}

void UiElement::ElementMouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if (!m_mouseDragConfirmed) {
        if (QLineF(m_mousePressPoint, e->pos()).length() > qApp->styleHints()->startDragDistance())
            m_mouseDragConfirmed = true;
        else
            return;
    }

}

void UiElement::ElementMouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    if (m_mousePressing) {
        m_mousePressing = false;
        if (!m_mouseDragConfirmed) {
            qApp->sendEvent(m_item->scene(),
                            new ElementSelectionEvent(this, m_mousePressModifiers)
                            );
        }
    }
}
