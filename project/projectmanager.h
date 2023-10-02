#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QTreeWidgetItem>
#include <QMap>

#include "uielement.h"

class ProjectManager : public QObject
{
    Q_OBJECT
public:
    ProjectManager(QObject *parent);

    struct ItemEntry {
        UiElement *element;
        QTreeWidgetItem* item;
    };

    ItemEntry* getItemEntry(size_t uid);

    ItemEntry* createNewElement(ElementType type, size_t layer, QPoint center);
    bool removeElement(size_t uid);
    void commitJigEdit(size_t uid);

    void setElementLayer(size_t uid, int);
    int elementLayer(size_t uid);
    void setColor(size_t uid, ElementColor color);
    void setTeamColor(ElementColor teamColor);

private:
    ElementColor realColor(ElementColor);

private:
    QMap<size_t, ItemEntry> m_store;
    QSet<size_t> m_itemsWithTeamColor;
    ElementColor m_lastColor = Team;
    ElementColor m_teamColor = Red;
    int m_lastLineWidth = 5;

};

#endif // PROJECTMANAGER_H
