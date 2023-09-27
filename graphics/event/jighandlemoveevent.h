#ifndef JIGHANDLEMOVEEVENT_H
#define JIGHANDLEMOVEEVENT_H

#include <QEvent>
#include <QPoint>

class JigHandleMoveEvent : public QEvent
{
public:
    static constexpr int type = QEvent::User + 1;
    enum NewEditingState {
        BeginEdit,
        DoingEdit,
        CommitEdit,
        CancelEdit,
    };

    JigHandleMoveEvent(int handleId, QPoint oldPos, QPoint newPos, NewEditingState state) :
        QEvent((QEvent::Type)type),
        m_handleId(handleId),
        m_oldPos(oldPos),
        m_newPos(newPos),
        m_state(state) {  };

    QPoint newPos() { return m_newPos; }
    QPoint oldPos() { return m_oldPos; }
    int handleId() { return m_handleId; }
    NewEditingState state() { return m_state; }

protected:
    const int m_handleId;
    const QPoint m_oldPos, m_newPos;
    const NewEditingState m_state;
};

#endif // JIGHANDLEMOVEEVENT_H
