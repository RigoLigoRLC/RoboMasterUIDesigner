#ifndef RECTANGLEELEMENT_H
#define RECTANGLEELEMENT_H

#include <QGraphicsRectItem>
#include "uielement.h"

class RectangleElement final : public QGraphicsRectItem, public UiElement
{
public:
    RectangleElement(QGraphicsItem* parent = nullptr);

    // Rect in RM client system
    QRect rmRect();

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

#endif // RECTANGLEELEMENT_H
