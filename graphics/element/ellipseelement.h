#ifndef ELLIPSEELEMENT_H
#define ELLIPSEELEMENT_H

#include <QGraphicsEllipseItem>
#include "uielement.h"

class EllipseElement final : public QGraphicsEllipseItem, public UiElement
{
public:
    EllipseElement(QGraphicsItem* parent = nullptr);

    virtual void remember() override;
    virtual void restore() override;
    virtual void forget() override;

    virtual void morphWithJigShape(ElementJig* jig) override;
    virtual void initializeJigShape(ElementJig* jig) override;

    virtual int applicableJigs() override;

    virtual QByteArray dumpState() override;
    virtual void undumpState(QByteArray state) override;

    virtual QString descriptiveText() override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e) override { ElementMousePressEvent(e); }
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override { ElementMouseMoveEvent(e); }
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override { ElementMouseReleaseEvent(e); }

private:
    QRectF m_rememberedShape;
};

#endif // ELLIPSEELEMENT_H
