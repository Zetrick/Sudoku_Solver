#ifndef SQUARE_H
#define SQUARE_H

#include <QVector>
#include <QObject>
#include <QLineEdit>

class Square : public QObject
{
    Q_OBJECT

public:
    Square(int assigned_value = 0);

    QVector<int> get_possible_values() const { return possible_values; }

    void set_possible_values(QVector<int> values) { possible_values = values; }

    void add_possible_value(int value) { if(!possible_values.contains(value)) possible_values += value; }

    bool remove_possible_value(int value);

    QPair<int, int> location;

    QLineEdit * display;

    void set_determined_value(int value);

    int get_determined_value() const { return determined_value; }

    QString index() const;

    int quadrant;

    int row;

    int column;

    bool operator==(const Square &rhs);

signals:
    void determined_value_assigned(int, int);

private:

    //Is 0 until a 1-9 value is 100% determined
    int determined_value;

    QVector<int> possible_values;
};

#endif // SQUARE_H
