
#include <QEvent>
#include <QApplication>
#include <QGraphicsScene>
#include "jigeditingevent.h"
#include "jighandlemoveevent.h"
#include "elementjig.h"


ElementJig::ElementJig(QGraphicsItem *parent) :
    QGraphicsItemGroup(parent)
{
    // Jigs are groups but should never interfere with its children's events
    // Let them propagate events here when needed
    setHandlesChildEvents(false);
}

bool ElementJig::sceneEvent(QEvent *e)
{
    switch (int(e->type())) {
    case JigHandleMoveEvent::type: {
        // This type of event MUST be propagated to base class ElementJig even after
        // subclass processes its information, because we have shared event propagation
        // logic here. We tell the scene what happened inside the jig so the signal
        // slot world outside graphics system will finally know what to do with
        // the actual graphics data.
        auto me = (JigHandleMoveEvent*)e;
        switch (me->state()) {
        case JigHandleMoveEvent::BeginEdit:
            qApp->sendEvent(scene(), new JigEditingEvent(JigEditingEvent::BeginEdit, this));
            break;
        case JigHandleMoveEvent::DoingEdit:
            qApp->sendEvent(scene(), new JigEditingEvent(JigEditingEvent::DoingEdit, this));
            break;
        case JigHandleMoveEvent::CommitEdit:
            qApp->sendEvent(scene(), new JigEditingEvent(JigEditingEvent::CommitEdit, this));
            break;
        case JigHandleMoveEvent::CancelEdit:
            qApp->sendEvent(scene(), new JigEditingEvent(JigEditingEvent::CancelEdit, this));
            break;
        }
        return true;
    }
    default:
        return QGraphicsItemGroup::sceneEvent(e);
    }
}
