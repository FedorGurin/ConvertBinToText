#ifndef LIBMPPMSPEC_H
#define LIBMPPMSPEC_H

#include "libmppm_global.h"
#include "IEngineData.h".h"
#include "parameter.h"
#include <QList>

extern "C" LIBMPPMSHARED_EXPORT IEngineData* createEngineSpec();
extern "C" LIBMPPMSHARED_EXPORT QString findDisplayName(Node* node);
extern "C" LIBMPPMSHARED_EXPORT Node* findNode(IEngineData* , QString idName);
extern "C" LIBMPPMSHARED_EXPORT Parameter* findParameter(IEngineData* , QString idName);
extern "C" LIBMPPMSHARED_EXPORT void grabberNodesByType(IEngineData*,
                                                   Node* startNode,
                                                   Node::Type isType,
                                                   QList<Node* > nodes);
extern "C" LIBMPPMSHARED_EXPORT QString findUnits(Node* node);
#endif // LIBMPPM_H
