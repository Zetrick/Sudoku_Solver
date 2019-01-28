#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "board.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_solve_clicked();

    void process_manual_key_press(const QString &text);

    void brute_force_value_changed(const QString &text);

    void on_pushButton_clear_clicked();

    void on_pushButton_load_next_clicked();

private:
    Ui::MainWindow *ui;

    Board * gameboard;

    QVector<Board*> more_sudokus;

    void format_ui();

    void fill_debug_puzzle();

    //Solves as much of the puzzle as possible
    void solve_via_brute_force_eliminating_possibles();

    void solve_via_brute_force_every_number();

    //Brute force recursion method, returns true if assignment is valid
    bool assign_possible_value_to_cell(Cell *cell);

    bool assign_every_value_to_cell(Cell *cell);

    void solve_via_deduction();

    void update_ui_from_board();

    void update_ui_from_board_every_value();

    void set_new_gameboard(Board* board);
};

#endif // MAINWINDOW_H
