#ifndef JIGEDITINGEVENT_H
#define JIGEDITINGEVENT_H

#include <QEvent>
#include "elementjig.h"

class JigEditingEvent : public QEvent {
public:
    static constexpr int type = QEvent::User + 2;
    enum NewEditingState {
        PrepareEdit,
        BeginEdit,
        DoingEdit,
        CommitEdit,
        CancelEdit,
    };

    JigEditingEvent(NewEditingState state, ElementJig* jig) :
        QEvent((QEvent::Type)type),
        m_state(state),
        m_jig(jig) { }

    NewEditingState state() { return m_state; }
    ElementJig* jig() { return m_jig; }

protected:
    NewEditingState m_state;
    ElementJig *m_jig;
};

#endif // JIGEDITINGEVENT_H
