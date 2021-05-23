#pragma once

#ifndef PARAMETER_H_
#define PARAMETER_H_


#include "GenericNode.h"
#include <QVariant>
#include <QAbstractItemModel>
#include <stdint.h>

#define ALIGN_MAX_LENGTH

/*!  Класс Parameter
 *  Класс описывающий любой параметр
 * хранит в себе тип параметра, значение, способ
 *  отображения пользователю
*/
class Parameter: public GenericNode
{
public:
    //! Тип данных
    enum TypeData{DOUBLE    = 0,     /*!< Enum тип параметра double */
                  INT       = 1,     /*!< Enum тип параметра int   */
                  FLOAT     = 2,     /*!< Enum тип параметра float */
                  BOOL      = 3,     /*!< Enum тип параметра bool  */
                  BIT       = 4,     /*!< Enum тип параметра bit   */
                  UCHAR     = 5,     /*!< Enum тип параметра uchar */
                  UINT      = 6,     /*!< Enum тип параметра uint  */
                  MCHAR     = 7,     /*!< Enum тип параметра char[]*/
                  CHAR      = 8,     /*!< Enum тип параметра char*/
                  INT16     = 9,     /*!< Enum тип параметра int16*/
                  LDOUBLE   = 10,    /*!< Enum тип параметра long double*/
                  INT64     = 11,    /*!< Enum тип параметра int64*/
                  UINT64    = 12,    /*!< Enum тип параметра uint64*/
                  UINT16    = 13};   /*!< Enum тип параметра uint16*/
    //! Тип редактирования
    enum TypeEdit{DEFAULT   ,   /*!< Enum тип редактирования по умолчанию */
                  DEC       ,   /*!< Enum тип редактировать, как число в 10 с.счисления */
                  HEX       ,   /*!< Enum тип редактировать, как число в 16 с.счисления */
                  BIN       ,   /*!< Enum тип редактировать, как число в двоичной */
                  OCT       };  /*!< Enum тип редактировать, как число в 8 с.счисления */
    //! Тип индикации
    enum TypeIndication{DEFAULT_IND,
                        DIAL,
                        SLIDER_H,
                        SLIDER_V,
                        GRAD_MINUTE_SEC, /*!< Enum редактирование через поля град.минуты.секунды */
                        CHECK_BOX};


    Parameter(const QDomElement&, Node*);

    Parameter(const Parameter* nodeNeedCopy,Node *glParent = nullptr /*глобальный элемент*/);

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



    //! множитель
    QString factor;


    //! признак того, что в описании для параметра определен множитель
    bool isFactor;
    //! значение множителя
    float factorValue;

    unsigned isBytes();
    unsigned isOffset();

    //! условие применения данного параметра
    //QString condition;

    //! Указатель на значение варианта
    QString* valueVariant;

    //! значение параметра в виде строки
    QString value;
    //! значение параметра
    QString rvalue(){return value;}
    //! значение параметра в бинарном виде(с учетом типа)
    char* bin;
    //! заполнение переменной bin
    static char* binData(Parameter* node);
    //! задать бинарные данные
    static void setBinData(uint8_t* fromData, Parameter* node);
    //! выравнивание
    static uint alignData(QDataStream &out,Parameter* node);
    static char alignArray[8];


    //! предыдущее значение параметра
    QString valuePred;
    //! параметр только для чтения
    bool readOnly;
    //! признак того, что значение данного параметра изменилось с предыдущего шага
    bool modifyValueLastStep;

    //! начальный бит
    quint8 numBit;
    //! конечный бит
    quint8 endBit;


    void setValue(QString name_)
    {
        valuePred   = value;
        value       = name_;

        if(valuePred.compare(value)!=0)
            modifyValueLastStep=true;
        else
            modifyValueLastStep=false;

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
    bool hasStaticVariants;

    //! признак циклического опроса данных
    bool cycle_request;

    //! способ редактирования
    TypeEdit modeEdit;

    //! способ редактирования и индикации
    TypeIndication modeIndication;

    //! данное поле отображает долготу или широту
    bool fiLamda;

    //! если поле является строкой
    int lengthMCHAR;//! длина строки char

    //! число состоящее из нескольких битиков
    bool complexBit;

    //! from Nata 20-11-2018
    //! Шаг для slider
    float step_slider;
    //! Шаг для spinbox
    float step_spinbox;

    //! тип занчения - перечисление(название занчение параметра и код)
    //! например, ("Вкл",1), ("Выкл",0)
    struct ParamEnum
    {
        QString name;
        QString value;
    };

    //! возврат значения по перечислению
    QString valueEnum(QString name_)
    {
        ParamEnum str;
        if(listEnum.isEmpty() == true)
            return QString();

        str  = listEnum.first();

        for(auto i: listEnum)
        {
            if(i.name == name_)
            {
                return i.value;
            }
        }

        return str.value;
    }
    //! возврат. имени перечисления по значению
    QString nameEnum(QString value_)
    {
        ParamEnum str;
        if(listEnum.isEmpty() == true)
            return QString();

        str  = listEnum.first();

        for(auto i: listEnum)
        {
            if(i.value == value_)
            {
                return i.name;
            }
        }
        return str.name;
    }

    uint checkAlign(uint b)
    {
        int aB = 0;
#ifndef ALIGN_MAX_LENGTH
        if(b > 4)
            b = 4;
#endif

        if(b == 0 || b == 1)
            return 0;
        if(offset<b && offset!=0)
            aB = b - offset;
        else
        {
            int slice = offset% b;
            if(slice!=0)
                aB = b - slice;
        }
        return aB;
    }
    virtual void calcOffset()
    {
        int alignStruct = 0;
        GenericNode *gP =  static_cast<GenericNode* > (parent);
        // определим смещение
        if((leftParam() != nullptr) && (uniformAddr() == false))
        {
            offset = leftParam()->offset + leftParam()->bytes;
            if(left->type() == Node::E_STRUCT)
            {
                GenericNode *gS = static_cast<GenericNode* > (leftParam());
                alignStruct     = checkAlign(recFindBiggerType(gS));
                offset         += alignStruct;
                gS->addBytes(alignStruct);
            }
        }else
        {
            offset = parent->offset;
        }

        int ch = 0;
        if(this->typeP != Parameter::MCHAR)
            ch = checkAlign(bytes);
        // подсчет байт для выравнивания
        alignBytes += ch;
        offset     += alignBytes;
        alignBytes += alignStruct;

        gP->addBytes(bytes + alignBytes - alignStruct);
    }

    uint32_t recFindBiggerType(GenericNode *node)
    {
        uint32_t max = 0;
        for(auto i : node->child)
        {
            if(i->bytes > max)
                max = i->bytes;
        }
        return max;
    }
    //! список нумерации
    QVector<ParamEnum> listEnum;
    //! список вариантов
    QVector<ParamEnum> listStaticVariant;

    //! установка варианта
    void setStaticVariant(QString name);
    //! проверка есть ли нумерация у параметра
    bool testHaveEnumParam(void);
    //! проверка есть статические параметры
    bool testHaveStaticVariants(void);
    //! поиск параметров с перечислениями
    void recFindEnumParam(const QDomElement&);
    //! поиск статических вариантов
    void recFindStaticVariants(const QDomElement& el);
    //! тип узла
    virtual int type()const {return Node::E_PARAM;}
    virtual ~Parameter() = default;
private:
};

#endif /* PARAMETER_H_ */
