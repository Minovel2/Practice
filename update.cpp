#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <conio.h>
#include "dependencies/include/nlohmann/json.hpp"
#include <map>
using namespace std;
string path1, path2, path3;
string searchFilm = "", searchName = "";
int sortMode = -1;

struct soloCinema
{
    string name;
    string film;
};

struct cinema
{
    string name;
    vector<string> films;
};

vector<cinema> convertToCinema(const vector<soloCinema>& soloCinemas)
{
    map<string, vector<string>> cinemaMap;
    for (const auto& solo : soloCinemas)
    {
        cinemaMap[solo.name].push_back(solo.film);
    }

    vector<cinema> cinemas;
    for (const auto& pair : cinemaMap)
    {
        cinemas.push_back({ pair.first, pair.second });
    }

    return cinemas;
}

vector<soloCinema> convertToSoloCinema(vector<cinema>& cinemas) {
    vector<soloCinema> soloCinemas;

    for (vector<cinema>::iterator i = cinemas.begin(); i < cinemas.end(); i++) {
        cinema& c = *i;
        vector<string> films = c.films;
        for (vector<string>::iterator j = films.begin(); j < films.end(); j++) {
            soloCinema s = { c.name, *j };
            soloCinemas.push_back(s);
        }
    }
    return soloCinemas;
}

string toEng(string& str) {
    string s;
    const string rus = "йцукенгшщзхъфывапролджэячсмитьбю";
    const string eng = "qwertyuiop[]asdfghjkl;'zxcvbnm,.";
    const string rusUp = "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
    const string engUp = "QWERTYUIOP()ASDFGHJKL:!ZXCVBNM+=";

    for (size_t i = 0; i < str.length(); i++)
    {
        size_t pos1 = rus.find(str[i]);
        size_t pos2 = rusUp.find(str[i]);

        if (pos1 != string::npos) {
            s += "\\";
            s += eng[pos1];
        }
        else if (pos2 != string::npos) {
            s += "\\";
            s += engUp[pos2];
        }
        else {
            s += str[i];
        }
    }
    return s;
}

string toRus(string& str) {
    string s;
    const string rus = "йцукенгшщзхъфывапролджэячсмитьбю";
    const string eng = "qwertyuiop[]asdfghjkl;'zxcvbnm,.";
    const string rusUp = "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
    const string engUp = "QWERTYUIOP()ASDFGHJKL:!ZXCVBNM+=";

    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '\\') {
            size_t pos1 = eng.find(str[i + 1]);
            size_t pos2 = engUp.find(str[i + 1]);
            if (pos1 != string::npos) {
                s += rus[pos1];
                i++;
            }
            else if (pos2 != string::npos) {
                s += rusUp[pos2];
                i++;
            }
        }
        else {
            s += str[i];
        }
    }
    return s;
}

void beforePrint() {
    cout << " id | ";
    cout << "Имя кинотеатра       | ";
    cout << "Фильм                | ";
    cout << "\n---------------------------------------------------\n";
}
void beforePrintInFile(fstream& fs) {
    fs << " id | ";
    fs << "Имя кинотеатра       | ";
    fs << "Фильм                | ";
    fs << "\n---------------------------------------------------\n";
}

void printOneFilm(int id, string name, string film) {
    cout << " " << setw(2) << id << " | ";
    cout << setw(20) << toRus(name) << " | ";
    cout << setw(20) << toRus(film) << " | " << endl;
}
void saveOneFilm(int id, string name, string film, fstream& fs) {
    fs << left << " " << setw(2) << id << " | ";
    fs << setw(20) << toRus(name) << " | ";
    fs << setw(20) << toRus(film) << " | " << endl;
}

void printCinemas(vector<cinema>& cinemas) {
    int k = 1;
    beforePrint();
    for (vector<cinema>::iterator i = cinemas.begin(); i < cinemas.end(); i++) {
        cinema& j = *i;
        for (vector<string>::iterator i = j.films.begin(); i < j.films.end(); i++) {
            string& film = *i;
            printOneFilm(k, j.name, film);
            k++;
        }
    }
}

void printCinemas(vector<soloCinema>& cinemas) {
    int k = 1;
    beforePrint();
    for (vector<soloCinema>::iterator i = cinemas.begin(); i < cinemas.end(); i++) {
        soloCinema& c = *i;
        printOneFilm(k, c.name, c.film);
        k++;
    }
}

void saveCinemas(vector<soloCinema>& cinemas, fstream& fs) {
    int k = 1;
    for (soloCinema c : cinemas) {
        saveOneFilm(k, c.name, c.film, fs);
        k++;
    }
}

void toEng(vector<cinema>& cinemas) {
    for (vector<cinema>::iterator i = cinemas.begin(); i < cinemas.end(); i++) {
        cinema& j = *i;
        j.name = toEng(j.name);
          for (vector<string>::iterator k = j.films.begin(); k < j.films.end(); k++) {
              string& j = *k;
              j = toEng(j);
          }
    }
}

void toRus(vector<cinema>& cinemas) {
    for (vector<cinema>::iterator i = cinemas.begin(); i < cinemas.end(); i++) {
        cinema& j = *i;
        j.name = toRus(j.name);
        for (vector<string>::iterator k = j.films.begin(); k < j.films.end(); k++) {
            string& j = *k;
            j = toRus(j);
        }
    }
}

void to_json(nlohmann::json& j, const cinema& p) {
    j = nlohmann::json{ {"name", p.name}, {"films", p.films} };
}

void from_json(const nlohmann::json& j, cinema& p) {
    j.at("name").get_to(p.name);
    j.at("films").get_to(p.films);
}

void removeProcess(vector<soloCinema>& soloCinemas, vector<cinema>& cinemas) {
    printCinemas(soloCinemas);
    int k = 0, id = 0;
    cout << "Введите номер строки, которую хотите удалить: ";
    cin >> id;
    if (std::cin.fail() || id < 0) {
        std::cout << "Ошибка.";
        std::cin.clear(); // очистить состояние ошибки
        std::cin.ignore(10000, '\n'); // очистить буфер ввода
        return;
    }

    for (vector<soloCinema>::iterator i = soloCinemas.begin(); i < soloCinemas.end(); i++) {
        k++;
        if (k == id) {
            soloCinemas.erase(i);
            cout << "Удалено!";
            cinemas = convertToCinema(soloCinemas);
            return;
        }
    }
    cout << "Не удалено, возможно такой строки не существует";
}

bool addFilm(vector<cinema>& cinemas, string name, string film) {
    vector<soloCinema> soloCinemas = convertToSoloCinema(cinemas);
    for (vector<soloCinema>::iterator i = soloCinemas.begin(); i < soloCinemas.end(); i++) {
        soloCinema& c = *i;
        if (c.name == name && c.film == film) {
            return false;
        }
    }
    soloCinema s = {name, film};
    soloCinemas.push_back(s);
    cinemas = convertToCinema(soloCinemas);
    return true;
}

bool updateCinema(vector<soloCinema>& soloCinemas, vector<cinema>& cinemas) {
    string name, film;
    cout << "Введите название кинотеатра, который хотите добавить/обновить: ";
    if (cin.peek() == '\n') cin.ignore();
    getline(cin, name);
    if (cin.fail()) {
        cout << "Ошибка";
        return false;
    }
    cout << "Введите название фильма, который хотите добавить: ";
    if (cin.peek() == '\n') cin.ignore();
    getline(cin, film);
    if (cin.fail()) {
        cout << "Ошибка";
        return false;
    }
    bool isAdded = addFilm(cinemas, name, film);
    if (isAdded) {
        cout << "Добавлено!";
        soloCinemas = convertToSoloCinema(cinemas);
    }
    else {
        cout << "Не добавлено, вводимые данные уже существуют.";
    }
    return true;
}

void saveToFile(vector<cinema>& cinemas) {
    toEng(cinemas);
    nlohmann::json j = cinemas;
    fstream fs;
    string p;
    cout << "Введите название файла без расширения: ";
    cin >> p;
    p += ".json";
    fs.open(p, fstream::out);
    fs << j;
    fs.close();
    toRus(cinemas);
}

void sortCinemasByName(vector<soloCinema>& cinemas, bool mode = 1) {
    // mode == 1 сортирует по алфавиту от А до Я
    if (mode) {
        sort(cinemas.begin(), cinemas.end(), [](soloCinema& c1, soloCinema& c2) { return c1.name < c2.name; });
    }
    else {
        sort(cinemas.begin(), cinemas.end(), [](soloCinema& c1, soloCinema& c2) { return c1.name > c2.name; });
    }
    sortMode = mode;
}

void sortCinemasByfilms(vector<soloCinema>& cinemas, bool mode = 1) {
    // mode == 1 сортирует по алфавиту от А до Я
    if (mode) {
        sort(cinemas.begin(), cinemas.end(), [](soloCinema& c1, soloCinema& c2) { return c1.film < c2.film; });
    }
    else {
        sort(cinemas.begin(), cinemas.end(), [](soloCinema& c1, soloCinema& c2) { return c1.film > c2.film; });
    }
    sortMode = 2 + mode;
}

void generateResult1(vector<soloCinema>& cinemas, bool showText) {
    int k = 0;
    fstream fs;
    fs.open(path1 + ".txt", fstream::out);
    if (showText) {
        cout << "\n1. Введите фильм, который будет искаться в репертуаре кинотеатров: ";
        if (cin.peek() == '\n') cin.ignore();
        getline(cin, searchFilm);
        cout << "Кинотеатры, содержащие фильм " << searchFilm << ":" << endl;
        beforePrint();
    }
    beforePrintInFile(fs);

    for (vector<soloCinema>::iterator i = cinemas.begin(); i != cinemas.end(); i++) {
        soloCinema& c = *i;
        if (c.film == searchFilm) {
            k++;
            if (showText)
                printOneFilm(k, c.name, c.film);

            saveOneFilm(k, c.name, c.film, fs);
        }
    }
    fs.close();
}

void generateResult2(vector<cinema>& cinemas, bool showText) {
    vector<vector<soloCinema>> list;
    int k = 0;
    if (showText)
        cout << "\n2. Список кинотеатров с одинаковым репертуаром:" << endl;

    for (vector<cinema>::iterator i = cinemas.begin(); i < cinemas.end(); i++) {
        cinema& c = *i;
        sort(c.films.begin(), c.films.end());
    }

    vector<vector<cinema>::iterator> dontCheckList;
    int findedCinemas = 0;
    for (vector<cinema>::iterator i = cinemas.begin(); i < cinemas.end(); i++) {
        vector<soloCinema> soloCinemas;
        findedCinemas = 0;
        k = 0;
        cinema c = *i;

        for (vector<cinema>::iterator j = i + 1; j < cinemas.end(); j++) {
            cinema c1 = *j;
            if (c.films == c1.films && std::find(dontCheckList.begin(), dontCheckList.end(), j) == dontCheckList.end()) {
                findedCinemas++;

                if (findedCinemas == 1) {
                    dontCheckList.push_back(i);
                    for (vector<string>::iterator f = c.films.begin(); f < c.films.end(); f++) {
                        k++;
                        string& film = *f;
                        soloCinemas.push_back({ c.name, film });
                    }
                }

                dontCheckList.push_back(j);
                for (vector<string>::iterator f = c1.films.begin(); f < c1.films.end(); f++) {
                    k++;
                    string& film = *f;
                    soloCinemas.push_back({ c1.name, film });
                }
            }
        }
        if (soloCinemas.size() > 0) {
            list.push_back(soloCinemas);
        }
    }

    fstream fs;
    k = 0;
    fs.open(path2 + ".txt", fstream::out);
    for (auto& soloCinemas : list) {
        if (sortMode == 0 || sortMode == 1) {
            sortCinemasByName(soloCinemas, sortMode);
        }
        else if (sortMode == 2 || sortMode == 3) {
            sortCinemasByfilms(soloCinemas,sortMode - 2);
        }

        if (showText) {
            cout << "Группа #" << ++k << endl;
            printCinemas(soloCinemas);
        }

        fs << "Группа #" << k << endl;
        beforePrintInFile(fs);
        saveCinemas(soloCinemas, fs);
    }
    fs.close();
}

void generateResult3(vector<soloCinema>& cinemas, bool showText) {
    int k = 0;
    fstream fs;
    fs.open(path3 + ".txt", fstream::out);
    if (showText) {
        cout << "\n3. Имя кинотеатра, список фильмов которого вывести в консоль: ";
        if (cin.peek() == '\n') cin.ignore();
        getline(cin, searchName);
        cout << "Фильмы кинотеатра " << searchName << ":\n";
        beforePrint();
    }
    beforePrintInFile(fs);

    for (vector<soloCinema>::iterator i = cinemas.begin(); i != cinemas.end(); i++) {
        soloCinema& c = *i;
        if (c.name == searchName) {
            k++;
            if (showText)
                printOneFilm(k, c.name, c.film);

            saveOneFilm(k, c.name, c.film, fs);
        }
    }
    fs.close();
}

void updateResults(vector<soloCinema>& soloCinemas, vector<cinema>& cinemas) {
    generateResult1(soloCinemas, 0);
    generateResult2(cinemas, 0);
    generateResult3(soloCinemas, 0);
}

const int buttonsCount = 10;

void showMenu(int menu, vector<soloCinema>& soloCinemas) {
    string* text = new string[buttonsCount]{ "1. Найти кинотеатры, среди которых есть вводимый фильм", "2. Найти кинотеатры с одинаковым репертуаром", "3. Найти список фильмов вводимого кинотеатра", "4. Отсортировать по названию кинотеатра (по возрастанию)", "5. Отсортировать по названию кинотеатра (по убыванию)", "6. Отсортировать по фильмам (по возрастанию)", "7. Отсортировать по фильмам (по убыванию)", "8. Добавить фильм/кинотеатр", "9. Удалить фильм/кинотеатр", "10. Сохранить в файл"};
    system("cls");
    printCinemas(soloCinemas);
    cout << endl;
    for (int i = 0; i < buttonsCount; i++) {
        if (i == menu) {
            cout << "-->  ";
        }
        cout << text[i];
        if (i == menu) {
            cout << "  <--";
        }
        cout << endl;
    }
}

void startCycle(vector<cinema>& cinemas, vector<soloCinema>& soloCinemas, int& menu) {
    showMenu(menu, soloCinemas);
    int ch;
    bool cycle = true;
    while (cycle) {
        ch = _getch();
        if (ch == 115 || ch == 80 || ch == 50) {
            menu = (menu + 1) % buttonsCount;
        }
        else if (ch == 119 || ch == 72 || ch == 56) {
            if (menu == 0)
                menu = buttonsCount;

            menu = (menu - 1) % buttonsCount;
        }

        if (ch != 13) {
            showMenu(menu, soloCinemas);
        }
        else {
            cycle = false;
            system("cls");
            switch (menu)
            {
            case 0:
                generateResult1(soloCinemas, 1);
                break;
            case 1:
                generateResult2(cinemas, 1);
                break;
            case 2:
                generateResult3(soloCinemas, 1);
                break;
            case 3:
                sortCinemasByName(soloCinemas);
                updateResults(soloCinemas, cinemas);
                printCinemas(soloCinemas);
                break;
            case 4:
                sortCinemasByName(soloCinemas, 0);
                updateResults(soloCinemas, cinemas);
                printCinemas(soloCinemas);
                break;
            case 5:
                sortCinemasByfilms(soloCinemas);
                updateResults(soloCinemas, cinemas);
                printCinemas(soloCinemas);
                break;
            case 6:
                sortCinemasByfilms(soloCinemas, 0);
                updateResults(soloCinemas, cinemas);
                printCinemas(soloCinemas);
                break;
            case 7:
                updateCinema(soloCinemas, cinemas);
                updateResults(soloCinemas, cinemas);
                break;
            case 8:
                removeProcess(soloCinemas, cinemas);
                updateResults(soloCinemas, cinemas);
                break;
            case 9:
                saveToFile(cinemas);
                break;
            }
            cinemas = convertToCinema(soloCinemas);
        }
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    system("chcp 1251");
    using namespace std;
    cout << setiosflags(ios::left);
    
    fstream fs;
    vector<cinema> cinemas;
    
    fs.open("defaultNames.txt", fstream::in);
    fs >> path1;
    fs >> path2;
    fs >> path3;
    fs.close();
    cout << "Список кинотетров, в названии которых есть фильм, название которого введено с клавиатуры, запишется в файл: " << path1 << ".txt\nСписок кинотеатров с одинаковым репертуаром запишется в файл: " << path2 << ".txt\nСписок фильмов, демонстрирующихся в кинотеатре, название которого вводится с клавиатуры, запишется в файл: " << path3 << ".txt" << endl;
    cout << "Желаете изменить файлы, в которые запишется результат? 1 - да, 0 - нет: ";
    bool mode;
    cin >> mode;
    if (cin.fail()) {
        cout << "Ошибка";
        return 0;
    }

    if (mode) {
        cout << "Введите имя файла 1: ";
        cin >> path1;
        cout << "Введите имя файла 2: ";
        cin >> path2;
        cout << "Введите имя файла 3: ";
        cin >> path3;

        fs.open("defaultNames.txt", fstream::out);
        fs << path1 << endl;
        fs << path2 << endl;
        fs << path3 << endl;
        fs.close();
    }

    cout << "Ввод исходных данных с клавиатуры? 1 - да, 0 - нет: ";
    cin >> mode;

    if (mode) {
        unsigned int cinemasCount, filmsCount;
        cout << "Сколько кинотеатров хотите добавить?: ";
        cin >> cinemasCount;
        if (cinemasCount > 0) {
            string film, name;
            getline(cin, name);
            for (int i = 0; i < cinemasCount; i++) {
                cout << "Введите название кинотеатра " << i + 1 << ": ";
                cin.sync();
                getline(cin, name);

                cout << "Сколько фильмов будет в кинотеатре?: ";
                cin >> filmsCount;
                cin.sync();
                getline(cin, film);
                for (int j = 0; j < filmsCount; j++) {
                    cout << "Введите фильм " << j + 1 << ": ";
                    cin.sync();

                    getline(cin, film);
                    addFilm(cinemas, toEng(name), toEng(film));

                }
            }
            printCinemas(cinemas);
        }

        nlohmann::json j = cinemas;

        bool mode1;
        cout << "Желаете сохранить как исходные данные в файл? 1 - да, 0 - нет: ";
        cin >> mode1;

        if (mode1) {
            string p;
            cout << "Введите название файла без расширения: ";
            cin >> p;
            p += ".json";
            fs.open(p, fstream::out);
            fs << j;
            fs.close();
        }
    }
    else {
        string p;
        cout << "Введите название файла c исходными данными (без расширения): ";
        cin >> p;
        p += ".json";
        fs.open(p, fstream::in);
        if (!fs.is_open()) {
            cout << "Ошибка открытия файла, возможно, файла с таким именем не существует.";
            return 0;
        }
        else {
            nlohmann::json j;
            fs >> j;
            cinemas = j;
            printCinemas(cinemas);
        }

        fs.close();
    }

    toRus(cinemas);
    vector<soloCinema> soloCinemas = convertToSoloCinema(cinemas);
    int menu = 0;
    while (true) {
        startCycle(cinemas, soloCinemas, menu);
        _getch();
    }

    return 0;
}
