#ifndef STATUSLISTWIDGETITEM_H
#define STATUSLISTWIDGETITEM_H

#include <QListWidgetItem>

class StatusListWidgetItem : public QListWidgetItem
{
private:
    static int fontSize;
public:
    StatusListWidgetItem(QString text, bool checked, bool admin);
};

#endif // STATUSLISTWIDGETITEM_H
