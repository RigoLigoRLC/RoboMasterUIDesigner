#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QButtonGroup>
#include <QTreeWidgetItem>
#include <QLabel>

#include "graphics/designergraphicsscene.h"
#include "projectmanager.h"
#include "rectangularjig.h"
#include "ellipticalarcjig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initializeUi();

    // Editor state modifiers
    void setCurrentLayer(int layer);
    void updatePropertyPanelAccordingToSelection();

private slots:
    void jigSelectionChanged(int id);
    void colorSelctionChanged(int id);
    void teamColorSelectionChanged(int id);
    void treeItemSelectionChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void _q_jigStateChanged(int applicableJigs, JigType selectedJig);
    void _q_colorChanged(ElementColor color);

    void _q_addedToSelection(size_t uid);
    void _q_batchAddedToSelection(QList<size_t> uid);
    void _q_removedFromSelection(size_t uid);
    void _q_selectionCleared();

    void _q_jigEditCommitted(size_t uid);

    void on_tbAdd_triggered(QAction *arg1);
    void on_tbAdd_clicked();
    void on_spinWidth_valueChanged(int arg1);
    void on_btnDelete_clicked();

public:
    // Constants
    static constexpr int ItemTreeItemLayerIdRole = Qt::UserRole + 1,
                         ItemTreeItemUidRole = Qt::UserRole + 2;

private:
    // Scene
    DesignerGraphicsScene *m_scene;

    // Project manager
    ProjectManager *m_prj;

    // Viewmodel bookkeeping
    QSet<size_t> m_selection;

    // Editor state
    int m_currentLayer = 0; // Layer 0 ~ 9

private:
    Ui::MainWindow *ui;

    // Mutual exclusion group for jig selectors
    QButtonGroup m_jigSelectionGroup;
    QButtonGroup m_teamColorSelectionGroup, m_colorSelectionGroup;

    // Jigs
    RectangularJig *m_rectJig;
    EllipticalArcJig *m_arcJig;
    // Circle jig
    // Linear jig
    // Anchor jig

    // Add item menu
    QMenu *m_menuAddItem;
    QAction *m_actAddLine;
    QAction *m_actAddRect;
    QAction *m_actAddArc;
    QAction *m_actAddEllipse;

    // Status bar indicators
    QLabel *m_currentLayerIndicator;

    // Tree items for layer groups 0~9
    QVector<QTreeWidgetItem *> m_itemTreeLayerGroupItems;
};
#endif // MAINWINDOW_H
