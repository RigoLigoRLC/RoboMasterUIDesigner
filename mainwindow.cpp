#include "mainwindow.h"
#include "elementjig.h"
#include "./ui_mainwindow.h"
#include "uielement.h"
#include "projectmanager.h"
#include <QGraphicsScene>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeUi();

    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeUi()
{
    // Add jig selection buttons to mutual exclusion group
    m_jigSelectionGroup.setExclusive(true);
    m_jigSelectionGroup.addButton(ui->btnRect, RectangularJigType);
    m_jigSelectionGroup.addButton(ui->btnArc, EllipticalArcJigType);
    m_jigSelectionGroup.addButton(ui->btnCircle, CircularJigType);
    m_jigSelectionGroup.addButton(ui->btnLine, LinearJigType);
    m_jigSelectionGroup.addButton(ui->btnAnchor, AnchorJigType);
    connect(&m_jigSelectionGroup, &QButtonGroup::idClicked, this, &MainWindow::jigSelectionChanged);

    // Add color selection buttons to group
    m_teamColorSelectionGroup.setExclusive(true);
    m_teamColorSelectionGroup.addButton(ui->btnTeamRed, Red);
    m_teamColorSelectionGroup.addButton(ui->btnTeamBlue, Blue);
    connect(&m_teamColorSelectionGroup, &QButtonGroup::idClicked, this, &MainWindow::teamColorSelectionChanged);
    m_colorSelectionGroup.addButton(ui->btnTeamColor, Team);
    m_colorSelectionGroup.addButton(ui->btnYellow, Yellow);
    m_colorSelectionGroup.addButton(ui->btnGreen, Green);
    m_colorSelectionGroup.addButton(ui->btnOrange, Orange);
    m_colorSelectionGroup.addButton(ui->btnPurple, Purple);
    m_colorSelectionGroup.addButton(ui->btnPink, Pink);
    m_colorSelectionGroup.addButton(ui->btnCyan, Cyan);
    m_colorSelectionGroup.addButton(ui->btnBlack, Black);
    m_colorSelectionGroup.addButton(ui->btnWhite, White);
    connect(&m_colorSelectionGroup, &QButtonGroup::idClicked, this, &MainWindow::colorSelctionChanged);

    // Project manager
    m_prj = new ProjectManager(this);

    // Initialize scene, jigs etc
    m_scene = new DesignerGraphicsScene(this);
    m_scene->addJig(new RectangularJig);
    m_scene->addJig(new EllipticalArcJig);
    connect(m_scene, &DesignerGraphicsScene::jigStateChanged, this, &MainWindow::_q_jigStateChanged);
    connect(m_scene, &DesignerGraphicsScene::colorSelectionChanged, this, &MainWindow::_q_colorChanged);
    connect(m_scene, &DesignerGraphicsScene::addedToSelection, this, &MainWindow::_q_addedToSelection);
    connect(m_scene, &DesignerGraphicsScene::batchAddedToSelection, this, &MainWindow::_q_batchAddedToSelection);
    connect(m_scene, &DesignerGraphicsScene::removedFromSelection, this, &MainWindow::_q_removedFromSelection);
    connect(m_scene, &DesignerGraphicsScene::selectionCleared, this, &MainWindow::_q_selectionCleared);

    connect(m_scene, &DesignerGraphicsScene::commitJigEdit, m_prj, &ProjectManager::commitJigEdit);

    // Set up add item menu
    m_menuAddItem = new QMenu(this);
    m_actAddRect = new QAction(tr("&Rectangle"));
//    m_menuAddItem->addAction(m_actAddRect);
    ui->tbAdd->addAction(m_actAddRect);

    // Initialize status bar
    m_currentLayerIndicator = new QLabel;
    ui->statusbar->addWidget(new QWidget, 100); // Spring
    ui->statusbar->addWidget(m_currentLayerIndicator);

    // Add groups item to tree
    for (int i = 9; i >= 0; i--) {
        auto itm = new QTreeWidgetItem();
        itm->setText(0, tr("Layer %1").arg(i));
        itm->setData(0, ItemTreeItemLayerIdRole, i);
        ui->treeItems->addTopLevelItem(itm);
        itm->setExpanded(true);
        itm->setFlags(itm->flags() & ~Qt::ItemIsSelectable);
        m_itemTreeLayerGroupItems.prepend(itm);
    }
    connect(ui->treeItems, &QTreeWidget::currentItemChanged, this, &MainWindow::treeItemSelectionChanged);
    setCurrentLayer(0); // Set current layer to layer zero
}

void MainWindow::setCurrentLayer(int layer)
{
    m_currentLayer = layer;
    m_currentLayerIndicator->setText(tr("Layer %1").arg(layer));
}

void MainWindow::jigSelectionChanged(int id)
{
    switch ((JigType)id) {
    case RectangularJigType:
        break;
    case EllipticalArcJigType:
        break;
    case CircularJigType:
        break;
    case LinearJigType:
        break;
    case AnchorJigType:
        break;
    case NullJigType:
    default:
        break;
    }
}

void MainWindow::colorSelctionChanged(int id)
{
    foreach (auto i, m_selection) {
        m_prj->setColor(i, (ElementColor)id);
    }
}

void MainWindow::teamColorSelectionChanged(int id)
{
    m_prj->setTeamColor((ElementColor)id);
}

void MainWindow::treeItemSelectionChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    // Check if it is top level
    if (current->parent() == nullptr) {
        setCurrentLayer(current->data(0, ItemTreeItemLayerIdRole).toInt());
    } else {
        // TODO: do we support groups?
    }
}

void MainWindow::_q_jigStateChanged(int applicableJigs, JigType selectedJig)
{
    ui->btnRect->setEnabled(applicableJigs & RectangularJigType);
    ui->btnArc->setEnabled(applicableJigs & EllipticalArcJigType);
    ui->btnCircle->setEnabled(applicableJigs & CircularJigType);
    ui->btnLine->setEnabled(applicableJigs & LinearJigType);
    ui->btnAnchor->setEnabled(applicableJigs & AnchorJigType);

    if (selectedJig != JigType::NullJigType)
        m_jigSelectionGroup.button(int(selectedJig))->setChecked(true);
}

void MainWindow::_q_colorChanged(ElementColor color)
{
    ui->grpColor->setEnabled(color != InvalidColor);

    if (color < Team || color > MaxColorForSelection) return;

    if (color == UnknownColor) {
        auto btn = m_colorSelectionGroup.checkedButton();
        if (btn)
            btn->setChecked(false);
    } else {
        m_colorSelectionGroup.button(color)->setChecked(true);
    }
}

void MainWindow::_q_addedToSelection(size_t uid)
{
    auto entry = m_prj->getItemEntry(uid);
    if (entry && entry->item)
        entry->item->setSelected(true);
    m_selection.insert(uid);
}

void MainWindow::_q_batchAddedToSelection(QList<size_t> uids)
{
    foreach(auto i, uids) {
        auto entry = m_prj->getItemEntry(i);
        if (entry && entry->item)
            entry->item->setSelected(true);
        m_selection.insert(i);
    }
}

void MainWindow::_q_removedFromSelection(size_t uid)
{
    auto entry = m_prj->getItemEntry(uid);
    if (entry && entry->item)
        entry->item->setSelected(false);
    m_selection.remove(uid);
}

void MainWindow::_q_selectionCleared()
{
    foreach (auto i, m_selection) {
        auto entry = m_prj->getItemEntry(i);
        if (entry && entry->item)
            entry->item->setSelected(false);
    }
    m_selection.clear();
}

void MainWindow::on_tbAdd_triggered(QAction *act)
{
    ProjectManager::ItemEntry *entry = nullptr;

    if (act == m_actAddRect) {
        entry = m_prj->createNewElement(RectangleElementType, m_currentLayer);
    }
    // TODO: all actions...

    if (entry) {
        m_scene->insertElement(entry->element, m_currentLayer);
        m_itemTreeLayerGroupItems[m_currentLayer]->insertChild(0, entry->item);
    }
}


void MainWindow::on_tbAdd_clicked()
{
    ui->tbAdd->showMenu();
}

