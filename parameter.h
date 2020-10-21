#ifndef PARAMETER_H_
#define PARAMETER_H_

#include "node.h"
#include <QVariant>
#include <QVector>

class Parameter: public Node
{
public:
    //! тип данных
    enum TypeData{DOUBLE    = 0,
                  INT       = 1,
                  FLOAT     = 2,
                  BOOL      = 3,
                  BIT       = 4,
                  UCHAR     = 5,
                  UINT      = 6,
                  MCHAR     = 7,
                  CHAR      = 8,
                  INT16     = 9,
                  LDOUBLE   = 10};


    Parameter(const QDomElement&, Node*);

    Parameter(const Parameter* nodeNeedCopy,Node *glParent=0 /*глобальный элемент*/);

    //! добавить массив с таким же параметрами
    void addMassiveParam(void);

    //! обновить значение параметра
    void refreshValues(void);

    //! проверка на выравнивание по 4 байтам
    uint checkAlign_4byte();
    //! проверка на выравнивание по 8 байтам
    uint checkAlign_8byte();

    //! параметр слева
    Parameter* leftParam()
    {
        return static_cast<Parameter*>(left);
    }
    //! параметр справа
    Parameter* rightParam()
    {
        return static_cast<Parameter*>(right);
    }
    //! Сколько элементов в случае если массив параметров
    int howElements;
    int startIndex;

    //! ед. измерения
    QString messure;

    //! множитель
    QString factor;

    //! признак того, что для параметра определен множитель
    bool isFactor;
    //! значение множителя
    float factorValue;

    //! условие применения данного параметра
    QString condition;

    //! Указатель на значение варианта
    QString* valueVariant;

    //! значение параметра
    QString value;
    //! значение параметра
    QString rvalue(){return value;}
    //! значение параметра в бинарном виде(с учетом типа)
    char* bin;
    //! заполнение переменной bin
    static char* binData(Parameter* node);
    //! задать бинарные данные
    static void setBinData(char* fromData, Parameter* node);
    //! выравнивание
    static uint alignData(QDataStream &out,Parameter* node);
    static char alignArray[8];


    //! предыдущее значение параметра
    QString valuePred;
    //! параметр только для чтения
    int readOnly;
    //! признак того, что значение данного параметра изменилось с предыдущего шага
    bool modifyValueLastStep;

    //! начальный бит
    quint8 numBit;
    //! конечный бит
    quint8 endBit;

    void setValue(QString name_)
    {
        valuePred=value;
        value=name_;
        if(valuePred.compare(value)!=0) modifyValueLastStep=true;
        else modifyValueLastStep=false;

        //for(int i=0;i<index.size();i++)
        //    index[i].model()->
    }
    //! значение в виде градусы/минуты/секунды
    QString valueFiLamda;
    //! пределы значений
    QVariant predValueMin;
    QVariant predValueMax;

    //! использовать ограничение интервала
    bool restriction;

    //! точность
    int acc;

    //! тип параметра в строке
    QString typeStr;

    //! Тип параметра
    TypeData typeP;

    //! признак наличия списка с перечислениями
    bool hasEnum;

    //! данное поле отображает долготу или широту
    bool fiLamda;

    //! если поле является строкой
    int lengthMCHAR;//! длина строки char

    //! число состоящее из нескольких битиков
    bool complexBit;

    //! структура имя перечисления и значения
    struct ParamEnum
    {
        QString name;
        QString value;
    };

    //! возврат значения по перечислению
    QString valueEnum(QString name_)
    {
        for(int i=0;i<listEnum.size();i++)
        {
            if(listEnum[i].name==name_)
            {
                return listEnum[i].value;
            }
        }
        return listEnum[0].value;
    }
    //! возврат. имени перечисления по значению
    QString nameEnum(QString value_)
    {
        for(int i=0;i<listEnum.size();i++)
        {
            if(listEnum[i].value==value_)
            {
                return listEnum[i].name;
            }
        }
        return listEnum[0].name;
    }

    //! список нумерации
    QVector<ParamEnum> listEnum;
    //! проверка есть ли нумерация у параметра
    bool testHaveEnumParam(void);
    //! поиск параметров с перечислениями
    void recFindEnumParam(const QDomElement&);
    //! тип узла
    virtual int type()const {return Node::PARAM;}
    virtual ~Parameter();
private:
};

#endif /* PARAMETER_H_ */
