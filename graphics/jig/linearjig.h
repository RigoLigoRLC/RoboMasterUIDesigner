#ifndef LINEARJIG_H
#define LINEARJIG_H

#include "elementjig.h"
#include "jighandle.h"

class LinearJig final : public ElementJig
{
public:
    LinearJig(QGraphicsItem* parent = nullptr);

    void setLine(QLine line);
    QLine line();

    virtual JigType jigType() override { return JigType::LinearJigType; }

    virtual void remember() override { m_rememberedState = m_indicatorLine->line(); };
    virtual void restore() override;;
    virtual void forget() override { remember(); restore(); };

protected:
    virtual bool sceneEvent(QEvent* e) override;

private:
    void jigHandleMoved(int id, QPoint oldPos, QPoint newPos);
    void invalidateHandlePositions();

private:
    JigHandle *m_handleP1, *m_handleP2;
    QGraphicsLineItem *m_indicatorLine;

    QLineF m_rememberedState;
};

#endif // LINEARJIG_H
