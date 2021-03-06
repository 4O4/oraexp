#include "iconutil.h"
#include <QObject>
#include <QPixmapCache>
#include <stdexcept>
#include <QtDebug>
#include <QPainter>
#include <QFile>

IconUtil::IconUtil()
{
}

QIcon IconUtil::getEnvironmentIcon(OraExp::ConnectionEnvironment environment, bool big)
{
    QString filename;

    switch(environment){
    case OraExp::Production:
        filename = "red";
        break;
    case OraExp::Staging:
        filename = "yellow";
        break;
    case OraExp::Development:
        filename = "blue";
        break;
    default:
        filename = "green";
        break;
    }

    if(big){
        filename.append("_big");
    }

    return QIcon(QString(":/images/%1.png").arg(filename));
}

QPixmap IconUtil::getIcon(const QString &iconName)
{
    if(iconName.isEmpty()){
        return QPixmap();
    }

    QPixmap result;

    if (!QPixmapCache::find(iconName, &result)) {
        result = loadIconFromFile(iconName);
        QPixmapCache::insert(iconName, result);
    }

    return result;
}

QPixmap IconUtil::loadIconFromFile(const QString &iconName)
{
    QString iconPath=QString(":/images/%1.png").arg(iconName);
    if(QFile::exists(iconPath)){
        return QPixmap(iconPath);
    }

    qDebug("trying to create overlay image");
    int ix=iconName.lastIndexOf("_");
    if(ix==-1){
        return QPixmap();
    }

    iconPath = QString(":/images/%1.png").arg(iconName.mid(0, ix));
    if(!QFile::exists(iconPath)){
        return QPixmap();
    }

    QString suffix = iconName.mid(ix+1);

    if(suffix=="add" || suffix=="alter" || suffix=="invalid" || suffix=="view" || suffix=="copy"){
        QString overlayPath = QString(":/images/%1_small.png").arg(suffix);
        return IconUtil::overlayIcons(QPixmap(iconPath), QPixmap(overlayPath));
    }

    return QPixmap();
}

QPixmap IconUtil::overlayIcons(const QPixmap &first, const QPixmap &second)
{
    Q_ASSERT(first.size().width()>=second.size().width());

    QPixmap pixmap(first.size());
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);
    p.drawPixmap(first.rect(), first);

    int xOffset=first.width()-second.width();
    int yOffset=first.height()-second.height();
    p.drawPixmap(xOffset, yOffset, second.width(), second.height(), second);

    return pixmap;
}
