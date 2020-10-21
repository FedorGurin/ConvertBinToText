#ifndef PARSERMEMDCS_H
#define PARSERMEMDCS_H
#include "node.h"
#include <QStringList>

//! иницализация сериализации XML
void initXmlSerialLib();
//! загрузка файла XML для сериализации
Node* loadFileXmlSerialLib(QString nameFile); // возвращает указатель на корень
//! копирование памяти в узлы дерева
void cpyMemToTreeSerialLib(Node* toNode, QByteArray *fromMem);
//! копирование данных из узлов в память
void cpyTreeToMemSerialLib(Node* fromNode, QByteArray *toMem);
//! преобразование дерева в строки
QStringList convertValueTreeToStrings(Node* root);
//! преобразование дерева в строки
void convertTitleTreeToStrings(Node* root,QStringList &listName,QStringList &listMes);
//! очистка узлов дерева
void clearTreeSerialLib();
#endif // PARSERMEMDCS_H
