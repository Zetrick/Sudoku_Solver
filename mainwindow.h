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

    void press_tab_key(const QString &text);

private:
    Ui::MainWindow *ui;

    Board * gameboard;

    void format_ui();

    void display_potentials(const Square *square);

    void display_potentials_for_entire_board();

    void fill_debug_puzzle();

    //Returns true if the potentials of any square changed
    bool find_potentials();

    //If a row, column, or quadrant has a square with a sole possibility, fill its determined value
    //e.g. a square in the top row has '1' listed as a potential and there are no other squares in that row with that potential
    //assign that square's determined value to '1'
    void fill_uniques();

    //Solves as much of the puzzle as possible
    void solve_puzzle();

};

#endif // MAINWINDOW_H
