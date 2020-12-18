/*Юных бухгалтеров отправляют на курсы повышения квалификации для обучения языку программирования 1С.
 Для облегчения работы им нужен электронный справочник, позволяющий записывать K и соответствующие V,
 а также предлагающий функцию автодополнения К при его вводе.
  1) Коллекция: префиксное дерево <K,V> с методом, возвращающим список хранящихся ключей К,
     начинающихся с заданной последовательности
  2) K = слово, т.е. последовательность букв (строка);
     V = тип (перечисление: служебное слово/функция/константа)*/
#include "mainwindow.h"
#include "PrefixTree.h"

#include <QApplication>
#include <iostream>
#include <QVector>
#include <assert.h>

void testDelete() {
//Тестирование удаления
    PrefixTree<QVector<QString>, QVector<QString>> tree;
    QVector<QString> keysVect1 = {"Function1", "Service word 1", "Constant1"};
    QVector<QString> valuesVect = {"ServiceWord", "Function", "Constant"};
    tree.addKeyAndValue(keysVect1, valuesVect);
    assert(tree.getCountKeys() == 1);
    assert(tree.getCountValues() == 3);
    tree.delKeyAndValue(keysVect1);
    assert(tree.getCountKeys() == 0);
}

void testDeleteOne() {
//Тестирование удаления единственного ключа
    PrefixTree<QVector<QString>, QVector<QString>> tree;
    QVector<QString> keysVect1 = {"Function1", "Service word 1", "Constant1"};
    QVector<QString> valuesVect = {"ServiceWord", "Function", "Constant"};
    tree.addKeyAndValue(keysVect1, valuesVect);
    tree.delKeyAndValue(keysVect1);
    assert(!tree.isKey(keysVect1));
    try {
        tree[keysVect1];
    } catch (std::out_of_range) {
        assert(true);
    }
    assert(tree.getCountValues() == 0);
}

void testDeletePartDelete() {
    //Тестирование частичного удаления
    PrefixTree<QVector<QString>, QVector<QString>> tree;
    QVector<QString> keysVect1 = {"Function1", "Service word 1", "Constant1"};
    QVector<QString> keysVect2 = {"Constant2", "Function2", "Service word 2"};
    QVector<QString> valuesVect = {"ServiceWord", "Function", "Constant"};
    tree.addKeyAndValue(keysVect1, valuesVect);
    tree.addKeyAndValue(keysVect2, valuesVect);
    tree.delKeyAndValue(keysVect1);
    assert(tree.isKey(keysVect2)==true);
}

void testDeleteNoDelete() {
//Тестирование невозможности удаления
    PrefixTree<QVector<QString>, QVector<QString>> tree;
    QVector<QString> keysVect1 = {"Function1", "Service word 1", "Constant1", "Constant3"};
    QVector<QString> keysVect2 = {"Constant2", "Function2", "Service word 2"};
    QVector<QString> valuesVect = {"ServiceWord", "Function", "Constant"};
    tree.addKeyAndValue(keysVect1, valuesVect);
    tree.addKeyAndValue(keysVect2, valuesVect);
    tree.delKeyAndValue(keysVect2);
    assert(!tree.isKey(keysVect2));
    assert(tree.isKey(keysVect1));
    assert(tree.getCountValues() == 4);
}

void testAddOne() {
//Тестирование добавления одного ключа
    PrefixTree<QVector<QString>, QVector<QString>> tree;
    QVector<QString> keysVect1 = {"Function1", "Service word 1", "Constant1"};
    QVector<QString> valuesVect = {"ServiceWord", "Function", "Constant"};
    assert(tree.getCountKeys() == 0);
    tree.addKeyAndValue(keysVect1, valuesVect);
    assert(tree.getCountKeys() == 1);
    tree.addKeyAndValue(keysVect1, valuesVect);
    assert(tree.getCountKeys() == 1);
    assert(tree.isKey(keysVect1));
    assert(tree[keysVect1] == valuesVect);
    assert(tree.getCountValues() == 3);
}


void testDeleteTree() {
//Тестирование полного удаления
    PrefixTree<QVector<QString>, QVector<QString>> tree1;
    PrefixTree<QVector<QString>, QVector<QString>> tree2;
    QVector<QString> keysVect1 = {"Function1", "Service word 1", "Constant1", "Constant3"};
    QVector<QString> keysVect2 = {"Constant2", "Function2", "Service word 2"};
    QVector<QString> valuesVect = {"ServiceWord", "Function", "Constant"};
    tree1.addKeyAndValue(keysVect1, valuesVect);
    tree1.addKeyAndValue(keysVect2, valuesVect);
    tree1.deleteAllValues();
    assert(tree1 == tree2);
};

void testCopyConstructor() {
//Тестирование конструктора копирования
    PrefixTree<QVector<QString>, QVector<QString>> tree;
    QVector<QString> keysVect1 = {"Function1", "Service word 1", "Constant1", "Constant3"};
    QVector<QString> keysVect2 = {"Constant2", "Function2", "Service word 2"};
    QVector<QString> valuesVect = {"ServiceWord", "Function", "Constant"};
    tree.addKeyAndValue(keysVect1, valuesVect);
    tree.addKeyAndValue(keysVect2, valuesVect);
    PrefixTree<QVector<QString>, QVector<QString>> copytree(tree);
    assert(copytree == tree);
}

void testPrefixTreeSaveToFile() {
//Тестирование записи в файл
    PrefixTree<QVector<QString>, QVector<QString>> tree;
    QVector<QString> keysVect1 = {"Function1", "Service word 1", "Constant1", "Constant3"};
    QVector<QString> keysVect2 = {"Constant2", "Function2", "Service word 2"};
    QVector<QString> valuesVect = {"ServiceWord", "Function", "Constant"};
    tree.addKeyAndValue(keysVect1, valuesVect);
    tree.addKeyAndValue(keysVect2, valuesVect);
    tree.saveTreeToFail("test.lab2");
    PrefixTree<QVector<QString>, QVector<QString>> newTree;
    newTree.readTreeIntoFail("test.lab2");
    assert(tree == newTree);
}

void test() {
//Тестирование
    testAddOne();
    testDelete();
    testDeleteOne();
    testDeletePartDelete();
    testDeleteNoDelete();
    testDeleteTree();
    testCopyConstructor();
    testPrefixTreeSaveToFile();
    std::cout << "All test are passed.." << std::endl;
}

int main(int argc, char *argv[])
{
    //test();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
