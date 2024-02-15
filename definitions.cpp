#include <header.h>
#include <vector>
#include <nlohmann/json.hpp>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <mainwindow.h>
#include <QMainWindow>
#include <QFile>
#include <QTextStream>


std::vector<card> vocabulary::json2vector(nlohmann::ordered_json p_json_daten){
    std::vector<card> card_vector;
    card x;
    for(auto it = p_json_daten.begin(); it != p_json_daten.end(); ++it){
        x.word_lang1 = it.key();
        x.word_lang2 = it.value();
        card_vector.push_back(x);
    }
    return card_vector;
}

std::string vocabulary::vector2string(std::vector<card> card_vec[], int activeBox_p){
    std::string cards_str = "{\n";
    for (int j=0; j<7; j++){
        cards_str += "\""+std::to_string(j+1)+"\":{";
        for (unsigned int i = 0; i<card_vec[j].size(); i++){
            if (i == 0){
                cards_str += "\"" +card_vec[j][i].word_lang1 + "\"" + " : " + "\"" + card_vec[j][i].word_lang2 + "\"";
            }
            else {cards_str += ", \"" + card_vec[j][i].word_lang1 + "\"" + " : " + "\"" + card_vec[j][i].word_lang2 + "\"";}
        }
        cards_str += "},\n";
    }
    std::cout<<"box"<<std::to_string(activeBox_p)<<'\n';
    cards_str += "\"activeBox\":" + std::to_string(activeBox_p) + "\n}";
    return cards_str;
}








