
#include "ellipseelement.h"
#include "ellipticalarcelement.h"
#include "rectangleelement.h"
#include "projectmanager.h"
#include "../mainwindow.h"

ProjectManager::ProjectManager(QObject *parent) :
    QObject(parent)
{

}

ProjectManager::ItemEntry *ProjectManager::getItemEntry(size_t uid)
{
    if (m_store.contains(uid))
        return &m_store[uid];
    else
        return nullptr;
}

ProjectManager::ItemEntry *ProjectManager::createNewElement(ElementType type, size_t layer)
{
    Q_ASSERT(layer >= 0 && layer <= 9);
    UiElement *newElem = nullptr;
    QTreeWidgetItem *newItem = nullptr;

    switch(type) {
    default:
    case NoneElementType:
        return nullptr;

    case LineElementType:
        break;

    case RectangleElementType:
        newElem = new RectangleElement;
        ((RectangleElement*)newElem)->setRect({ 0, 0, 100, 100 }); // TODO: defaults?
        break;

    case CircleElementType:
        break;

    case EllipseElementType:
        newElem = new EllipseElement;
        ((EllipseElement*)newElem)->setRect({ 0, 0, 100, 100 });
        break;

    case ArcElementType:
        newElem = new EllipticalArcElement;
        ((EllipticalArcElement*)newElem)->setRect({ 0, 0, 100, 100 });

        break;

    case FloatingPointTextElementType:
        break;

    case IntegerTextElementType:
        break;

    case StringElementType:
        break;
    }
    if (newElem == nullptr) return nullptr;

    newItem = new QTreeWidgetItem;

    newItem->setText(0, newElem->descriptiveText());
    newItem->setData(0, MainWindow::ItemTreeItemUidRole, newElem->uid());

    newElem->setLineWidth(m_lastLineWidth);

    m_store.insert(newElem->uid(), {newElem, newItem});

    setColor(newElem->uid(), m_lastColor);

    return &(m_store[newElem->uid()]);
}

void ProjectManager::commitJigEdit(size_t uid)
{
    Q_ASSERT(m_store.contains(uid));

    auto entry = m_store[uid];
    entry.item->setText(0, entry.element->descriptiveText());

    // TODO: log for undoing
}

void ProjectManager::setColor(size_t uid, ElementColor color)
{
    Q_ASSERT(m_store.contains(uid));

    auto entry = m_store[uid];
    auto ogColor = entry.element->color();
    entry.element->setColor(color, realColor(color));
    entry.item->setData(0, Qt::DecorationRole, elementColorList[realColor(color)]);

    // Maintain team color members consistency
    if (ogColor == Team && color != Team)
        m_itemsWithTeamColor.remove(uid);
    else if (ogColor != Team && color == Team)
        m_itemsWithTeamColor.insert(uid);
}

void ProjectManager::setTeamColor(ElementColor teamColor)
{
    m_teamColor = teamColor;

    foreach (auto i, m_itemsWithTeamColor) {
        setColor(i, Team);
    }
}

ElementColor ProjectManager::realColor(ElementColor color)
{
    if (color == Team) {
        return m_teamColor;
    }
    return color;
}
