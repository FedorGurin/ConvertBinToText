#ifndef NODE_H_
#define NODE_H_

#include <QString>
#include <QDomElement>
#include <QList>

class Node
{
public:
    Node();
    //! тип узла
    enum Type{EMPTY,
              STRUCT,
              PARAM};

    //! имя которое будет отображаться пользователю
    QString displayName;
    //! имя в протоколе информационного обмена
    QString picName;
    //! имя идентификатор
    QString idName;
    //! внутреннее имя
    QString internalName;
    //! имя пути к данному узлу
    QString pathName;
    //! комментарий
    QString comment;

    //! добавить кол-во байт
    virtual void addBytes(char b);

    //! добавить родителя
    void addParent(Node* p)
    {
        parent=p;
        pathName=p->pathName;
    }
    //! добавить потомка
    void addChild(Node* ch)
    {
        ch->parent=this;
        if(child.isEmpty()==false)
        {
            ch->left=child.last();
            ch->left->right=ch;
        }

        child.push_back(ch);
        ch->pathName=this->pathName+"/"+this->displayName;
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

    //! дочерние элементы
    QList<Node* > child;


    virtual int type()const{return EMPTY;}
    virtual ~Node();
    //////////Нужно перенести в GenericNode
    //! адрес памяти по которому лежит структура относительно начала
    quint32 offset;
    //! выравнивание указанным кол-вом байтов
    quint32 alignBytes;
     //! общее кол-во элементов байтов, которое занимает этот элемент с потомками
    quint32 bytes;
    //! прямой адрес в памяти
    quint32 directAddr;

};

#endif /* NODE_H_ */
