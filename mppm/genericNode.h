#pragma once

#ifndef GENERICNODE_H_
#define GENERICNODE_H_

#include "node.h"
#include "variantNode.h"

/*! \brief Базовый класс структуру данных
*/

class GenericNode:public Node
{
public:
    GenericNode();
    //! комментарий к варианту
    QString comment;
    //! адрес для ДПК
    QString addr;
    //! ед. измерения
    QString messure;

    //! добавить кол-во байт
    virtual void addBytes(uint b);

    //! соединить дерево
    virtual void joinTree(Node*);

    void setZeroVariantNode()
    {
        variantNode = nullptr;
        for(auto i:child)
        {
            GenericNode * node = static_cast<GenericNode* > (i);
            node->setZeroVariantNode();
        }
    }
    //! ссылка на вариант
    Node *variantNode;
    //! признак единого адресного пространства
    virtual bool uniformAddr(){return false;}
    virtual ~GenericNode() = default;
};

#endif /* GENERICNODE_H_ */
