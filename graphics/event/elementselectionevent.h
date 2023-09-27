#ifndef ELEMENTSELECTIONEVENT_H
#define ELEMENTSELECTIONEVENT_H

#include <QEvent>
#include "uielement.h"

class ElementSelectionEvent : public QEvent {
public:
    static constexpr int type = QEvent::User + 3;
    ElementSelectionEvent(UiElement* element, Qt::KeyboardModifiers modifiers) :
        QEvent((QEvent::Type)type),
        m_element(element),
        m_modifiers(modifiers) {};

    UiElement* element() { return m_element; }
    Qt::KeyboardModifiers modifiers() { return m_modifiers; }

private:
    UiElement *m_element;
    Qt::KeyboardModifiers m_modifiers;
};

#endif // ELEMENTSELECTIONEVENT_H
