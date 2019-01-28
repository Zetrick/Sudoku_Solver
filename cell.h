#ifndef CELL_H
#define CELL_H

#include <QVector>
#include <QObject>
#include <QLineEdit>

class Cell : public QObject
{
    Q_OBJECT

public:
    Cell(int assigned_value = 0);

    QVector<int> get_possible_values() const { return possible_values; }

    void set_possible_values(QVector<int> values) { possible_values = values; }

    void add_possible_value(int value);

    bool remove_possible_value(int value);

    QPair<int, int> location;

    QLineEdit * display;

    void set_determined_value(int value);

    void remove_determined_value();

    int get_determined_value() const { return determined_value; }

    QString index() const;

    int quadrant;

    int row;

    int column;

    bool has_determined_value;

    //If this bool is true, it means the determined value will not be changed again.
    bool determined_value_is_official;

    bool operator==(const Cell* rhs);

    int determined_value;

    QVector<int> possible_values;

signals:
    void determined_value_set(int, int, int);

    void determined_value_removed(int, int, int);
};

#endif // CELL_H
