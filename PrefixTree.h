#ifndef PREFIXTREE_H
#define PREFIXTREE_H

#include <QFile>
#include <QString>
#include <QDataStream>
#include <QVector>
#include <map>
#include <iostream>

template <class K, class V>
class PrefixTree {
    public:
        PrefixTree(); // конструктор
        PrefixTree(const PrefixTree& other_); // конструктор копирования
        bool operator==(const PrefixTree& other_); // оператор == (принимает другую коллекцию, возвращает истину или ложь в зависимости от пар ключей)
        V& operator[](const K& key) const; // оператор [](принимает ключ и возвращает его значение по ссылке)

        void addKeyAndValue(const K& key, const V& value); // метод, добавляющий в коллекцию заданную пару ключ-значение
        void delKeyAndValue(const K& key); // метод, удаляющий из коллекции пару ключ-значение по произвольному ключу

        bool isKey(const K& key); // метод, принимающий ключ К и возвращающий при его наличии в коллекции истинну

        QVector<K> getKeys(const K& key) const; // метод, возращающий список хранящихся ключей К, начинающихся с заданной последовательности
        int getCountValues() const; // метод, возвращающий количество хранящихся в коллекции значений
        int getCountKeys() const; // метод, возвращающий количество хранящихся в коллекции ключей
        int getMaxLength() const; // метод, возвращающий максимальную длину хранящихся в коллекции ключей

        void saveTreeToFail(const std::string& filename) const; // метод, принимающий путь к файлу и загружающий в него
        void readTreeIntoFail(const std::string& filename); // метод, принимающий путь к файлу и читающий его

        void deleteAllValues(); // метод, удаляющий все хранящиеся значения из коллекции

        ~PrefixTree(); // деструктор

    protected:

    private:
        class node { //Узел префиксного дерева
            public:
                node() {
                    isValue = false;
                    m_parent = nullptr;
                }

                void add(const K k_) {
                    items[k_] = new node;
                    items[k_]->m_parent = this;
                }

                ~node() {
                    typename std::map<K, node*>::iterator i, end;
                    i = items.begin();
                    end = items.end();
                    while (i != end) {
                        delete i->second;
                        i++;
                    }
                }
            V m_value;
            bool isValue = false;
            node* m_parent = nullptr;
            std::map<K, node*> items;
        };

        node* m_root;
        int m_numberKeys; //Количество ключей
        int m_maxLength; //Максимальная длина ключа
        int m_numberNodes; //Количество узлов

        void addKey(typename PrefixTree<K, V>::node* root, K* key, const int i, QVector<K>* vect) const;
};

template<class K, class V>
void PrefixTree<K, V>::addKey(typename PrefixTree<K, V>::node* root,
                              K* key, const int i, QVector<K>* vect) const {
    //Добавление ключа в вектор
    if (root->isValue)
    {
        vect->push_back(*key);
    }
    typename std::map<K, typename PrefixTree<K, V>::node*>::iterator it, end;
    it = root->items.begin();
    end = root->items.end();
    while (it != end)
    {
        key->push_back((it->first)[0]);
        addKey(it->second, key, i+1, vect);
        key->pop_back();
        it++;
    }
}

template <class K, class V>
PrefixTree<K, V>::PrefixTree() { //Конструктор по умолчанию
    m_root = new node;
    m_numberKeys = 0;
    m_maxLength = 0;
    m_numberNodes = 0;
}

template <class K, class V>
PrefixTree<K, V>::PrefixTree(const PrefixTree& other_) { //Конструктор копирования
    K newKey;
    m_root = new node;
    m_numberKeys = 0;
    m_maxLength = 0;
    m_numberNodes = 0;
    QVector<K> vect = other_.getKeys(newKey);
    int i = 0;

    while (i < static_cast<int>(vect.size())) {
        addKeyAndValue(vect[i], other_[vect[i]]);
        i++;
    }
}

template <class K, class V>
bool PrefixTree<K, V>::operator==(const PrefixTree& other_) {	//Перегрузка оператора ==
    K newKey;
    QVector<K> KeyVect1 = getKeys(newKey);
    QVector<K> KeyVect2 = other_.getKeys(newKey);
    int i = 0;

    while (i < static_cast<int>(KeyVect1.size())) {
        if ((*this)[KeyVect1[i]] != other_[KeyVect2[i]]){
            return false;
        }
        i++;
    }

    return true;
}

template <class K, class V>
V& PrefixTree<K, V>::operator[](const K& key) const { // Перегрузка оператора []
    int i=0;
    node* p = m_root;
    while (i < static_cast<int>(key.size()))
    {
        std::map<K, node*> dict = p->items;
        K newKey = {key[i]};
        if (p->items.count(newKey))
            p = dict[newKey];
        /*else
            throw std::exception("Key error...\n");*/
        i++;
    }
    if (p->isValue)
        return p->m_value;
    /*else
        throw std::exception("Key error...\n");*/
}

template <class K, class V>
void PrefixTree<K, V>::addKeyAndValue(const K& key, const V& value) {
    // метод, добавляющий в коллекцию заданную пару ключ-значение
    node* p = m_root;
    int i=0;
    while (i< static_cast<int>(key.size()))
    {
        K newKey = {key[i]};
        if (p->items.count(newKey) == 0) {
            p->add(newKey);
            m_numberNodes++;
        }
        p = p->items.at(newKey);
        i++;
    }
    if (p->isValue)
        m_numberKeys--;
    p->m_value = value;
    p->isValue = true;
    if (m_maxLength < i)
        m_maxLength = i;
    m_numberKeys++;
}

template <class K, class V>
void PrefixTree<K, V>::delKeyAndValue(const K& key) {
// метод, удаляющий из коллекции пару ключ-значение по произвольному ключу
    if (isKey(key))
    {
        int i=0;
        node* p = m_root;
        while (i< static_cast<int>(key.size())) {
            std::map<K, node*> dict = p->items;
            K newKey = {key[i]};
            p = dict[newKey];
            i++;
        }
        i--;

        if (p->items.size() != 0) {
            p->isValue = false;
            m_numberKeys--;
            return;
        }

        bool flag = true;
        while (p->m_parent&&flag) {
            if ((p->m_parent->items.size() == 1) && (p->m_parent->isValue == 0)) {
                p = p->m_parent;
                i--;
                m_numberNodes--;
            }
            else
                flag=false;
        }

        if (p->m_parent) {
            K newKey = {key[i]};
            p->m_parent->items.erase(newKey);
            delete p;
            m_numberNodes--;
        }
        else {
            delete p;
            m_root = new node;
        }
        m_numberKeys--;
    }
    else
        throw std::exception("Key error...");
}

template <class K, class V>
bool PrefixTree<K, V>::isKey(const K& key) {
    // метод, принимающий ключ К и возвращающий при его наличии в коллекции истинну

    bool flag = false;
    int i=0;
    node* p = m_root;
    while (i<(int)key.size())
    {
        std::map<K, node*> dict = p->items;
        K newKey = {key[i]};
        if (p->items.count(newKey))
            p = dict[newKey];
        else
            return flag;
        i++;
    }
    flag=true;
    return true;
}

template<class K, class V>
QVector<K> PrefixTree<K, V>::getKeys(const K &key) const {
// метод, возращающий список хранящихся ключей К, начинающихся с заданной последовательности
    QVector<K> vect;
    int i=0;

    typename PrefixTree<K, V>::node* p = m_root;
    K* keyItem = new K;
    while (i< static_cast<int>(key.size()))
    {
        std::map<K, typename PrefixTree<K, V>::node*> dict = p->items;
        K newKey = {key[i]};
        if (p->items.count(newKey)) {
            p = dict[newKey];
            keyItem->push_back(key[i]);
        }
        else {
            return vect;}
        i++;
    }
    addKey(p, keyItem, i, &vect);
    delete keyItem;

    return vect;
}

template <class K, class V>
int PrefixTree<K, V>::getCountKeys() const {
    return m_numberKeys;
}

template <class K, class V>
int PrefixTree<K, V>::getCountValues() const {
    return m_numberNodes;
}

template <class K, class V>
int PrefixTree<K, V>::getMaxLength() const {
    return m_maxLength;
}

template<class K, class V>
void PrefixTree<K, V>::saveTreeToFail(const std::string& filename) const {
    //Сохранение коллекции в файл
    QString filePath = QString::fromStdString(filename);

    QFile file(filePath);
    if (file.open(QFile::WriteOnly))
    {
        K newKey;
        QVector<K> key = getKeys(newKey);
        int i = 0;
        QDataStream out(&file);
        out << key;
        QVector<V> values;
        while (i < static_cast<int>(key.size()))
        {
            values.push_back((*this)[key[i]]);
            i++;
        }
        out << values;
        file.close();
    }
}

template<class K, class V>
void PrefixTree<K, V>::readTreeIntoFail(const std::string& filename)
{
    //Загрузка префиксного дерева из файла
    QString filePath = QString::fromStdString(filename);
    QFile file(filePath);

    if (file.open(QFile::ReadOnly)) {
        QDataStream in(&file);

        deleteAllValues();
        QVector<K> keys;
        in >> keys;
        QVector<V> values;
        in >> values;

        for (int i = 0; i< static_cast<int>(keys.size()); i++)
            addKeyAndValue(keys[i], values[i]);
        file.close();
    }

}

template <class K, class V>
void PrefixTree<K, V>::deleteAllValues() {
    // метод, удаляющий все хранящиеся значения из коллекции
    delete m_root;
    m_numberNodes = 0;
    m_numberKeys = 0;
    m_maxLength = 0;
    m_root = new node;

}

template <class K, class V>
PrefixTree<K, V>::~PrefixTree() { //Деструктор
    delete m_root;
}

#endif // PREFIXTREE_H
