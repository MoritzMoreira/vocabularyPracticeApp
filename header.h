#include <vector>
#include <nlohmann/json.hpp>
#include <QMainWindow>
#include <QLabel>

#ifndef HEADER_H
#define HEADER_H

class card{
public:
    std::string word_lang1, word_lang2;
};

class Fach{
    int max_cards;
    std::vector<card> vector_cards;
public:
    void add_newCard(std::string, std::string);
};

class vocabulary{
public:
    std::vector<card> json2vector(nlohmann::ordered_json p_json_daten);
    std::string vector2string(std::vector<card> card_vector1[7], int activeBox_p);
    void fillIn2();

    std::vector<card> card_vector1[7] = {
        {{"max",""}}, {{"max", ""}},{{"max",""}},{{"max",""}},{{"max",""}},{{"max",""}},{}
                                        };
    int activeBox = 1;
    int max[6] = {10, 15, 25, 40, 60, -1};
};

static vocabulary V;

#endif // HEADER_H
