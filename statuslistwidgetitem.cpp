#include "statuslistwidgetitem.h"

int StatusListWidgetItem::fontSize = 12;

StatusListWidgetItem::StatusListWidgetItem(QString text, bool checked, bool admin)
{
    QFont font;
    font.setPointSize(fontSize);
    this->setFont(font);
    this->setText(text);
    this->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    this->setFlags(admin ? this->flags() : this->flags() & ~Qt::ItemIsUserCheckable);
}
