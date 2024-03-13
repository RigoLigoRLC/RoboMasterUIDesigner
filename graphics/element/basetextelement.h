#ifndef BASETEXTELEMENT_H
#define BASETEXTELEMENT_H

#include <QGraphicsItem>
#include <QPen>
#include "uielement.h"

class BaseTextElement : public UiElement, public QGraphicsItem
{
public:
    BaseTextElement(QGraphicsItem *parent = nullptr);
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

    void setText(QString text);
    QString text() { return m_textString; }
    void setFontSize(int size);
    int fontSize() { return m_fontSize; }
    void setPen(QPen pen);
    QPen pen() { return m_pen; }

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e) override { ElementMousePressEvent(e); }
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *e) override { ElementMouseMoveEvent(e); }
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override { ElementMouseReleaseEvent(e); }

    static QMap<int, QVector<float>> vectorFont;

private:
    QPointF m_rememberedPos;

    QString m_textString;
    QPen m_pen;
    int m_fontSize;
};

#endif // BASETEXTELEMENT_H
