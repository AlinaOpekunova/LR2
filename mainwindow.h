#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include<PrefixTree.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_deleteAllButton_clicked();

    void on_addFromFileButton_clicked();

    void on_saveToFileButton_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;

    PrefixTree<QVector<QString>,QVector<QString>> *m_PrefixTree=nullptr;

    int count; // для хранения номера узла
    QTreeWidgetItem *currentItem; //текущий элемент, запоминается при клике в QTreeWidget
    int currentColumn; //номер столбца

    void InsertItem (QTreeWidgetItem *, QString); //добавление элемента в QTreeWidget
    void showAll(void); //вывод информации о QTreeWidget
    int treeCount(QTreeWidget *, QTreeWidgetItem *); //подсчёт количества элементов в QTreeWidget

    void DeleteItem(QTreeWidgetItem *currentItem);
};
#endif // MAINWINDOW_H
