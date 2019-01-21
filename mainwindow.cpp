#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QLineEdit>
#include <QValidator>
#include <QMessageBox>
#include <QKeyEvent>

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

            static_cast<QLineEdit*>(ui->centralWidget->children()[i])->setValidator(v);
            ui->centralWidget->children()[i]->setProperty("loc1", loc1);
            ui->centralWidget->children()[i]->setProperty("loc2", loc2);
            connect(static_cast<QLineEdit*>(ui->centralWidget->children()[i]), &QLineEdit::textEdited, this, &MainWindow::press_tab_key);
        }
}

bool MainWindow::find_potentials()
{
    bool potentials_changed = false;

    for(int i = 0; i < gameboard->board.size(); ++i)
    {
        for(int j = 0; j < gameboard->board[i].size(); ++j)
        {
            if(gameboard->board[i][j]->get_determined_value() != 0) continue;

            if(gameboard->find_possibles_in_quadrant(gameboard->board[i][j]))
                potentials_changed = true;
            if(gameboard->find_possibles_in_row(gameboard->board[i][j]))
                potentials_changed = true;
            if(gameboard->find_possible_in_column(gameboard->board[i][j]))
                potentials_changed = true;

            //display_potentials(gameboard->board[i][j]);
        }
    }

    return  potentials_changed;
}

void MainWindow::fill_uniques()
{
    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {       
            //Find uniques in quadrant
            gameboard->find_unique_possibility(gameboard->board[i][j], Related::IN_QUADRANT);

            //Find uniques in row
            gameboard->find_unique_possibility(gameboard->board[i][j], Related::IN_ROW);

            //Find uniques in column
            gameboard->find_unique_possibility(gameboard->board[i][j], Related::IN_COLUMN);
        }
    }
}

void MainWindow::display_potentials(const Square *square)
{
    qDebug() << "\nPossible values for square: [" << square->location.first << "]"
            "[" << square->location.second << "]: ";

    for(int i = 0; i < square->get_possible_values().size(); ++i)
    {
        qDebug() << square->get_possible_values()[i] << " ";
    }
}

void MainWindow::display_potentials_for_entire_board()
{
    for(int i = 0; i < 9; ++i)
    {
        for(int j = 0; j < 9; ++j)
        {
            qDebug() << "\nPossibilities for square: " << gameboard->board[i][j]->index();
            for(int k = 0; k < gameboard->board[i][j]->get_possible_values().size(); ++k)
                qDebug() << "\n" << gameboard->board[i][j]->get_possible_values()[k];
        }
    }
}

void MainWindow::fill_debug_puzzle()
{
    gameboard->board[0][0]->set_determined_value(5);
    gameboard->board[0][1]->set_determined_value(8);
    gameboard->board[0][2]->set_determined_value(6);
    gameboard->board[1][1]->set_determined_value(7);
    gameboard->board[1][3]->set_determined_value(9);
    gameboard->board[1][5]->set_determined_value(1);
    gameboard->board[1][6]->set_determined_value(6);
    gameboard->board[2][3]->set_determined_value(6);
    gameboard->board[2][6]->set_determined_value(2);
    gameboard->board[2][7]->set_determined_value(5);
    gameboard->board[2][8]->set_determined_value(4);
    gameboard->board[3][2]->set_determined_value(7);
    gameboard->board[3][3]->set_determined_value(9);
    gameboard->board[3][5]->set_determined_value(2);
    gameboard->board[3][8]->set_determined_value(5);
    gameboard->board[4][4]->set_determined_value(1);
    gameboard->board[4][7]->set_determined_value(9);
    gameboard->board[5][3]->set_determined_value(3);
    gameboard->board[5][5]->set_determined_value(5);
    gameboard->board[6][1]->set_determined_value(9);
    gameboard->board[6][5]->set_determined_value(3);
    gameboard->board[7][1]->set_determined_value(4);
    gameboard->board[7][3]->set_determined_value(5);
    gameboard->board[7][7]->set_determined_value(2);
    gameboard->board[8][2]->set_determined_value(8);
    gameboard->board[8][4]->set_determined_value(6);
    gameboard->board[8][6]->set_determined_value(4);
    gameboard->board[8][7]->set_determined_value(7);
}

void MainWindow::solve_puzzle()
{
    find_potentials();

    fill_uniques();

    display_potentials_for_entire_board();
}

void MainWindow::on_pushButton_solve_clicked()
{
    solve_puzzle();
}

void MainWindow::press_tab_key(const QString &text)
{
    QKeyEvent * key_press = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
    QApplication::sendEvent(this, key_press);

    int loc1 = QObject::sender()->property("loc1").toInt();
    int loc2 = QObject::sender()->property("loc2").toInt();

    gameboard->board[loc1][loc2]->set_determined_value(text.toInt());
}
