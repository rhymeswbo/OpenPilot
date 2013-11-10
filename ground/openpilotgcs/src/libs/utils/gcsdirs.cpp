#include "gcsdirs.h"

#include <QDir>
#include <QDebug>
#include <QString>
#include <QApplication>

#ifdef Q_OS_MAC
const QLatin1String GCS_SHARE_PATH("/../Resources");
const QLatin1String MARBLE_DATA_PATH("/../Resources");
#else
const QLatin1String GCS_SHARE_PATH("/../share/openpilotgcs");
const QLatin1String MARBLE_DATA_PATH("/../share/marble/data");
#endif

GCSDirs::GCSDirs()
    : d(0)
{
}

QString GCSDirs::pluginPath(QString provider)
{
    // Figure out root : Up one from 'bin'
    QDir rootDir = QApplication::applicationDirPath();
    rootDir.cdUp();
    const QString rootDirPath = rootDir.canonicalPath();

    QString pluginPath = rootDirPath;

#ifdef Q_OS_MACX
    // "PlugIns" (OS X)
    pluginPath += QLatin1Char('/');
    pluginPath += QLatin1String("Plugins");
#else
    // "plugins" (Win/Linux)
    pluginPath += QLatin1Char('/');
    // GCS_LIBRARY_BASENAME is compiler define set by qmake
    pluginPath += QLatin1String(GCS_LIBRARY_BASENAME);
    pluginPath += QLatin1Char('/');
    pluginPath += provider;
    pluginPath += QLatin1Char('/');
    pluginPath += QLatin1String("plugins");
#endif

    return pluginPath;
}

QString GCSDirs::gcsSharePath()
{
	return QCoreApplication::applicationDirPath() + GCS_SHARE_PATH;
}

QString GCSDirs::gcsPluginPath()
{
    return pluginPath("openpilotgcs");
}

QString GCSDirs::marbleDataPath()
{
	return QCoreApplication::applicationDirPath() + MARBLE_DATA_PATH;
}

QString GCSDirs::marblePluginPath()
{
    return pluginPath("marble");
}

void GCSDirs::debug()
{
    qDebug() << "=== GCSDirs ===";
    qDebug() << "GCS Share Path (Run Time) :" << gcsSharePath();
    qDebug() << "GCS Plugin Path (Run Time) :" << gcsPluginPath();
    qDebug() << "";
    qDebug() << "Marble Data Path (Run Time) :" << marbleDataPath();
    qDebug() << "Marble Plugin Path (Run Time) :" << marblePluginPath();
    qDebug() << "===================";
}
