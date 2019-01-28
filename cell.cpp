#include "cell.h"
#include <QDebug>

Cell::Cell(int assigned_value) :
    display(nullptr),
    has_determined_value(false),
    determined_value_is_official(false),
    determined_value(assigned_value),
    possible_values(QVector<int>{1,2,3,4,5,6,7,8,9})
{

}

void Cell::add_possible_value(int value)
{
     if(!possible_values.contains(value))
         possible_values += value;

     //std::sort(possible_values.begin(), possible_values.end());
}

bool Cell::remove_possible_value(int value)
{
    return possible_values.removeAll(value) > 0;
}

void Cell::set_determined_value(int value)
{
    int previous_value = determined_value;

    determined_value = value;

    has_determined_value = true;

    emit(determined_value_set(previous_value, location.first, location.second));
}

void Cell::remove_determined_value()
{
    if(!has_determined_value)
        return;

    int previous_value = determined_value;
    determined_value = 0;
    has_determined_value = false;

    emit(determined_value_removed(previous_value, location.first, location.second));
}

QString Cell::index() const
{
    return "[" + QString::number(location.first) + "][" + QString::number(location.second) + "]";
}

bool Cell::operator==(const Cell *rhs)
{
    return ( (location.first == rhs->location.first) && (location.second == rhs->location.second) );
}
