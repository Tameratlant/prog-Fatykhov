#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

#define TOP 3
#define IN "input.txt"
#define OUT "output.txt"

struct Stat {
    int count;
    std::string word;
};

std::map<std::string, int> Map;
std::string word;

void prepare(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (ispunct(str[str.length() - 1])) //ispunct - функция, проверяющая знак ли это припинания.
        str.pop_back();
};




//Все выше по сути из презентации

bool cmp(const Stat& a, const Stat& b) {
    return a.count > b.count;
};

void write(std::vector<Stat> array) {
    string str;
    int counter = 0;
    std::ofstream fout(OUT);
    if (fout.is_open()) {
        
        for (int i = 0; i < array.size(); i++) {
            str = array[i].word;                //Небольшой наворот, чтобы союзы, предлоги, местоимения и пр. не считались за слова.  
            if (str.length() > 2) {             //По хорошему можно было создать словарь с такими словами и проверять, но данный проект больше не в этом
                fout << array[i].word << '\n';
                counter++;
            }
            if (counter == TOP) break;
        }
    }
    else {
        cout << "Файл OUT не может быть открыт!\n";
        exit(-1);
    }
    fout.close();
}


int main(int argc, char* argv[])
{
    std::vector<Stat> array;
    setlocale(LC_ALL, "rus"); // корректное отображение Кириллицы
    char buff[50]; // буфер промежуточного хранения считываемого из файла текста
    ifstream fin(IN); 
    if (!fin.is_open()) {
        // если файл не открыт
        cout << "Файл IN не может быть открыт!\n"; // сообщить об этом
        exit(-1);
    }
    else
    {
        /*
        while (!fin.eof()) {
            fin.getline(buff, 50); // считали строку из файла
            cout << buff << endl; // напечатали эту строку
        }
        */
        while (!fin.eof()) {        //пока файл не кончится, считываем, обрабатываем и записываем слова
            fin >> word;
            prepare(word);
            Map[word]++;
        }

        for (const auto& i : Map) {
            array.push_back({ i.second, i.first });
        }
        std::sort(array.begin(), array.end(), cmp);
        
        /*
        fin >> buff; // считали первое слово из файла
        cout << buff << endl; // напечатали это слово
        */
        
        fin.close(); // закрываем файлы
    }
    write(array); //вызываем функцию, сортирующую данные и пишущую  в файл
    return 0;
}
