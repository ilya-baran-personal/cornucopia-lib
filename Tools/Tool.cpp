#include "Tool.h"

#include <QMessageBox>
#include <QDebug>
#include <QTextStream>

void Tool::toolError(QString text)
{
    QMessageBox::critical(_parentWidget, "Tool error", text);
    qDebug() << "Error:" << text;
}

void Tool::transformStream(QTextStream &from, QTextStream &to, const QList<QPair<QRegExp, QString> > &mapping)
{
    while(!from.atEnd())
    {
        QString line = from.readLine();
        
        for(QList<QPair<QRegExp, QString> >::const_iterator it = mapping.constBegin();
            it != mapping.constEnd(); ++it)
        {
            line.replace(it->first, it->second);
        }

        to << line << "\n";
    }
}

#include "Tool.moc"
