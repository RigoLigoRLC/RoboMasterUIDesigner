#ifndef ELEMENTJIG_H
#define ELEMENTJIG_H

#include <QGraphicsItemGroup>

enum JigType {
    NullJigType = 0,
    RectangularJigType = 1,
    EllipticalArcJigType = 2,
    CircularJigType = 4,
    LinearJigType = 8,
    AnchorJigType = 16,
};

class ElementJig : public QGraphicsItemGroup {
public:
    ElementJig(QGraphicsItem *parent = nullptr);

    void setActive(bool active) { m_active = active; setVisible(active); };

    virtual JigType jigType() { return NullJigType; }

    // Cancellable editing experience; see uielement.h
    virtual void remember() = 0;
    virtual void restore() = 0;
    virtual void forget() = 0;

protected:
    virtual bool sceneEvent(QEvent* e) override;

protected:
    bool m_active = false;

signals:
    void jigTransformed();
};

#endif // ELEMENTJIG_H
