
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include "jigeditingevent.h"
#include "elementselectionevent.h"
#include "designergraphicsscene.h"
#include "jighandlemoveevent.h"
#include "qgraphicssceneevent.h"


DesignerGraphicsScene::DesignerGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    m_jigDoingEditing = false;

    setSceneRect({0, 0, 1920, 1080});
}

void DesignerGraphicsScene::addJig(ElementJig *jig)
{
    Q_ASSERT(jig);

    if (m_registeredJigs.contains(jig->jigType())) {
        qWarning() << "Adding multiple jigs of type" << jig->jigType();
        return;
    }

    m_registeredJigs[jig->jigType()] = jig;
    addItem(jig);
    jig->setVisible(false);
    jig->setZValue(20);
}

void DesignerGraphicsScene::selectJig(JigType type)
{
    // Hide the jig
    if (type == NullJigType || m_selection.isEmpty()) {
        if (m_visibleJig)
            m_visibleJig->setVisible(false);
        m_visibleJig = nullptr;
        return;
    }

    if (m_registeredJigs.contains(type)) {
        if (m_visibleJig)
            m_visibleJig->setVisible(false);
        m_visibleJig = m_registeredJigs[type];
        if (m_selection.size() == 1) {
            (*m_selection.begin())->initializeJigShape(m_visibleJig);
        } else {
            // TODO: group scaling
        }
        m_visibleJig->setVisible(true);
    } else {
        qWarning() << "Attempting to select an unregistered jig of type" << type;
    }
}

void DesignerGraphicsScene::insertElement(UiElement *element, int layer)
{
    Q_ASSERT(layer >= 0 && layer <= 9);
    Q_ASSERT(element);

    element->item()->setZValue(layer + 10);
    addItem(element->item());
    setElementAsSelection(element);
}

void DesignerGraphicsScene::clearElementSelection()
{
    // TODO: anything to do here before?
    m_selection.clear();
    emit selectionCleared();
    broadcastStateAccordingToSelection();
}

void DesignerGraphicsScene::setElementAsSelection(UiElement *element)
{
    clearElementSelection();
    addElementToSelection(element);
    broadcastStateAccordingToSelection();
}

void DesignerGraphicsScene::toggleElementSelection(UiElement *element)
{
    if (m_selection.contains(element)) {
        removeElementFromSelection(element);
    } else {
        addElementToSelection(element);
    }
}

void DesignerGraphicsScene::addElementToSelection(UiElement *element)
{
    m_selection.insert(element);
    emit addedToSelection(element->uid());
    broadcastStateAccordingToSelection();
}

void DesignerGraphicsScene::removeElementFromSelection(UiElement *element)
{
    m_selection.remove(element);
    emit removedFromSelection(element->uid());
    broadcastStateAccordingToSelection();
}

void DesignerGraphicsScene::broadcastStateAccordingToSelection()
{
    if (m_selection.size() == 0) {
        // Clear jig
        selectJig(JigType::NullJigType);
        emit jigStateChanged(0, JigType::NullJigType);
        emit colorSelectionChanged(InvalidColor);
    } else if (m_selection.size() == 1) {
        // Single element
        auto &element = *m_selection.begin();
        // See if we already had a jig on screen
        if (m_visibleJig) {
            // See if the jig is still applicable
            if (m_visibleJig->type() & element->applicableJigs()) {
                // Great
            } else {
                // Find the first applicable one
                for(auto i = 1; i != 0; i <<= 1) {
                    if (i & element->applicableJigs()) {
                        selectJig((JigType)i);
                        break;
                    }
                }
            }
        } else {
            // Find the first applicable one
            for(auto i = 1; i != 0; i <<= 1) {
                if (i & element->applicableJigs()) {
                    selectJig((JigType)i);
                    break;
                }
            }
        }
        emit jigStateChanged(element->applicableJigs(), m_visibleJig->jigType());
        emit colorSelectionChanged(element->color());
    } else {
        // Multiple elements
        // FIXME: Hard code for now, should detect if all supports rectangle jig
        selectJig(JigType::AnchorJigType);
        emit jigStateChanged(JigType::AnchorJigType, JigType::AnchorJigType);
        emit colorSelectionChanged(UnknownColor);
    }
}

bool DesignerGraphicsScene::event(QEvent *e)
{
    switch (int(e->type())) {
    case JigEditingEvent::type: {
        jigEditingEvent((JigEditingEvent*)e);
        return true;
    }
    case ElementSelectionEvent::type: {
        auto se = (ElementSelectionEvent*)e;
        if (se->modifiers() & Qt::ControlModifier) {
            toggleElementSelection(se->element());
        } else {
            setElementAsSelection(se->element());
        }
        return true;
    }
    default:
        return QGraphicsScene::event(e);
    }
}

void DesignerGraphicsScene::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        // Escape key is used to cancel an ongoing jig operation
        if (!m_selection.isEmpty() && m_visibleJig && m_jigDoingEditing) {
            // HACK:
            // Emulate a cancel event that was from the handle (but actually from scene's keyboard event)
            // (because unless you make jig or jig handle "focusable" they would not be getting
            // keyPressEvent's from scene, so we have to intercept key event here)
            // This will be forwarded to scene in the jig.
            // NOTE:
            // I have made sure that the args here except for the new state (CancelEdit) is not used
            // in the code path.

            sendEvent(m_visibleJig, new JigHandleMoveEvent(nullptr, {}, {}, JigHandleMoveEvent::CancelEdit));
        }
    } else {
        QGraphicsScene::keyPressEvent(e);
    }
}

void DesignerGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene::mousePressEvent(e);

    // Check if the event is processed
    if (e->isAccepted())
        return;

    // If not, clear selection
    clearElementSelection();
}

void DesignerGraphicsScene::jigEditingEvent(JigEditingEvent *e)
{
    // FIXME: switch to selection state, and handle group scaling
    switch (e->state()) {
    case JigEditingEvent::PrepareEdit:
        if (!m_jigDoingEditing) {
            e->jig()->remember();
            if (m_selection.size() == 1) {
                auto itemEditing = *m_selection.begin();
                itemEditing->remember();
            }
        }
        break;
    case JigEditingEvent::BeginEdit:
        if (!m_jigDoingEditing) {
            m_jigDoingEditing = true;
            if (m_selection.size() == 1) {
                auto itemEditing = *m_selection.begin();
                itemEditing->morphWithJigShape(e->jig());
            }
        }
        break;
    case JigEditingEvent::DoingEdit:
        if (m_jigDoingEditing) {
            if (m_selection.size() == 1) {
                auto itemEditing = *m_selection.begin();
                itemEditing->morphWithJigShape(e->jig());
            }
        }
        break;
    case JigEditingEvent::CommitEdit:
        if (m_jigDoingEditing) {
            if (m_selection.size() == 1) {
                auto itemEditing = *m_selection.begin();
                itemEditing->forget();
                e->jig()->forget();
                emit commitJigEdit(itemEditing->uid());
                m_jigDoingEditing = false;
            }
        }
        break;
    case JigEditingEvent::CancelEdit:
        if (m_jigDoingEditing) {
            if (m_selection.size() == 1) {
                auto itemEditing = *m_selection.begin();
                itemEditing->restore();
                e->jig()->restore();
                m_jigDoingEditing = false;
            }
        }
        break;
    }
}
