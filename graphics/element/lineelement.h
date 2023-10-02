#ifndef LINEELEMENT_H
#define LINEELEMENT_H

#include <QGraphicsLineItem>
#include "uielement.h"

class LineElement final : public QGraphicsLineItem, public UiElement
{
public:
    LineElement(QGraphicsItem* parent = nullptr);

    QLine rmLine();

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
    virtual QPainterPath shape() const override;

private:
    QLine m_rememberedState;
};

#endif // LINEELEMENT_H
