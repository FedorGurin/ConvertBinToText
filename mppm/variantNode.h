#pragma once

#ifndef VARIANTNODE_H_
#define VARIANTNODE_H_

#include <QList>
#include <QString>
#include <QDomElement>
#include "node.h"

/*! \brief Класс описывающий вариант программного модуля
 * он предназначен для хранения, загрузки и выгрузку варианта в файл
*/
class VariantNode:public Node{
public:
    VariantNode();
    VariantNode(const QDomElement&, Node*);
    VariantNode(Node* nodeData,QString& nameVariant);
    VariantNode(Node* nodeData,QString& nameVariant,QList<Node* > listNodesToSave);

    void recursCreateVariant(Node* dataNode,VariantNode* variantNode,QString& nameNodeSIO);
    void recursCreateVariant(Node* dataNode,VariantNode* variantNode,QString& nameNodeSIO,Node* nodesToSave);
    Type _type;

    Node *dataNode;//Указатель на узел с данными
    Node *variant;//укзатель на вариант программного модуля
    QList<Node* >variantState;//! ссылки на варианты состояния которые содержат данный вариант

    QString value;
    QString* valueParam;
    bool findDataNode(VariantNode* var/*структуры которую мы создаем*/,Node *data/*узел который нужно найти*/);
    void isHaveCommonParent(QList<Node* > *listNodes);
    void finUpDataNode(VariantNode* var/*структуры которую мы создаем*/,Node *data/*узел который нужно найти*/);

    QString pathToVariant()
    {
        QString tempPath="";
        if(this->child.size()!=0)
        {
            if(this->type()!=Node::E_PARAM)
            {
                tempPath=pathToVariant((VariantNode*)this->child[0]);
            }else tempPath="/";
        }
        return tempPath;
    }
    QString pathToVariant(VariantNode* varNode)
    {
        QString tempPath="";

        if(varNode->child.size()!=0)
        {
            if(this->type()!=Node::E_PARAM)
            {
                tempPath=pathToVariant((VariantNode*)(varNode->child[0]))+varNode->displayName+"/";
            }else tempPath="/";
        }
        return tempPath;
    }

    //! Функция переимнования данного варианта
    void rename(QString name)
    {
        displayName = name;
    }
    //! Функция удаления данного варианта
    void deleteVariant();

    //! Функция удаления данного варианта
    void deleteVariantNode(Node* node)
    {
        child.removeOne(node);
    }
    //! Комментарий к варианту
    QString comment;

    void setPModule()       {_type = E_PM;}// помечаем, что данный узел содержит имя программный модуль
    void setSIO()           {_type = E_SIO;}
    void setParam()         {_type = E_PARAM;}
    void setStruct()        {_type = E_STRUCT;}
    void setVariantPM()     {_type = E_VARIANTPM;}
    void setVariantState()  {_type = E_VARIANTSTATE;}//вариант состояния
    //void setBit()           {_type=BIT;}
    /*! \brief Функция преобразования вариантов узел (рекурсивная)
     * QDomDocument для дальнейшего сохранения в формате xml
     * @param domDocument - ссылка на объект файла XML
     * @param parent
     * */
    void save(QDomDocument &domDocument,QDomElement& parent, VariantNode *parentNode);
    void setVariant();
    //! Обновить значения в варианте
    void refreshVariant();
    int type()const{return _type;}
    virtual ~VariantNode() = default;
};

#endif /* VARIANTNODE_H_ */
