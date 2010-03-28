#ifndef TOOL_H_INCLUDED
#define TOOL_H_INCLUDED

#include <QWidget>
#include <QString>

class QTextStream;

class Tool : public QObject
{
    Q_OBJECT

public:
    Tool(QWidget *parent) : QObject(parent), _parentWidget(parent) {}

    virtual QString getName() const = 0;
    QWidget *parentWidget() const { return _parentWidget; }

public slots:
    virtual void execute() = 0;

protected:
    //utility functions
    void toolError(QString text);
    static void transformStream(QTextStream &from, QTextStream &to, const QList<QPair<QRegExp, QString> > &mapping);

private:
    QWidget *_parentWidget;
};

#endif //TOOL_H_INCLUDED
