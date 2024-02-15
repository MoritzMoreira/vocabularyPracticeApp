#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <header.h>
#include <QFile>
#include <QDir>

using ordered_json = nlohmann::ordered_json;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    struct stat buffer;
    std::string b1 = "./box1.json";
    if (stat(b1.c_str(), &buffer) == 0){
        QFile datei(QString::fromStdString(b1));
        if(!datei.open(QFile::ReadOnly | QFile::Text)){}
        QTextStream in(&datei);
        QString text  = in.readAll();
        std::string json_b1 = text.toStdString();
        nlohmann::ordered_json jsdata_b1 = nlohmann::ordered_json::parse(json_b1);
        V.activeBox = jsdata_b1["activeBox"];
        for (auto it = jsdata_b1.begin(); it.key() != "activeBox"; it++){
              V.card_vector1[std::stoi(it.key())-1] = V.json2vector(it.value());
        }
    }

    updateCount_labels();
    word2lineEdit("lang1");
    ui->label_14->setAttribute(Qt::WA_TransparentForMouseEvents); ui->label_14->setStyleSheet("color:gray");
    ui->label_13->setAttribute(Qt::WA_TransparentForMouseEvents); ui->label_13->setStyleSheet("color:gray");
    ui->lineEdit_Ml1->setText("[front]"); ui->lineEdit_Ml2->setText("[back]"); ui->lineEdit_answer->setText("[answer / leave blank]");

    ui->label_word->setAlignment(Qt::AlignCenter);
    ui->lineEdit_Ml1->setStyleSheet("color:gray"); ui->lineEdit_Ml2->setStyleSheet("color:gray");
    ui->lineEdit_answer->setStyleSheet("color:gray");
    box_border(1);
    ui->progressBar->setTextVisible(0); ui->progressBar_2->setTextVisible(0); ui->progressBar_3->setTextVisible(0);
    ui->progressBar_4->setTextVisible(0); ui->progressBar_5->setTextVisible(0);

    ui->progressBar->setRange(0,10); ui->progressBar_2->setRange(0,15); ui->progressBar_3->setRange(0,25);
    ui->progressBar_4->setRange(0,40); ui->progressBar_5->setRange(0,60);
    ui->pushButton_nw2->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->pushButton_nw2->setStyleSheet("color:gray");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_turn_clicked()
{
      if (turn_flag == 1){
          ui->label_word->setText(QString::fromStdString(V.card_vector1[V.activeBox-1].begin()->word_lang1));
          turn_flag = 0;
          ui->pushButton_wrong->setStyleSheet("background-color: rgb(243, 243, 243)");
          ui->pushButton_correct->setStyleSheet("background-color: rgb(243, 243, 243)");
      }
      else{
          if (turn_flag == 2){
              ui->pushButton_wrong->setStyleSheet("background-color: rgb(243, 243, 243)");
              ui->pushButton_correct->setStyleSheet("background-color: rgb(243, 243, 243)");
                }
          word2lineEdit("lang2");
          ui->label_r7->setStyleSheet("background-color: rgb(239, 239, 239)");
          if (ui->lineEdit_answer->text() != "[answer / leave blank]" && ui->lineEdit_answer->text() != ""){
              if (ui->lineEdit_answer->text().toStdString() == V.card_vector1[V.activeBox-1][0].word_lang2){
                  on_pushButton_correct_clicked();
                  ui->pushButton_correct->setStyleSheet("background-color: green");
                  ui->pushButton_wrong->setStyleSheet("background-color: rgb(243, 243, 243)");
                  turn_flag = 2;
              }
              else {
                  ui->label_word->setText(QString::fromStdString(V.card_vector1[V.activeBox-1].begin()->word_lang2));
                  V.card_vector1[0].insert(V.card_vector1[0].end()-1, V.card_vector1[V.activeBox-1][0]);
                  V.card_vector1[V.activeBox-1].erase(V.card_vector1[V.activeBox-1].begin());
                  updateCount_labels();
                  ui->pushButton_wrong->setStyleSheet("background-color: red");
                  ui->pushButton_correct->setStyleSheet("background-color: rgb(243, 243, 243)");
                  turn_flag = 1;
              }
              ui->lineEdit_answer->setText("");
          }
          else{
               word2lineEdit("lang2");
          }
          ui->lineEdit_answer->setText("[answer / leave blank]"); ui->lineEdit_answer->setStyleSheet("color:gray");
     }
}

void MainWindow::on_pushButton_wrong_clicked()
{
    ui->label_r7->setStyleSheet("background-color: rgb(239, 239, 239)");
    V.card_vector1[0].insert(V.card_vector1[0].end()-1, V.card_vector1[V.activeBox-1][0]);
    V.card_vector1[V.activeBox-1].erase(V.card_vector1[V.activeBox-1].begin());
    word2lineEdit("lang1");
    updateCount_labels();
}

void MainWindow::on_pushButton_correct_clicked()
{
    if (V.activeBox != 6){
        V.card_vector1[V.activeBox].insert(V.card_vector1[V.activeBox].end()-1, V.card_vector1[V.activeBox-1][0]);
    }
    else {
        V.card_vector1[V.activeBox].push_back(V.card_vector1[V.activeBox-1][0]);
        ui->label_r7->setStyleSheet("background-color: yellow");
    }

    V.card_vector1[V.activeBox-1].erase(V.card_vector1[V.activeBox-1].begin());

    if (V.card_vector1[V.activeBox].size()-1 == V.max[V.activeBox-1]){
        word2lineEdit("");
        box_border(0);
        V.activeBox += 1;
        box_border(1);
    }
    word2lineEdit("lang1");
    updateCount_labels();
}

void MainWindow::word2lineEdit(std::string cardside){

    if (V.card_vector1[V.activeBox-1][0].word_lang1 == "max"){
        ui->label_word->setText("");
        box_border(0);
        V.activeBox = 1;
        box_border(1);
    }
    if (V.card_vector1[0][0].word_lang1 == "max" && V.activeBox == 1){
        if (V.card_vector1[1].size()-1 != V.max[0]){
            ui->pushButton_correct->setAttribute(Qt::WA_TransparentForMouseEvents); ui->pushButton_correct->setStyleSheet("color:gray");
        }
    }
    else{
        if (cardside == "lang1"){
             ui->label_word->setText(QString::fromStdString(V.card_vector1[V.activeBox-1].begin()->word_lang1));
        }
        else if (cardside == ""){
            ui->label_word->setText("");
        }
        else{
            ui->label_word->setText(QString::fromStdString(V.card_vector1[V.activeBox-1].begin()->word_lang2));
        }
    }
}

void MainWindow::updateCount_labels(){
    QLabel* label_array[] = {ui->label_numberB1, ui->label_numberB2, ui->label_numberB3, ui->label_numberB4, ui->label_numberB5,
                             ui->label_numberB6, ui->label_numberB7};
    QProgressBar* progBar_array[] = {ui->progressBar, ui->progressBar_2, ui->progressBar_3, ui->progressBar_4, ui->progressBar_5};
    for (int i = 0, j = 1; i<7; ++i, ++j){
        if (i == 6)
            label_array[i]->setText(QString::number(V.card_vector1[i].size()));
        else {
            label_array[i]->setText(QString::number(V.card_vector1[i].size()-1));
            if (j<6){
                progBar_array[i]->setValue(V.card_vector1[j].size()-1);
            }
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    std::string out = V.vector2string(V.card_vector1, V.activeBox);
    std::ofstream outfile;
    outfile.open("./box1.json");
    outfile << out;
    QMainWindow::closeEvent(event);
}

void MainWindow::on_pushButton_nw2_clicked()
{
    std::string lang1 = ui->lineEdit_Ml1->text().toStdString();
    std::string lang2 = ui->lineEdit_Ml2->text().toStdString();
    card x; x.word_lang1 = lang1; x.word_lang2 = lang2;
    V.card_vector1[0].insert(V.card_vector1[0].end()-1, x);
    word2lineEdit("lang1");

    ui->lineEdit_Ml1->setText("");     ui->lineEdit_Ml2->setText("");
    updateCount_labels();
    ui->lineEdit_Ml2->setText("[back]"); ui->lineEdit_Ml1->setText("[front]");
    ui->lineEdit_Ml1->setStyleSheet("color:gray"); ui->lineEdit_Ml2->setStyleSheet("color:gray");
    ui->pushButton_correct->setAttribute(Qt::WA_TransparentForMouseEvents, false); ui->pushButton_correct->setStyleSheet("color:black");

    ui->pushButton_nw2->setAttribute(Qt::WA_TransparentForMouseEvents); ui->pushButton_nw2->setStyleSheet("color:gray");

}

//--------------------------------------------------------------------------------------------------------------------------
Ui::MainWindow* MainWindow::getUI(){
    return ui;
}

void MainWindow::on_lineEdit_Ml2_textEdited(const QString &arg1)
{
    ui->lineEdit_Ml2->setStyleSheet("color:black");
    if (ui->lineEdit_Ml1->text() != "[front]" && ui->lineEdit_Ml1->text() != "" && ui->lineEdit_Ml2->text() != ""){
        ui->pushButton_nw2->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        ui->pushButton_nw2->setStyleSheet("color:black");
    }
}

void MainWindow::on_lineEdit_Ml1_textEdited(const QString &arg1)
{
   ui->lineEdit_Ml1->setStyleSheet("color:black");
    if (ui->lineEdit_Ml2->text() != "[back]" && ui->lineEdit_Ml2->text() != "" && ui->lineEdit_Ml1->text() != ""){
        ui->pushButton_nw2->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        ui->pushButton_nw2->setStyleSheet("color:black");
    }
}

void MainWindow::on_lineEdit_answer_textChanged(const QString &arg1)
{
    ui->lineEdit_answer->setStyleSheet("color:black");

}

void MainWindow::on_pushButton_correct_pressed()
{
    ui->pushButton_correct->setStyleSheet("background-color:green");
}

void MainWindow::on_pushButton_correct_released()
{
    ui->pushButton_correct->setStyleSheet("background-color:white");
}

void MainWindow::on_pushButton_wrong_pressed()
{
    ui->pushButton_wrong->setStyleSheet("background-color:red");
}

void MainWindow::on_pushButton_wrong_released()
{
    ui->pushButton_wrong->setStyleSheet("background-color:white");
}

void MainWindow::box_border(int b){
    QLabel* border_arr[] = {ui->label_r1, ui->label_r2, ui->label_r3, ui->label_r4, ui->label_r5, ui->label_r6};
    if (b == 0){
        border_arr[V.activeBox-1]->setStyleSheet("border: 0px solid blue");
    }
    else{
        border_arr[V.activeBox-1]->setStyleSheet("border: 3px solid blue");
    }
}
