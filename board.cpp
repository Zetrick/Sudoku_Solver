#include "board.h"
#include <QDebug>

Board::Board()
{
    board.resize(9);
    for(int i = 0; i < board.size(); ++i)
    {
        board[i].resize(9);
    }

    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            board[i][j] = new Square;
            board[i][j]->location.first = i;
            board[i][j]->location.second = j;

            board[i][j]->quadrant = i;
            board[i][j]->row = ( i - (i % 3) ) + (j / 3);
            board[i][j]->column = (i % 3) * 3 + (j % 3);

            connect(board[i][j], &Square::determined_value_assigned, this, &Board::update_related_squares_possibilities);
        }
    }
}

bool Board::find_possibles_in_quadrant(Square *square)
{
    QVector<Square*> squares_in_quadrant = get_squares_in_quadrant(square);

    bool possibles_changed = false;

    //Search through each box of the quadrant and remove possible values for this square if they already
    //exist in the quadrant
    for(int i = 0; i < 9; ++i)
    {
        int val = squares_in_quadrant[i]->get_determined_value();
        if(val != 0)
        {
            if(square->remove_possible_value(val))
                possibles_changed = true;
        }
    }

    return possibles_changed;
}

bool Board::find_possibles_in_row(Square *square)
{
    QVector<Square*> squares_in_row = get_squares_in_row(square);

    bool possibles_changed = false;

    for(int i = 0; i < 9; ++i)
    {
        int val = squares_in_row[i]->get_determined_value();
        if(val != 0)
        {
            if(square->remove_possible_value(val))
                possibles_changed = true;
        }
    }

    return possibles_changed;
}

bool Board::find_possible_in_column(Square *square)
{
    QVector<Square*> squares_in_column = get_squares_in_column(square);

    bool possibles_changed = false;

    for(int i = 0; i < 9; ++i)
    {
        int val = squares_in_column[i]->get_determined_value();
        if(val != 0)
        {
            if(square->remove_possible_value(val))
                possibles_changed = true;
        }
    }

    return possibles_changed;
}

bool Board::find_unique_possibility(Square *square, Related where)
{
    if(square->get_determined_value() != 0)
        return false;

    bool found_unique = false;

    QString where_related;

    QVector<Square*> other_squares;

    if(where == Related::IN_QUADRANT)
        other_squares = get_squares_in_quadrant(square);
    if(where == Related::IN_ROW)
        other_squares = get_squares_in_row(square);
    if(where == Related::IN_COLUMN)
        other_squares = get_squares_in_column(square);

    other_squares.remove( other_squares.indexOf(square));

    QVector<int> possibilities_in_this_square = square->get_possible_values();
    QVector<int> possibilities_in_related_squares;

    for(int i = 0; i < other_squares.size(); ++i)
        possibilities_in_related_squares += other_squares[i]->get_possible_values();

    for(int i = 0; i < possibilities_in_this_square.size(); ++i)
        if(!possibilities_in_related_squares.contains(possibilities_in_this_square[i]))
        {
            square->set_determined_value(possibilities_in_this_square[i]);
            found_unique = true;
            break;
        }


    return found_unique;
}

QVector<Square*> Board::get_squares_in_quadrant(const Square *square)
{
    QVector<Square*> squares;

    for(int i = 0; i < 9; ++i)
        squares += board[square->quadrant][i];

    return squares;
}

QVector<Square*> Board::get_squares_in_row(const Square *square)
{
    QVector<Square*> squares;

    int section_min = (square->row / 3) * 3;

    int row_min = (square->row % 3) * 3;

    for(int i = section_min; i < section_min + 3; ++i)
        for(int j = row_min; j < row_min + 3; ++j)
            squares += board[i][j];

    return squares;
}

QVector<Square *> Board::get_squares_in_column(const Square *square)
{
    QVector<Square*> squares;

    int section_min = square->column / 3;
    int col_min = square->column % 3;

    for(int i = section_min; i < section_min + 7; i += 3)
        for(int j = col_min; j < col_min + 7; j += 3)
            squares += board[i][j];

    return squares;
}

QVector<Square *> Board::get_all_related_squares(const Square *square)
{
    QVector<Square*> all_squares = get_squares_in_quadrant(square);
    QVector<Square*> row_squares = get_squares_in_row(square);
    QVector<Square*> column_square = get_squares_in_column(square);

    for(int i = 0; i < 9; ++i)
    {
        if(!all_squares.contains(row_squares[i]))
            all_squares += row_squares[i];
        if(!all_squares.contains(column_square[i]))
            all_squares += column_square[i];
    }

    return all_squares;
}

void Board::update_related_squares_possibilities(int loc1, int loc2)
{
//    qDebug() << "\nAll related squares of " << board[loc1][loc2]->index() << ": ";
//    for(int i = 0; i < get_all_related_squares(board[loc1][loc2]).size(); ++i)
//        qDebug() << "\n" << get_all_related_squares(board[loc1][loc2])[i]->index();

    QVector<Square*> all_squares = get_all_related_squares(board[loc1][loc2]);
    int val = board[loc1][loc2]->get_determined_value();

    for(Square* i : all_squares)
        i->remove_possible_value(val);
}
