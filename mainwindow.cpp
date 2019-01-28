#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QLineEdit>
#include <QValidator>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTime>
#include <QFile>
#include <QTextStream>

static int calls = 0;

const QMap<QString, int> keys = { {"TL_", 0},
                                  {"TM_", 1},
                                  {"TR_", 2},
                                  {"ML_", 3},
                                  {"MM_", 4},
                                  {"MR_", 5},
                                  {"BL_", 6},
                                  {"BM_", 7},
                                  {"BR_", 8} };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gameboard(new Board)
{
    format_ui();

    //fill_debug_puzzle();

    update_ui_from_board();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::format_ui()
{
    ui->setupUi(this);

    this->setWindowTitle("Sudoku Solver");

    QRegExpValidator *v = new QRegExpValidator(QRegExp("[1-9]{1,1}"));

    for(int i = 0; i < ui->centralWidget->children().count(); ++i)
        if(strncmp(ui->centralWidget->children()[i]->metaObject()->className(), "QLineEdit", 9) == 0)
        {
            QString name = ui->centralWidget->children()[i]->objectName();

            int loc1 = keys[name.left(3)];
            int loc2 = name.right(1).toInt() - 1;
            gameboard->board[loc1][loc2]->display =
                    static_cast<QLineEdit*>(ui->centralWidget->children()[i]);

            if(gameboard->board[loc1][loc2]->determined_value > 0)
                gameboard->board[loc1][loc2]->display->setStyleSheet("color: red");
            else
                gameboard->board[loc1][loc2]->display->setStyleSheet("");

            static_cast<QLineEdit*>(ui->centralWidget->children()[i])->setValidator(v);
            ui->centralWidget->children()[i]->setProperty("loc1", loc1);
            ui->centralWidget->children()[i]->setProperty("loc2", loc2);
            connect(static_cast<QLineEdit*>(ui->centralWidget->children()[i]), &QLineEdit::textEdited, this, &MainWindow::process_manual_key_press);
            //connect(static_cast<QLineEdit*>(ui->centralWidget->children()[i]), &QLineEdit::textChanged, this, &MainWindow::brute_force_value_changed);
        }
}

void MainWindow::fill_debug_puzzle()
{
//    gameboard->board[0][0]->set_determined_value(2);
//    gameboard->board[0][1]->set_determined_value(6);
//    gameboard->board[0][2]->set_determined_value(4);
    gameboard->board[0][3]->set_determined_value(1);
//    gameboard->board[0][4]->set_determined_value(3);
//    gameboard->board[0][5]->set_determined_value(7);
//    gameboard->board[0][6]->set_determined_value(5);
//    gameboard->board[0][7]->set_determined_value(9);
//    gameboard->board[0][8]->set_determined_value(8);
    gameboard->board[1][0]->set_determined_value(7);
    gameboard->board[1][6]->set_determined_value(4);
    gameboard->board[1][7]->set_determined_value(3);
    gameboard->board[2][6]->set_determined_value(2);
    gameboard->board[4][3]->set_determined_value(5);
    gameboard->board[4][5]->set_determined_value(9);
    gameboard->board[5][2]->set_determined_value(6);
    gameboard->board[5][6]->set_determined_value(4);
    gameboard->board[5][7]->set_determined_value(1);
    gameboard->board[5][8]->set_determined_value(8);
    gameboard->board[6][5]->set_determined_value(2);
    gameboard->board[6][7]->set_determined_value(4);
    gameboard->board[7][1]->set_determined_value(8);
    gameboard->board[7][2]->set_determined_value(1);
    gameboard->board[8][4]->set_determined_value(5);
    gameboard->board[8][6]->set_determined_value(3);
}

void MainWindow::solve_via_brute_force_eliminating_possibles()
{
    if(assign_possible_value_to_cell(gameboard->get_next_empty_cell(gameboard->board[0][0])))
    {
        qDebug() << "SOLVED! in " << calls << " calls";
        update_ui_from_board();

        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    else
        qDebug() << "NOT SOLVED!";

}

void MainWindow::solve_via_brute_force_every_number()
{
    if(assign_every_value_to_cell(gameboard->get_next_empty_cell(gameboard->board[0][0])))
    {
        qDebug() << "SOLVED! in " << calls << " calls";
        update_ui_from_board_every_value();

        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    else
        qDebug() << "NOT SOLVED!";

}

bool MainWindow::assign_possible_value_to_cell(Cell * cell)
{
    ++calls;
    if(calls % 1000 == 0)
    {
        //qDebug() << calls;
        update_ui_from_board();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }

    //If all determined values are filled out, return! We have a solved puzzle!
    if(!cell)
    {
        if(gameboard->check_for_solved())
            return true;
    }

    //Assign the next possible value for this cell
    QVector<int> possible_values = cell->get_possible_values();

    for(int i = 0; i < possible_values.size(); ++i)
    {
        cell->set_determined_value(possible_values[i]);

        if(assign_possible_value_to_cell(gameboard->get_next_empty_cell(cell)) == true)
            return true;
    }

    cell->remove_determined_value();

    return false;
}

bool MainWindow::assign_every_value_to_cell(Cell *cell)
{
    ++calls;
    if(calls % 10000 == 0)
    {
        qDebug() << calls;
        update_ui_from_board_every_value();
                QTime dieTime= QTime::currentTime().addMSecs(1);
                    while (QTime::currentTime() < dieTime)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }

    //If all determined values are filled out, return! We have a solved puzzle!
    if(!cell)
        return true;

    //Assign the next possible value for this cell

    for(int i = 1; i < 10; ++i)
    {
        if(gameboard->cell_assignment_valid(cell, i))
            cell->determined_value = i;
        else
            continue;

        if(assign_every_value_to_cell(gameboard->get_next_empty_cell(cell)) == true)
            return true;
    }

    cell->determined_value = 0;

    return false;
}

void MainWindow::solve_via_deduction()
{
    while(!gameboard->check_for_solved())
    {
        gameboard->eliminate_possibles_for_every_cell();
        if(!gameboard->fill_uniques_for_entire_board())
            return;

        update_ui_from_board();

        ++calls;
        qDebug() << calls;

//        QTime dieTime= QTime::currentTime().addMSecs(1000);
//            while (QTime::currentTime() < dieTime)
//                QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
    }
}

void MainWindow::update_ui_from_board()
{
    for(int i = 0; i < gameboard->board.size(); ++i)
        for(int j = 0; j < gameboard->board.last().size(); ++j)
            if(gameboard->board[i][j]->has_determined_value)
            {
                gameboard->board[i][j]->display->setText(QString::number(gameboard->board[i][j]->get_determined_value()));
            }
            else
            {
                gameboard->board[i][j]->display->clear();
            }
}

void MainWindow::update_ui_from_board_every_value()
{
    for(int i = 0; i < gameboard->board.size(); ++i)
        for(int j = 0; j < gameboard->board.last().size(); ++j)
        {
            if(gameboard->board[i][j]->determined_value > 0)
                gameboard->board[i][j]->display->setText(QString::number(gameboard->board[i][j]->determined_value));
            else
                gameboard->board[i][j]->display->clear();
        }
}

void MainWindow::set_new_gameboard(Board *board)
{
    gameboard = board;

    format_ui();
}

void MainWindow::on_pushButton_solve_clicked()
{
    //Should display the total time elapsed for this

    int highest_numger_of_calls = 0;

    QTime total_time = QTime::currentTime();

    for(int i = 0; i < more_sudokus.size(); ++i)
    {
        calls = 0;

        set_new_gameboard(more_sudokus[i]);

        update_ui_from_board_every_value();
                QTime dieTime= QTime::currentTime().addMSecs(1000);
                    while (QTime::currentTime() < dieTime)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

        QTime puzzle_time = QTime::currentTime();

        qDebug() << "Puzzle " << i+1 << "...";

        solve_via_brute_force_every_number();

        if(calls > highest_numger_of_calls)
            highest_numger_of_calls = calls;

        qDebug() << "Elapsed time: " << puzzle_time.secsTo(QTime::currentTime())/60 << " minutes " <<
                    puzzle_time.secsTo(QTime::currentTime()) - (puzzle_time.secsTo(QTime::currentTime())/60)*60 << " seconds.\n";
    }

    qDebug() << "Total time: " << total_time.secsTo(QTime::currentTime())/60 << " minutes " <<
                total_time.secsTo(QTime::currentTime()) - (total_time.secsTo(QTime::currentTime())/60)*60 << " seconds.\n";

    qDebug() << "The hardest puzzle took " << highest_numger_of_calls << " calls";
}

void MainWindow::process_manual_key_press(const QString &text)
{
    //Get the cell that was altered
    int loc1 = QObject::sender()->property("loc1").toInt();
    int loc2 = QObject::sender()->property("loc2").toInt();

    //Set its determined value, if it's valid
    if(gameboard->cell_assignment_valid(gameboard->board[loc1][loc2], text.toInt()))
    {
        //Simulate pressing the Tab key to advance to the next cell automatically
        QKeyEvent * key_press = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
        QApplication::sendEvent(this, key_press);

        if(text.toInt() != gameboard->board[loc1][loc2]->get_determined_value())
        {
                gameboard->board[loc1][loc2]->set_determined_value(text.toInt());
                gameboard->board[loc1][loc2]->display->setStyleSheet(gameboard->board[loc1][loc2]->display->styleSheet() + "color: red;");
                gameboard->board[loc1][loc2]->display->setText(QString::number(gameboard->board[loc1][loc2]->get_determined_value()));
                gameboard->board[loc1][loc2]->determined_value_is_official = true;

        }
    }
    else
    {
        //If the value in the text box was deleted, reset the style sheet
        if(text.size() == 0)
        {
            gameboard->board[loc1][loc2]->display->setStyleSheet("");
            gameboard->board[loc1][loc2]->remove_determined_value();
            gameboard->board[loc1][loc2]->determined_value_is_official = false;
        }
        else
        {
            gameboard->board[loc1][loc2]->display->clear();
            ui->statusBar->showMessage("Invalid placement");
        }
    }


}

void MainWindow::brute_force_value_changed(const QString &text)
{
    //if(!brute_force_going) return;

    //gameboard->display_possibles_for_entire_board();

    //Get the cell that was altered
    int loc1 = QObject::sender()->property("loc1").toInt();
    int loc2 = QObject::sender()->property("loc2").toInt();

    //qDebug() << "Assigning: " << text << " to cell: " << gameboard->board[loc1][loc2]->index();
    //gameboard->display_possibles_for_cell(gameboard->board[loc1][loc2]);

    gameboard->board[loc1][loc2]->set_determined_value(text.toInt());
}

void MainWindow::on_pushButton_clear_clicked()
{
    for(int i = 0; i < 9; ++i)
        for(int j = 0; j < 9; ++j)
        {
            gameboard->board[i][j]->remove_determined_value();
            gameboard->board[i][j]->set_possible_values(QVector<int>{1,2,3,4,5,6,7,8,9});
            gameboard->board[i][j]->determined_value_is_official = false;
            gameboard->board[i][j]->display->setStyleSheet("");
            gameboard->board[i][j]->determined_value = 0;

        }

    calls = 0;

    update_ui_from_board();
}

void MainWindow::on_pushButton_load_next_clicked()
{
    on_pushButton_clear_clicked();
    QString line_read;
    QString sudoku_in_text;
    QFile sudokus("/Users/Zetrick/Desktop/sudoku.txt");
    if(sudokus.open(QFile::ReadOnly))
    {
        QTextStream in(&sudokus);

        while(!in.atEnd())
        {
            sudoku_in_text.clear();

            for(int i = 0; i < 81; ++i)
            {
                QString line = in.read(1);
                //qDebug() << line;
                sudoku_in_text += line;
            }
            //in.read(1);
            //sudoku_in_text = in.readLine();

//            for(int i = 0; i < 9; ++i)
//            {
//                if(i > 0)
//                {
//                    line_read = in.read(1);
//                }
//                for(int j = 0; j < 9; ++j)
//                {
//                    sudoku_in_text += in.read(1);
//                }

            more_sudokus.append( new Board(sudoku_in_text) );
            in.read(1);
        }
    }

    ui->statusBar->showMessage("Loaded " + QString::number(more_sudokus.size()) + " puzzles");

//    set_new_gameboard(more_sudokus[0]);
//    update_ui_from_board_every_value();
//    //Print the sudokus
//    for(int i = 0; i < more_sudokus.size(); ++i)
//    {
//        set_new_gameboard( more_sudokus[i] );
//        gameboard->print_board();
//    }

//    //set_new_gameboard(lots_of_sudokus[0]);
//    update_ui_from_board_every_value();
//    //gameboard->print_board();
}
