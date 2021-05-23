#pragma once

#ifndef NODE_H_
#define NODE_H_

#include <QString>

#include <QList>
#include <QVector>
#include <QModelIndex>
#include <QObject>
#include <stdint.h>

/*! \brief Базовый класс для всех узлов
 * обеспечивает связь родитель-потомок между элементами
*/

class Node
{
public:
    Node();
    //! @enum тип узла
    enum Type{E_NONE       ,  /*!< Enum тип узла не указан */
              E_STRUCT      , /*!< Enum тип узла "Структура" */
              E_PARAM       , /*!< Enum тип узла "Параметр" */
              E_PM          , /*!< Enum тип узла "Программный модуль */
              E_SIO         , /*!< Enum тип узла Разновидность параметров "Настроечные/Входные/Выходные*/
              E_UNION       , /*!< Enum тип узла "Объедиение" */
              E_VARIANTSTATE, /*!< Enum тип узла "Контейнер */
              E_VARIANTPM   , /*!< Enum тип узла "Вариант программного модуля */
              E_GROUP         /*!< Enum тип узла "Группа" */
             };
    //! enum типа дерева
    enum TypeTree{//тип дерева
        E_TR_NONE       , /*!< Enum тип дерева не указан */
        E_TR_VARIANT    , /*!< Enum тип дерево вариантов  */
        E_TR_DATA         /*!< Enum тип дерево данных  */
    };

    //! текущий тип дерева
    TypeTree typeTree;
    QString displayName;//имя которое будет отображаться пользователю
    //! имя в протоколе информационного обмена
    QString picName;
    //! имя идентификатор
    QString idName;
    //! внутреннее имя
    QString internalName;
    //! имя пути к данному узлу
    QString pathName;
    //! Признак отображать пользователю данный узел или нет(true - )
    bool isDisplay;

    //! найти видимый узел по порядковому номеру
    Node * findDispayNode(int index)
    {
        int i = -1;
        for(auto node : child)
        {
            if(node->isDisplay == true)
                i++;
            if(index == i)
                return node;
        }

        return nullptr;
    }
    int sizeDisplayNodes()
    {
        int size = 0;
        for(auto node : child)
        {
            if(node->isDisplay == true)
                size ++;
        }
        return size;
    }
    //! добавить родителя
    void addParent(Node* p)
    {
        parent   = p;
    }
    //! обновить пути
    void updatePath()
    {
        for(auto node : child)
        {
            //if(this->pathName.isEmpty() == false)
            node->pathName = this->pathName+"." + node->idName;
            node->updatePath();
        }
    }
    //! добавить потомка
    virtual void addChild(Node* ch)
    {
        ch->parent = this;
        if(child.isEmpty() == false)
        {
            ch->left        = child.last();
            ch->left->right = ch;
        }

        child.push_back(ch);
        if(this->pathName.isEmpty() == false)
            ch->pathName = this->pathName+"." + ch->pathName;
    }
    void addModelIndex(QModelIndex i, int column)
    {
        if(column>=index.size())
        {
            index.resize(column+1);
        }
        index[column]=i;
    }
    QModelIndex modelIndexByCol(int column)
    {
        return index[column];
    }

    //! расчет смещений
    virtual void calcOffset()
    {
        for(auto node : child)
        {
            node->calcOffset();
        }
    }
    Node *parent;//родителький элемент

    Node *left; //элемент левее
    Node *right;//элемент правее

    Node* leftNode()// вернуть узел слева
    {
        return left;
    }
    Node* rightNode()//вернуть узел справа
    {
        return right;
    }

    QList<Node* > child;// дочерние элементы
    QVector<QModelIndex> index;

    virtual int type()const{return E_NONE;}
    virtual ~Node() = default;
    //////////Нужно перенести в GenericNode
    //! адрес памяти по которому лежит структура относительно начала
    unsigned offset;
    //! выравнивание указанным кол-вом байтов
    unsigned alignBytes;
     //! общее кол-во элементов байтов, которое занимает этот элемент с потомками
    unsigned bytes;
    //! прямой адрес в памяти
    unsigned directAddr;

};

#endif /* NODE_H_ */
