#ifndef RALATIVEPOINTSPLUGIN_H
#define RALATIVEPOINTSPLUGIN_H
#include "RelativePoints.h"
#include <App/PluginInterface.h>
#include <QtCore>
//=============================================================================
class RelativePointsPlugin : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.uavos.gcs.PluginInterface/1.0")
    Q_INTERFACES(PluginInterface)
public:
    int flags() override { return Feature | Map; }
    QObject *createControl() override { return new RelativePoints(); }
    QStringList depends() override { return QStringList() << "MissionPlanner"; }
};
//=============================================================================
#endif
