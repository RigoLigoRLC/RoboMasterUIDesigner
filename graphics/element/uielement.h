#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <QGraphicsItem>

class ElementJig;

enum ElementType {
    NoneElementType = -1,
    LineElementType = 0,
    RectangleElementType,
    CircleElementType,
    EllipseElementType,
    ArcElementType,
    FloatingPointTextElementType,
    IntegerTextElementType,
    StringElementType,
};

enum ElementColor {
    InvalidColor = -4,
    UnknownColor = -3,
    TeamRed = -2,
    TeamBlue = -1,
    Team = 0,
    Yellow,
    Green,
    Orange,
    Purple,
    Pink,
    Cyan,
    Black,
    White,
    Blue,
    Red,
    MaxColors,
    MaxColorForSelection = White
};

extern QColor elementColorList[];

// UiElement should be used as one of the multiple base class of actual element classes
// since they would inherit other Qt provided graphics items too
class UiElement
{
public:
    UiElement(ElementType type, QGraphicsItem* item);

    // Cancellable editing experience:
    // The uncommitted operation is held by graphical elements themselves.
    // When editing begins, remember() is called so it can remember how it looked like;
    // when editing is cancelled, restore() is called so it can restore the original shape;
    // when editing is successfully completed, forget() is called so it may dispose of its
    // remembered state.
    virtual void remember() = 0;
    virtual void restore() = 0;
    virtual void forget() = 0;

    // Pass a jig and hope the UiElement will cooperate
    virtual void morphWithJigShape(ElementJig* jig) = 0;
    // Set an initial shape for a newly selected jig
    virtual void initializeJigShape(ElementJig* jig) = 0;

    // Bit flags for jigs that the current element can interact with
    virtual int applicableJigs() = 0;

    // Dump and undump element state, used for undo/redo system
    virtual QByteArray dumpState() = 0;
    virtual void undumpState(QByteArray state) = 0;

    // Descriptive text in tree item
    virtual QString descriptiveText() = 0;

    // Unique incrementing ID for in-application indexing
    size_t uid() { return m_uid; };
    static void resetUidTop() { m_uidTop = 0; } // Reset incrementing uid to zero, called when chaning document

    // Associated item
    QGraphicsItem* item() { return m_item; }

    // Element type
    ElementType type() { return m_type; }

    // Line width
    void setLineWidth(int px);
    int lineWidth() { return m_lineWidth; };

    // Color
    void setColor(ElementColor storedColor, ElementColor color);
    ElementColor color() { return m_color; };

protected:
    // Here lies some event methods that cannot be overridden in UiElement, they must be
    // called by respective event handlers in element classes.
    void ElementMousePressEvent(QGraphicsSceneMouseEvent *e);
    void ElementMouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void ElementMouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    ElementType m_type; // Element type
    static size_t m_uidTop; // UID top
    size_t m_uid; // Item UID

    ElementColor m_color;
    int m_lineWidth;

    // Subclass graphics item
    QGraphicsItem* m_item;

    // Element pressing
    bool m_mousePressing, m_mouseDragConfirmed;
    QPointF m_mousePressPoint;
    Qt::KeyboardModifiers m_mousePressModifiers;
};

#endif // UIELEMENT_H
