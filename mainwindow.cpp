#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PrefixTree.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->m_PrefixTree = new PrefixTree<QVector<QString>,QVector<QString>>;

    count = 0;
    currentItem = NULL;
    currentColumn = 0;

    ui->treeWidget->setColumnCount(1);
    QStringList headers;
    headers << "Термины" << "Тип";
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->setSortingEnabled(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked() {//кнопка Добавить

    if (ui->treeWidget->currentItem()) {
        QString word = ui->lineEdit->text();
        QString type = ui->lineEdit_4->text();
        InsertItem  (currentItem, word+ " " + QString("%1").arg(++count));
        InsertItem  (currentItem+1, type+ " " + QString("%1").arg(++count));
    }
    else {
        QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->treeWidget, ui->treeWidget->currentItem());
        QString word = ui->lineEdit->text();
        QString type = ui->lineEdit_4->text();
        newItem->setText (currentColumn, word);
        newItem->setText (currentColumn+1, type);
        newItem->setExpanded(true);
    }
    currentItem = ui->treeWidget->currentItem();
    showAll();

}

void MainWindow::on_deleteButton_clicked() {//кнопка Удалить
    if (currentItem) {
     DeleteItem (currentItem);
     currentItem = NULL;
    }
    showAll();
}

void MainWindow::on_deleteAllButton_clicked() { // кнопка удалить все
    m_PrefixTree->~PrefixTree();
}

void MainWindow::on_addFromFileButton_clicked() { // кнопка загрузить дерево из файла
    m_PrefixTree->readTreeIntoFail(ui->lineEdit_2->text().toStdString());
}

void MainWindow::on_saveToFileButton_clicked() { // кнопка сохранить в файл
    m_PrefixTree->saveTreeToFail(ui->lineEdit_3->text().toStdString());
}


int MainWindow::treeCount(QTreeWidget *tree, QTreeWidgetItem *parent = 0) {
    tree->expandAll();
    int count = 0;
    if (parent == 0) {
        int topCount = tree->topLevelItemCount();
        for (int i = 0; i < topCount; i++) {
            QTreeWidgetItem *item = tree->topLevelItem(i);
            if (item->isExpanded()) {
                count += treeCount(tree, item);
            }
        }
        count += topCount;
    }
    else {
        int childCount = parent->childCount();
        for (int i = 0; i < childCount; i++) {
            QTreeWidgetItem *item = parent->child(i);
            if (item->isExpanded()) {
                count += treeCount(tree, item);
            }
        }
        count += childCount;
    }
    return count;
}

void MainWindow::InsertItem (QTreeWidgetItem *parent, QString text) {
     if (parent->isExpanded()==false)
         parent->setExpanded(true);
     QTreeWidgetItem *newItem = new QTreeWidgetItem(parent, ui->treeWidget->currentItem());
     newItem->setText (currentColumn, text);
     newItem->setExpanded(true);
}

void MainWindow::showAll(void) {
     int cnt = treeCount (ui->treeWidget);
     QString str(tr("Всего элементов в словаре: ")+QString("%1").arg(cnt));
     setWindowTitle(str);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column) {
     currentItem = item;
     currentColumn = column;
}

void MainWindow::DeleteItem (QTreeWidgetItem *currentItem) {
     QTreeWidgetItem *parent = currentItem->parent();
     int index;
     if (parent) {
        index = parent->indexOfChild(ui->treeWidget->currentItem());
        delete parent->takeChild(index);
     }
     else {
        index = ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem());
        delete ui->treeWidget->takeTopLevelItem(index);
    }
}
