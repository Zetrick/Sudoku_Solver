#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include <QObject>
#include <cell.h>

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

    Board(QString sudoku);

    QVector<QVector<Cell*>> board;

    QMap<Cell*, QVector<Cell*>> related_cells_in_row;

    QMap<Cell*, QVector<Cell*>> related_cells_in_column;

    QMap<Cell*, QVector<Cell*>> related_cells_in_quadrant;

    void fill_related_cells_maps();

    bool check_for_solved();

    //Goes through each cell on the board and updates its possible_values vector
    //Recursively calls itself until no more possibilities are found
    void eliminate_possibles_for_every_cell();

    void eliminate_possibles_for_cell_in_all_related(Cell *cell);

    bool eliminate_possibles_for_cell_in_quadrant(Cell *cell);

    bool eliminate_possibles_for_cell_in_row(Cell *cell);

    bool eliminate_possibles_for_cell_in_column(Cell *cell);

    bool find_and_fill_unique_possibility_for_cell(Cell *cell, Related where);

    //Checks related cells' determined values to see if assignment is valid
    bool cell_assignment_valid(Cell *cell, int value);

    QVector<Cell*> get_cells_in_quadrant(const Cell *cell);

    QVector<Cell*> get_cells_in_row(const Cell *cell);

    QVector<Cell*> get_cells_in_row(int row);

    QVector<Cell*> get_cells_in_column(const Cell *cell);

    //This always includes the caller cell
    QVector<Cell*> get_all_related_cells(Cell *cell);

    QVector<int> get_all_related_cells_determined_values(Cell *cell);

    void display_possibles_for_cell(const Cell *cell);

    void display_possibles_for_entire_board();

    //If a row, column, or quadrant has a cell with a sole possibility, fill its determined value
    //e.g. a cell in the top row has '1' listed as a potential and there are no other cells in that row with that potential
    //assign that cell's determined value to '1'
    bool fill_uniques_for_entire_board();

    //Returns the next cell of the current quadrant, or the first cell of the next
    //Returns cell at [0][0] if the last cell of the board is passed in
    Cell * get_next_cell(Cell *cell);

    //Returns the next EMPTY cell after the passed in cell
    //Returns the passed in cell if it is empty
    //Returns nullptr if an empty cell cannot be found
    Cell * get_next_empty_cell(Cell *cell);

    //Holds the number of determined values currently on the board
    int determined_values;

    void print_board();

public slots:
    void update_related_cells_possibilities_from_add(int previous_value, int loc1, int loc2);

    void update_related_cells_possibilities_from_remove(int previous_value, int loc1, int loc2);
};

#endif // BOARD_H
