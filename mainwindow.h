#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "new_word.h"
#include "header.h"
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public vocabulary
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fillIn();
    void word2lineEdit(std::string);
    void updateCount_labels();
    void box_border(int b);    // 0 for deselection
    QLabel* label_array[3];
    Ui::MainWindow* getUI();
    int turn_flag = 0;

private slots:
    void on_pushButton_turn_clicked();

    void on_pushButton_nw2_clicked();

    void on_pushButton_wrong_clicked();

    void on_pushButton_correct_clicked();

    void on_lineEdit_Ml2_textEdited(const QString &arg1);

    void on_lineEdit_Ml1_textEdited(const QString &arg1);

    void on_lineEdit_answer_textChanged(const QString &arg1);

    void on_pushButton_correct_pressed();

    void on_pushButton_correct_released();

    void on_pushButton_wrong_pressed();

    void on_pushButton_wrong_released();

private:
    Ui::MainWindow *ui;
    new_word* newWord_gui_ptr;

protected:
    void closeEvent(QCloseEvent* event);
};

//class keyEnterReceiver : public QObject
//{
//    Q_OBJECT
//protected:
//    bool eventFilter(QObject* obj, QEvent* event);
//};
#endif // MAINWINDOW_H
