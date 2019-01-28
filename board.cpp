#include "board.h"
#include <QDebug>

Board::Board()
{
    board.resize(9);
    for(int i = 0; i < board.size(); ++i)
    {
        board[i].resize(9);
    }

    for(int i = 0; i < board.size(); ++i)
    {
        for(int j = 0; j < board[i].size(); ++j)
        {
            board[i][j] = new Cell;
            board[i][j]->location.first = i;
            board[i][j]->location.second = j;

            board[i][j]->quadrant = i;
            board[i][j]->row = ( i - (i % 3) ) + (j / 3);
            board[i][j]->column = (i % 3) * 3 + (j % 3);

            //connect(board[i][j], &Cell::determined_value_set, this, &Board::update_related_cells_possibilities_from_add);
            //connect(board[i][j], &Cell::determined_value_removed, this, &Board::update_related_cells_possibilities_from_remove);
        }
    }

    fill_related_cells_maps();
}

Board::Board(QString sudoku)
{
    board.resize(9);
    for(int i = 0; i < board.size(); ++i)
    {
        board[i].resize(9);
    }

    for(int i = 0; i < board.size(); ++i)
    {
        for(int j = 0; j < board[i].size(); ++j)
        {
            board[i][j] = new Cell;
            board[i][j]->location.first = i;
            board[i][j]->location.second = j;

            board[i][j]->quadrant = i;
            board[i][j]->row = ( i - (i % 3) ) + (j / 3);
            board[i][j]->column = (i % 3) * 3 + (j % 3);

            //connect(board[i][j], &Cell::determined_value_set, this, &Board::update_related_cells_possibilities_from_add);
            //connect(board[i][j], &Cell::determined_value_removed, this, &Board::update_related_cells_possibilities_from_remove);
        }
    }

    fill_related_cells_maps();

    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
        {
            int val = 0;

            if(sudoku[(9*i)+j].digitValue() != -1)
                val = sudoku[(9*i)+j].digitValue();

            get_cells_in_row(i)[j]->determined_value = val;
        }
}

void Board::fill_related_cells_maps()
{
    for(int i = 0; i < board.size(); ++i)
        for(int j = 0; j < board.last().size(); ++j)
        {
            related_cells_in_row.insert(board[i][j], get_cells_in_row(board[i][j]));
            related_cells_in_column.insert(board[i][j], get_cells_in_column(board[i][j]));
            related_cells_in_quadrant.insert(board[i][j], get_cells_in_quadrant(board[i][j]));
        }
}

bool Board::check_for_solved()
{
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
            if(!board[8-i][8-j]->has_determined_value)
                return false;

    return true;
}

void Board::eliminate_possibles_for_every_cell()
{
    bool eliminated = false;

    for(int i = 0; i < board.size(); ++i)
    {
        for(int j = 0; j < board[i].size(); ++j)
        {
            if(board[i][j]->has_determined_value)
            {
                board[i][j]->set_possible_values(QVector<int>());
                continue;
            }

            if(eliminate_possibles_for_cell_in_quadrant(board[i][j]))
                eliminated = true;
            if(eliminate_possibles_for_cell_in_row(board[i][j]))
                eliminated = true;
            if(eliminate_possibles_for_cell_in_column(board[i][j]))
                eliminated = true;
        }
    }

    if(eliminated)
        eliminate_possibles_for_every_cell();
}

void Board::eliminate_possibles_for_cell_in_all_related(Cell *cell)
{
    eliminate_possibles_for_cell_in_quadrant(cell);
    eliminate_possibles_for_cell_in_row(cell);
    eliminate_possibles_for_cell_in_column(cell);
}

bool Board::eliminate_possibles_for_cell_in_quadrant(Cell *cell)
{
    QVector<Cell*> cells_in_quadrant = get_cells_in_quadrant(cell);

    bool eliminated = false;

    //Search through each box of the quadrant and remove possible values for this cell if they already
    //exist in the quadrant
    for(int i = 0; i < cells_in_quadrant.size(); ++i)
    {
        if(cells_in_quadrant[i]->has_determined_value)
        {
            if(cell->remove_possible_value(cells_in_quadrant[i]->get_determined_value()))
                eliminated = true;
        }
    }

    return eliminated;
}

bool Board::eliminate_possibles_for_cell_in_row(Cell *cell)
{
    QVector<Cell*> cells_in_row = get_cells_in_row(cell);

    bool eliminated = false;

    for(int i = 0; i < cells_in_row.size(); ++i)
    {
        if(cells_in_row[i]->has_determined_value)
        {
            if(cell->remove_possible_value(cells_in_row[i]->get_determined_value()))
                eliminated = true;
        }
    }

    return eliminated;
}

bool Board::eliminate_possibles_for_cell_in_column(Cell *cell)
{
    QVector<Cell*> cells_in_column = get_cells_in_column(cell);

    bool eliminated = false;

    for(int i = 0; i < cells_in_column.size(); ++i)
    {
        if(cells_in_column[i]->has_determined_value)
        {
            if(cell->remove_possible_value(cells_in_column[i]->get_determined_value()))
                eliminated = true;
        }
    }

    return eliminated;
}

bool Board::find_and_fill_unique_possibility_for_cell(Cell *cell, Related where)
{
    bool found_unique = false;

    QVector<Cell*> other_cells;

    if(where == Related::IN_QUADRANT)
        other_cells = related_cells_in_quadrant[cell];
    if(where == Related::IN_ROW)
        other_cells = related_cells_in_row[cell];
    if(where == Related::IN_COLUMN)
        other_cells = related_cells_in_column[cell];

    QVector<int> possibilities_in_this_cell = cell->get_possible_values();
    QVector<int> possibilities_in_related_cells;


    for(int i = 0; i < other_cells.size(); ++i)
    {
        if(!other_cells[i]->has_determined_value)
            possibilities_in_related_cells += other_cells[i]->get_possible_values();
    }

    for(int i = 0; i < possibilities_in_this_cell.size(); ++i)
        if(!possibilities_in_related_cells.contains(possibilities_in_this_cell[i]))
        {
            cell->set_determined_value(possibilities_in_this_cell[i]);
            cell->determined_value_is_official = true;
            cell->display->setStyleSheet("color: green");
            found_unique = true;
            break;
        }

    return found_unique;
}

bool Board::cell_assignment_valid(Cell *cell, int value)
{
    for(auto c: related_cells_in_quadrant[cell])
        if(c->get_determined_value() == value)
            return false;
    for(auto c: related_cells_in_row[cell])
        if(c->get_determined_value() == value)
            return false;
    for(auto c: related_cells_in_column[cell])
        if(c->get_determined_value() == value)
            return false;

    return true;
}

QVector<Cell*> Board::get_cells_in_quadrant(const Cell *cell)
{
    QVector<Cell*> cells;

    for(int i = 0; i < 9; ++i)
    {
        if(i == cell->location.second)
            continue;
        cells += board[cell->quadrant][i];
    }

    return cells;
}

QVector<Cell*> Board::get_cells_in_row(const Cell *cell)
{
    QVector<Cell*> cells;

    int section_min = (cell->row / 3) * 3;

    int row_min = (cell->row % 3) * 3;

    for(int i = section_min; i < section_min + 3; ++i)
        for(int j = row_min; j < row_min + 3; ++j)
        {
//            if(i == cell->location.first && j == cell->location.second)
//                continue;
            cells += board[i][j];
        }

    return cells;
}

QVector<Cell*> Board::get_cells_in_row(int row)
{
    return get_cells_in_row(board[(row/3)*3][3*(row%3)]);
}

QVector<Cell *> Board::get_cells_in_column(const Cell *cell)
{
    QVector<Cell*> cells;

    int section_min = cell->column / 3;
    int col_min = cell->column % 3;

    for(int i = section_min; i < section_min + 7; i += 3)
        for(int j = col_min; j < col_min + 7; j += 3)
        {
            if(i == cell->location.first && j == cell->location.second)
                continue;
            cells += board[i][j];
        }

    return cells;
}

QVector<Cell *> Board::get_all_related_cells(Cell *cell)
{
    return ( related_cells_in_row[cell] + related_cells_in_column[cell] + related_cells_in_quadrant[cell] );
}

QVector<int> Board::get_all_related_cells_determined_values(Cell *cell)
{
    QVector<int> values;

    QVector<Cell*> related_cells = get_all_related_cells(cell);
    for(int i = 0; i < related_cells.size(); ++i)
    {
        if(related_cells[i]->has_determined_value)
            if(!values.contains(related_cells[i]->get_determined_value()))
                values += related_cells[i]->get_determined_value();
    }

    return values;
}

void Board::display_possibles_for_cell(const Cell *cell)
{
    qDebug() << "\nPossible values for cell:" << cell->index();

    QString possibilities;

    for(int i : cell->get_possible_values())
        possibilities += QString::number(i) + ", ";

    if(!possibilities.isEmpty())
        possibilities.chop(2);

    qDebug() << possibilities;
}

void Board::display_possibles_for_entire_board()
{
    for(int i = 0; i < board.size(); ++i)
        for(int j = 0; j < board.last().size(); ++j)
            display_possibles_for_cell(board[i][j]);
}

bool Board::fill_uniques_for_entire_board()
{
    int found_unique = 0;

    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            if(board[i][j]->has_determined_value) continue;

            //Find uniques in quadrant
            if(find_and_fill_unique_possibility_for_cell(board[i][j], Related::IN_QUADRANT))
                ++found_unique;

            //Find uniques in row
            if(find_and_fill_unique_possibility_for_cell(board[i][j], Related::IN_ROW))
                ++found_unique;

            //Find uniques in column
            if(find_and_fill_unique_possibility_for_cell(board[i][j], Related::IN_COLUMN))
                ++found_unique;
        }
    }

    return found_unique > 0;
}

Cell *Board::get_next_cell(Cell *cell)
{
    if(cell->location.first == board.size() - 1 && cell->location.second == board.last().size() - 1)
        return board[0][0];
    else
    {
        if(cell->location.second < board.last().size() - 1)
            return board[cell->location.first][cell->location.second + 1];
        else
        {
            //If we're here we're on the last cell of a quadrant, return the first cell of the next
            return board[cell->location.first + 1][0];
        }
    }
}

Cell *Board::get_next_empty_cell(Cell *cell)
{ 
    for(int i = 0 ; i < board.size(); ++i)
    {
        for(int j = 0; j < board.last().size(); ++j)
        {
            if(cell->determined_value < 1)
                return cell;
            else
                cell = get_next_cell(cell);
        }
    }

    return nullptr;
}

void Board::print_board()
{
    QString line;
    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            line += QString::number(board[i][j]->determined_value);
        }
        qDebug() << line;
        line.clear();
    }
    qDebug() << "\n\n";
}

void Board::update_related_cells_possibilities_from_remove(int previous_value, int loc1, int loc2)
{
    //We need to add in possibilities to this and every other related cell if the determined value has changed
    for(int i = 0; i < get_all_related_cells(board[loc1][loc2]).size(); ++i)
    {
        if(previous_value != 0)
            get_all_related_cells(board[loc1][loc2])[i]->add_possible_value(previous_value);
        eliminate_possibles_for_cell_in_all_related(get_all_related_cells(board[loc1][loc2])[i]);
    }
}

void Board::update_related_cells_possibilities_from_add(int previous_value, int loc1, int loc2)
{
    //We need to add in possibilities to this and every other related cell if the determined value has changed
    for(int i = 0; i < get_all_related_cells(board[loc1][loc2]).size(); ++i)
    {
        if(previous_value != 0)
            get_all_related_cells(board[loc1][loc2])[i]->add_possible_value(previous_value);
        eliminate_possibles_for_cell_in_all_related(get_all_related_cells(board[loc1][loc2])[i]);
    }
}
