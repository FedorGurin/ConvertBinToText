#ifndef LIBMPPMSPEC_H
#define LIBMPPMSPEC_H

#include "libmppm_global.h"
#include "IEngineData.h".h"
#include "parameter.h"
#include <QList>

extern "C" LIBMPPMSHARED_EXPORT IEngineData* createEngineSpec();
extern "C" LIBMPPMSHARED_EXPORT QString findDisplayName(Node* node);
//! поиск Node согласно idName(строка с путем к параметру, путь может разделен .
extern "C" LIBMPPMSHARED_EXPORT Node* findNode(IEngineData* , QString idName);
extern "C" LIBMPPMSHARED_EXPORT Parameter* findParameter(IEngineData* , QString idName);
extern "C" LIBMPPMSHARED_EXPORT void grabberNodesByType(IEngineData*,
                                                   Node* startNode,
                                                   Node::Type isType,
                                                   QList<Node* > nodes);
extern "C" LIBMPPMSHARED_EXPORT QString findUnits(Node* node);

//! копирование памяти в узлы дерева
extern "C" void cpyMemToTreeSerialLib(IEngineData* e,Node* toNode, QByteArray *fromMem);
//! преобразование дерева в строки
extern "C" void  convertValueTreeToStrings(Node* root, QStringList &list);
//! преобразование дерева в строки
extern "C" void convertTitleTreeToStrings(Node* root,QStringList &listName,QStringList &listMes);

#endif // LIBMPPM_H
