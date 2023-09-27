#ifndef DESIGNERGRAPHICSSCENE_H
#define DESIGNERGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QWidget>
#include <QSet>
#include "elementjig.h"
#include "uielement.h"

class JigEditingEvent;

class DesignerGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DesignerGraphicsScene(QObject *parent = nullptr);

    void addJig(ElementJig* jig);
    void selectJig(JigType type);

    void insertElement(UiElement* element, int layer);

    void clearElementSelection();
    void setElementAsSelection(UiElement* element);
    void toggleElementSelection(UiElement* element);
    void addElementToSelection(UiElement* element);
    void removeElementFromSelection(UiElement* element);

private:
    void broadcastStateAccordingToSelection();

signals:
    void jigStateChanged(int applicableJigs, JigType selectedJig);
    void colorSelectionChanged(ElementColor color);
    void commitJigEdit(size_t uid);

    void addedToSelection(size_t uid);
    void batchAddedToSelection(QList<size_t> uid);
    void removedFromSelection(size_t uid);
    void selectionCleared();

protected:
    virtual bool event(QEvent* e) override;
    virtual void keyPressEvent(QKeyEvent* e) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e) override;

    void jigEditingEvent(JigEditingEvent* e);

private:
    bool m_jigDoingEditing; // User is dragging handles on the jig

    QMap<JigType, ElementJig*> m_registeredJigs;
    ElementJig* m_visibleJig = nullptr;

    QSet<UiElement*> m_selection;

};

#endif // DESIGNERGRAPHICSSCENE_H
