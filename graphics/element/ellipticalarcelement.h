#ifndef ELLIPTICALARCELEMENT_H
#define ELLIPTICALARCELEMENT_H

#include <QGraphicsEllipseItem>
#include <QPainter>
#include "uielement.h"

class EllipticalArcElement final : public QGraphicsEllipseItem, public UiElement
{
public:
    EllipticalArcElement(QGraphicsItem* parent = nullptr);

    QRect rmRect();
    std::tuple<int, int> degrees();

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

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        painter->setPen(pen());
        painter->setBrush(brush());
        painter->drawArc(rect(), startAngle(), spanAngle());
    }

private:
    // Begin/End - For output and storing in file
    // Start/Span - For use in Qt graphics
    double m_beginAngle, m_endAngle;

    QRectF m_rememberedShape;
    double m_rememberedBeginAngle, m_rememberedEndAngle;
    int m_rememberedStartAngle, m_rememberedSpanAngle;
};

#endif // ELLIPTICALARCELEMENT_H
