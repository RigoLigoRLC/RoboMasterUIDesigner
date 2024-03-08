#ifndef ANCHORJIG_H
#define ANCHORJIG_H

#include "elementjig.h"
#include "jighandle.h"

class AnchorJig final : public ElementJig
{
public:
    AnchorJig(QGraphicsItem* parent = nullptr);

    void setRect(QRect rect);
    QPoint anchor();

    virtual JigType jigType() override { return JigType::AnchorJigType; }

    virtual void remember() override { m_rememberedRect = m_boundingRect->rect(); };
    virtual void restore() override;;
    virtual void forget() override { remember(); restore(); };

protected:
    virtual bool sceneEvent(QEvent* e) override;

private:
    void jigHandleMoved(int id, QPoint oldPos, QPoint newPos);
    void invalidateHandlePositions();

private:
    JigHandleAnchor *m_handle;
    QGraphicsRectItem *m_boundingRect;

    QRectF m_rememberedRect;
};

#endif // ANCHORJIG_H
