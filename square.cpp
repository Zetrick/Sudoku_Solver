#include "square.h"

Square::Square(int assigned_value) :
    display(nullptr),
    determined_value(assigned_value),
    possible_values(QVector<int>{1,2,3,4,5,6,7,8,9})
{

}

bool Square::remove_possible_value(int value)
{
    bool value_removed = false;

    if(possible_values.contains(value))
    {
        possible_values.remove(possible_values.indexOf(value));
        value_removed = true;
    }

    if(possible_values.size() == 1)
        set_determined_value(possible_values.first());

    return value_removed;
}

void Square::set_determined_value(int value)
{
    determined_value = value;
    if(value != 0)
    {
        possible_values.clear();
        display->setText(QString::number(determined_value));
        emit(determined_value_assigned(location.first, location.second));
    }
    else
    {
        possible_values = QVector<int> {1,2,3,4,5,6,7,8,9};
    }
}

QString Square::index() const
{
    return "[" + QString::number(location.first) + "][" + QString::number(location.second) + "]";
}

bool Square::operator==(const Square &rhs)
{
    return index() == rhs.index();
}
