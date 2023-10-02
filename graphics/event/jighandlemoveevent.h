#ifndef JIGHANDLEMOVEEVENT_H
#define JIGHANDLEMOVEEVENT_H

#include <QEvent>
#include <QPoint>

class JigHandle;

class JigHandleMoveEvent : public QEvent
{
public:
    static constexpr int type = QEvent::User + 1;
    enum NewEditingState {
        PrepareEdit,
        BeginEdit,
        DoingEdit,
        CommitEdit,
        CancelEdit,
    };

    JigHandleMoveEvent(JigHandle* handle, QPoint oldPos, QPoint newPos, NewEditingState state) :
        QEvent((QEvent::Type)type),
        m_handle(handle),
        m_oldPos(oldPos),
        m_newPos(newPos),
        m_state(state) {  };

    QPoint newPos() { return m_newPos; }
    QPoint oldPos() { return m_oldPos; }
    JigHandle* handle() { return m_handle; }
    NewEditingState state() { return m_state; }

protected:
    JigHandle *const m_handle;
    const QPoint m_oldPos, m_newPos;
    const NewEditingState m_state;
};

#endif // JIGHANDLEMOVEEVENT_H
