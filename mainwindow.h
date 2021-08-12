#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QComboBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QTableView;
#ifdef CUSTOM_MODEL
    class TableModel;
#else
class StandardTableModel;
#endif
class ProxyModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void load();
    void load(const QString &filename);
    bool save();
    void addZipcode();
    void deleteZipcode();
    void setDirty() { setWindowModified(true);}
    void updateUi();
    void radioButtonClicked();
    void selectionChanged();

private:
    QDialogButtonBox *buttonBox;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *quitButton;
    QGroupBox *filterSelectGroupBox;
    QRadioButton *dontFilterOrSelectRadioButton;
    QRadioButton *filterRadioButton;
    QRadioButton *selectByCriteriaRadioButton;
    QLabel *minimumZipLabel;
    QSpinBox *minimumZipSpinBox;
    QLabel *maximumZipLabel;
    QSpinBox *maximumZipSpinBox;
    QGroupBox *countyGroupBox;
    QComboBox *countyComboBox;
    QGroupBox *stateGroupBox;
    QComboBox *stateComboBox;
    QTableView *tableView;
    bool loading;

    void createWidgets();
    void createComboBoxModels();
    void createComboBoxModel(QComboBox *comboBox, int column);
    void createLayout();
    void createConnections();
    bool okToClearData();
    void enableButtons(bool enable=true);
    bool matchingColumn(const QString &value, int row, int column);
    void restoreFilters();
    void reportFilterEffect();
    void performSelection();
};
#endif // MAINWINDOW_H




























