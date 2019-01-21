#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include <QObject>
#include <square.h>

enum class Related
{
    IN_QUADRANT,
    IN_ROW,
    IN_COLUMN
};

class Board : public QObject
{
    Q_OBJECT

public:
    Board();

    QVector<QVector<Square*>> board;

    bool find_possibles_in_quadrant(Square *square);

    bool find_possibles_in_row(Square *square);

    bool find_possible_in_column(Square *square);

    bool find_unique_possibility(Square *square, Related where);

    QVector<Square*> get_squares_in_quadrant(const Square *square);

    QVector<Square*> get_squares_in_row(const Square *square);

    QVector<Square*> get_squares_in_column(const Square *square);

    QVector<Square*> get_all_related_squares(const Square *square);

public slots:
    void update_related_squares_possibilities(int loc1, int loc2);
};

#endif // BOARD_H
