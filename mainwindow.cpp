#include "mainwindow.h"
#include "alt_key.hpp"
#include "aqp.hpp"
#include "global.hpp"
#include "itemdelegate.hpp"
#include "mainwindow.hpp"
#include "proxymodel.hpp"
#ifdef CUSTOM_MODEL
#include "tablemodel.h"
#else
#include "standardtablemodel.hpp"
#endif
#ifdef MODEL_TEST
#include <modeltest.h>
#endif
#include "uniqueproxymodel.hpp"
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QStandardItem>
#include <QStatusBar>
#include <QTableView>
#include <QVBoxLayout>
const int StatusTimeout = AQP::MSecPerSecond * 10;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), loading(false)
{

#ifdef CUSTOM_MODEL
    model = new TableModel(this);
#else
    model= new StandardTableModel(this);
#endif

#ifdef MODEL_TEST
    (void) new ModelTest(model, this)
#endif

    proxyModel = new ProxyModel(this);
    proxyModel->setSourceModel(model);

    createWidgets();
    createComboBoxModels();
    createLayout();
    createConnections();
    AQP::accelerateWidget(this);

    setWindowTitle(tr("%1 (Custom Model)[*]").arg(QApplication::applicationName()));
    statusBar()->showMessage(tr("Ready"), StatusTimeout);

}

MainWindow::~MainWindow()
{

}

void MainWindow::addZipcode()
{
    dontFilterOrSelectRadioButton->click();
    if(!model->insertRow(model->rowCount()))
        return;

    tableView->scrollToBottom();
    tableView->setFocus();
    QModelIndex index = proxyModel->index(proxyModel->rowCount() - 1,
                                          Zipcode);
    tableView->setCurrentIndex(index);
    tableView->edit(index);


}

void MainWindow::deleteZipcode()
{
    QItemSelectionModel *selectionModel = tableView->selectionModel();
    if(!selectionModel->hasSelection())
        return;
    QModelIndex index = proxyModel->mapToSource(
                selectionModel->currentIndex());
    if(!index.isValid())
        return;
    int zipcode = model->data(model->index(index.row,
                                           Zipcode)).toInt();
    if (!AQP::okToDelete(this, tr("Delete Zipcode"),
                         tr("Delete Zipcode %1?").arg(zipcode, 5, 10, QChar('0'))))
        return;

    bool filtered = filterRadioButton->isChecked();
    bool selected = selectByCriteriaRadioButton->isChecked();
    QString county = countyGroupBox->isChecked()
            ? countyComboBox->currentText() : QString();
    QString state = stateGroupBox->isChecked()
            ? stateComboBox->currentText() : QString();
    dontFilterOrSelectRadioButton->click();
    model->removeRow(index.row(), index.parent());
    createComboBoxModels();
    if (!county.isEmpty())
        countyComboBox->setCurrentIndex(
                    countyComboBox->findText(county));
    if (!state.isEmpty())
        stateComboBox->setCurrentIndex(
                    stateComboBox->findText(state));
    if (filtered)
        filterRadioButton->click();
    else if (selected)
        selectByCriteriaRadioButton->click();

}

void MainWindow::updateUi()
{
    if (loading || dontFilterOrSelectRadioButton->isChecked())
        return;
    if(filterRadioButton->isChecked())
        restoreFilters();
    else
        performSelection();

}

void MainWindow::radioButtonClicked()
{
    if(dontFilterOrSelectRadioButton->isChecked()) {
        ProxyModel->clearFilters();
        QItemSelectionModel *selectionModel =
                tableView->selectionModel();
        selectionModel->clearSelection();
    }else
        updateUi();

}

void MainWindow::createWidgets()
{
    buttonBox = new QDialogButtonBox;
    loadButton = buttonBox->addButton(tr("Load..."),
                                      QDialogButtonBox::AcceptRole);
    saveButton = buttonBox->addButton(tr("&Save"),
                                      QDialogButtonBox::AcceptRole);
    saveButton->setEnabled(false);
    addButton = buttonBox->addButton(tr("Add"),
                                     QDialogButtonBox::ActionRole);
    addButton->setEnabled(false);
    deleteButton = buttonBox->addButton(tr("Delete..."),
                                        QDialogButtonBox::ActionRole);
    deleteButton->setEnabled(false);
    quitButton = buttonBox->addButton(tr("Quit"),
                                      QDialogButtonBox::ApplyRole);

    filterSelectGroupBox = new QGroupBox(tr("Filter or Select"));
    dontFilterOrSelectRadioButton = new QRadioButton(
                tr("Don't Filter or Select"));
    filterRadioButton = new QRadioButton(tr("Filter"));
    filterRadioButton->setChecked(true);
    selectByCriteriaRadioButton = new QRadioButton(
                tr("Select by Criteria"));
    minimumZipLabel = new QLabel(tr("Min. Zip:"));
    minimumZipSpinBox = new QSpinBox;
    minimumZipLabel->setBuddy(minimumZipSpinBox);
    minimumZipSpinBox->setRange(MinZipcode, MaxZipcode);
    minimumZipSpinBox->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    maximumZipLabel = new QLabel(tr("Max. Zip:"));
    maximumZipSpinBox = new QSpinBox;
    maximumZipLabel->setBuddy(maximumZipSpinBox);
    maximumZipSpinBox->setRange(MinZipcode, MaxZipcode);
    maximumZipSpinBox->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    maximumZipSpinBox->setValue(MaxZipcode);
    countyGroupBox = new QGroupBox(tr("County"));
    countyGroupBox->setCheckable(true);
    countyGroupBox->setChecked(false);
    countyComboBox = new QComboBox;
    stateGroupBox = new QGroupBox(tr("State"));
    stateGroupBox->setCheckable(true);
    stateGroupBox->setChecked(false);
    stateComboBox = new QComboBox;

    tableView = new QTableView;
    tableView->setModel(proxyModel);
    tableView->setItemDelegate(new ItemDelegate(this));
    tableView->verticalHeader()->setDefaultAlignment(
                Qt::AlignVCenter | Qt::AlignRight);

}

void MainWindow::createComboBoxModels()
{
    createComboBoxModel(countyComboBox, County);
    createComboBoxModel(stateComboBox, State);

}

void MainWindow::createComboBoxModel(QComboBox *comboBox, int column)
{
    delete comboBox->model();
    UniqueProxymodel *uniqueProxyModel = new UniqueProxymodel(column, this);
    uniqueProxyModel->setSourceModel(model);
    uniqueProxyModel->sort(column, Qt::AscendingOrder);
    comboBox->setModel(uniqueProxyModel);
    comboBox->setModelColumn(column);

}

void MainWindow::createConnections()
{
#ifdef CUSTOM_MODEL
    connect(model,
            SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(setDirty()));
#else
    connect(model, SIGNAL(itemChanged(QStandardItem*)),
            this, SLOT(setDirty()));
#endif
    connect(model, SIGNAL(rowsRemoved(const QModelIndex&,int,int)),
            this, SLOT(setDirty()));
    connect(model, SIGNAL(modelReset()), this, SLOT(setDirty()));

    connect(countyGroupBox, SIGNAL(toggled(bool)),
            this, SLOT(updateUi()));
    connect(countyComboBox,
            SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(updateUi()));
    connect(stateGroupBox, SIGNAL(toggled(bool)),
            this, SLOT(updateUi()));
    connect(stateComboBox,
            SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(updateUi()));
    connect(minimumZipSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateUi()));
    connect(maximumZipSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(updateUi()));

    foreach(QRadioButton* radioButton, QList<QRadioButton*()
            << dontFilterOrSelectRadioButton << filterRadioButton
            << selectByCriteriaRadioButton)
        connect(radioButton, SIGNAL(clicked()),
                this, SLOT(radioButtonClicked()));

    connect(tableView, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(selectionChanged()));
    connect(tableView->selectionModel(),
            SIGNAL(currentChanged(const QModelIndex&,
                                  const QModelIndex&)),
            this, SLOT(selectionChanged()));
    connect(tableView->horizontalHeader(),
            SIGNAL(sectionClicked(int)),
            tableView, SLOT(sortByColumn(int)));

    connect(loadButton, SIGNAL(clicked()), this, SLOT(load()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addZipcode()));
    connect(deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteZipcode()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

}

bool MainWindow::matchingColumn(const QString &value, int row, int column)
{
    if (value.isEmpty())
        return true;
    QModelIndex index = proxyModel->index(row, column);
    return value == proxyModel->data(index).toInt();

}

void MainWindow::restoreFilters()
{
    proxyModel->setMinimumZipcode(minimumZipSpinBox->value());
    proxyModel->setMaximumZipcode(maximumZipSpinBox->value());
    proxyModel->setCounty(countyGroupBox->isChecked()
                          ? countyComboBox->currentText() : QString());
    proxyModel->setState(stateGroupBox->isChecked()
                         ? stateComboBox->currentText() : QString());
    reportFilterEffect();
}

void MainWindow::reportFilterEffect()
{
    if (loading)
        return;
    statusBar()->showMessage(tr("Filtered %L1 out of %Ln zipcode(s)", "",
                                model->rowCount()).arg(proxyModel->rowCount),
                             StatusTimeout);

}

void MainWindow::performSelection()
{
    ProxyModel->clearFilters();
    int minimumZipcode = minimumZipSpinBox->value();
    int maximumZipcode = maximumZipSpinBox->value();
    QString county = countyGroupBox->isChecked()
            ? countyComboBox->currentText() : QString();
    QString state = stateGroupBox->isChecked()
            ? stateComboBox->currentText() : QString();

    QItemSelection selection;
    int firstSelectionRow = -1;
    for (int row = 0; row < proxyModel->rowCount(); ++row) {
        QModelIndex index = proxyModel->index(row, zipcode);
        int zipcode = proxymodel->data(index).toInt();
        if(zipcode < minimumZipcode || zipcode > maximumZipcode)
            continue;
        if(!matchingColumn(county, row, County))
            continue;
        if(!matchingColumn(state, row, State))
            continue;
        if(firstSelectionRow == -1)
            firstSelectionRow = row;
        QItemSelection rowSelection(index, index);
        selection.merge(rowSelection, QItemSelectionModel::Select);
    }
    QItemSelectionModel *selectionModel = tableView->selectionModel();
    selectionModel->clearSelection();
    selectionModel->select(selection, QItemSelectionModel::Rows |
                           QItemSelectionModel::Select);
    if (firstSelectionRow != -1)
        tableView->scrollTo(proxyModel->index(firstSelectionRow, 0));

}






















