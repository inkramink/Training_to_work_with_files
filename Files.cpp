#include <array>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdio.h>
#include <Windows.h>
using namespace std;
namespace fs = std::filesystem;

void work_with_student(int where_start, string folder, string* links, int kol, int file_num);
void create_student(ofstream& file);
void work_with_groups(string folder, string* links, int kol, int file_num);

string file_beauty(string s)
{
    std::string::size_type n;
    n = s.rfind("/");
    s = s.substr(n);
    s.erase(0, 1);
    return s;
}

int file_type(string s)
{
    s = file_beauty(s); 
    if (s.find("group") != std::string::npos)
        return 0;
    else if (s.find("student")!= std::string::npos)
        return 1;
    else return -1;
}

bool file_corr_format(fs::path s)
{
    string l = s.fs::path::generic_string(), format="    ";
    for (int i = l.size() - 4; i < l.size(); i++)
        format[i - l.size() + 4] = l[i];
    return format == ".txt" && file_type(l)>=0;
}

void beauty_files_out(string* links, int kol)
{
    cout << "Существуют файлы:\n";
    for(int i=0; i <kol;i++)
    { 
        if (file_type(links[i]) == 0)
            cout << i + 1 << ". Файл группы: "<<file_beauty(links[i])<<endl;
        if (file_type(links[i]) == 1)
            cout << i + 1 << ". Файл студента: " << file_beauty(links[i]) << endl;
    }
}

string* analyt_folder(string place, int& kol)
{
    kol = 0;
    for (fs::path p : fs::directory_iterator(place))
        if (file_corr_format(p)) kol++;
    if (kol == 0)
        return 0;
    else
    {
        string* links = new string[kol];
        int i = 0;
        for (fs::path p : fs::directory_iterator(place))
            if (file_corr_format(p)) 
            {
                links[i] = p.fs::path::generic_string();
                i++;
            }
        return links;
    }
}

void create_charact(string way, string place, int where_kol, int number_line)
{
    string charact;
    cout << "Пожалуйста, введите характеристику студента:\n";
    cin.get();
    getline(cin, charact);
    string s;
    ifstream file(way);
    ofstream copy(place + "/copy.txt");
    for (int i = 0; getline(file, s); i++)
    {   
        int kol;
        if(i==where_kol)
        {
            kol = stoi(s);
            kol++;
            copy << to_string(kol) << "\n";
        }
        else copy << s << "\n";
        if(i == number_line)
            copy << charact << "\n";
    }
    file.close();
    copy.close();
    file.open(place + "/copy.txt");
    copy.open(way);
    for (int i = 0; getline(file, s); i++)
        copy << s << "\n";
    cout << "Характеристика успешно добавлена!\n\n";
}

void edit_charact(string way, string place, int where_kol)
{
    string charact; int num_charact;
    cout << "Пожалуйста, введите номер характеристики студента, которую Вы хотите заменить:\n";
    cin >> num_charact;
    cout << "Пожалуйста, введите характеристику, на которую Вы хотите заменить:\n";
    cin.get();
    getline(cin, charact);
    string s;
    ifstream file(way);
    ofstream copy(place + "/copy.txt");
    for (int i = 0; getline(file, s); i++)
    {
        if (i == where_kol+num_charact)
            copy << charact << "\n";
        else copy << s << "\n";
    }
    file.close();
    copy.close();
    file.open(place + "/copy.txt");
    copy.open(way);
    for (int i = 0; getline(file, s); i++)
    copy << s << "\n";
    cout << "Характеристика успешно изменена!\n\n";
}

void delete_charact(string way, string place, int where_kol)
{
    int charact;
    cout << "Пожалуйста, введите номер характеристики студента:\n";
    cin >> charact;
    string s;
    ifstream file(way);
    ofstream copy(place + "/copy.txt");
    for (int i = 0; getline(file, s); i++)
    {
        int kol;
        if (i == where_kol + charact)
            continue;
        if (i == where_kol)
        {
            kol = stoi(s);
            kol--;
            copy << to_string(kol) << "\n";
        }
        else copy << s << "\n";
        
    }
    file.close();
    copy.close();
    file.open(place + "/copy.txt");
    copy.open(way);
    for (int i = 0; getline(file, s); i++)
        copy << s << "\n";
    cout << "Характеристика успешно удалена!\n\n";
}

void output_charact(int where_start, string way)
{
    cout << "\nХарактеристики студента:\n";
    fstream file(way); string s;
    for (int i = 0; i <= where_start; i++)
        getline(file, s);
    getline(file, s);
    int kol = stoi(s);
    for (int i = 0; i < kol; i++)
    {
        getline(file, s);
        cout<<i+1<< ". " << s << "\n";
    }
    file.close();
}

void work_with_files(string folder, string* links, int kol)
{
    int ans = 0, file_num=0;
    cout << "С каким файлом Вы бы хотели начать работу? Введите номер файла, с которым Вы бы хотели начать работу:\n";
    beauty_files_out(analyt_folder(folder, kol), kol);
    cin >> file_num; file_num -= 1;
    cout << "Вы открыли файл "<< file_beauty(links[file_num]) << ". Что Вы хотите сделать?\n";
    cout << "1. Редактировать\n2. Удалить\n";
    cin >> ans;
    if (ans == 1)
    {
        if (file_type(links[file_num]) == 0) work_with_groups(folder, links, kol, file_num);
        else work_with_student(0, folder, links, kol, file_num);
    }
    else 
    {
        remove(links[file_num].c_str());
        cout << "Файл успешно удален!\n";
    }
}

void work_with_student(int where_start, string folder, string* links, int kol, int file_num)
{
    int ans = 0;
    fstream file(links[file_num]);
    string name, charact; int kol_charact, num_strok=-1;
    while (num_strok != where_start)
    {
        getline(file, name);
        num_strok++;
    }
    cout << "Это студент по имени " << name << endl;
    getline(file, charact);
    kol_charact = stoi(charact);
    file.close();
    if (kol_charact == 0)
    {
        cout << "У студента нет ни одной характеристики. Создать?\n1. Да\n2. Вернуться\n";
        cin >> ans;
        if (ans == 1) create_charact(links[file_num], folder, where_start + 1, where_start + 1);
        else work_with_files(folder, links, kol);
    }
    else
    {
        output_charact(where_start, links[file_num]);
        cout << "Что Вы хотите сделать?\n1. Добавить\n2. Редактировать\n3. Удалить\n4. Вернуться\n";
        cin >> ans;
        if (ans == 1) create_charact(links[file_num], folder, where_start + 1, where_start + 1);
        if (ans == 2) edit_charact(links[file_num], folder, where_start + 1);
        if (ans == 3) delete_charact(links[file_num], folder, where_start+ 1);
        if (ans == 4) work_with_files(folder, links, kol);

    }

}

void output_students(string way)
{
    cout << "Студенты данной группы:\n";
    fstream file(way);
    string s; int kol_stud, kol;
    getline(file, s); 
    getline(file, s);
    kol_stud = stoi(s);
    for (int i = 0; i < kol_stud; i++)
    {
        getline(file, s);
        cout << i + 1 << ". " << s << "\n";
        getline(file, s);
        kol = stoi(s)+1;
        for (int j = 0; j < kol; j++)
            getline(file, s);
    }
}

void add_student(string way, string place)
{
    string s;
    ifstream file(way);
    ofstream copy(place + "/copy.txt");
    for (int i = 0; getline(file, s); i++)
    {
        int kol;
        if (i == 1)
        {
            kol = stoi(s);
            kol++;
            copy << to_string(kol) << "\n";
        }
        else copy << s << "\n";
    }
    cin.get();
    create_student(copy);
    file.close();
    copy.close();
    file.open(place + "/copy.txt");
    copy.open(way);
    for (int i = 0; getline(file, s); i++)
        copy << s << "\n";
    cout << "Студент успешно добавлен!\n\n";
    copy << "\n";
    file.close();
    copy.close();
}

int find_student(string way, int num_stud)
{
    fstream file(way);
    string s; int kol_stud, kol, num_strok=2;
    getline(file, s);
    getline(file, s);
    kol_stud = stoi(s);
    for (int i = 0; i < kol_stud; i++)
    {
        if (i == num_stud - 1)
            break;
        getline(file, s);
        getline(file, s);
        kol = stoi(s)+1;
        num_strok += 2;
        for (int j = 0; j < kol; j++)
        { 
            getline(file, s);
            num_strok++;
        }
    }
    return num_strok;
}

void delete_student(string way, string place)
{
    int ans, time=0;
    cout << "\n";
    cin >> ans;
    ans = find_student(way, ans);
    string s;
    ifstream file(way);
    ofstream copy(place + "/copy.txt");
    for (int i = 0; getline(file, s); i++)
    {
        int kol;
        if (i == ans)
            continue;
        if (i == ans + 1)
            time = stoi(s)+1;
        else if (time != 0) time--;
        else if (i == 1)
        {
            kol = stoi(s);
            kol--;
            copy << to_string(kol) << "\n";
        }
        else copy << s << "\n";
    }
    file.close();
    copy.close();
    file.open(place + "/copy.txt");
    copy.open(way);
    for (int i = 0; getline(file, s); i++)
        copy << s << "\n";
    cout << "Студент успешно удален!\n\n";
}

void work_with_groups(string folder, string* links, int kol, int file_num)
{
    int ans = 0;
    fstream file(links[file_num]);
    string name, charact; int kol_student;
    getline(file, name);
    cout << "Это файл группы " << name << endl;
    getline(file, charact);
    kol_student = stoi(charact);
    file.close();
    if (kol_student == 0)
    {
        cout << "В группе нет ни одного студента. Создать?\n1. Да\n2. Вернуться\n";
        cin >> ans;
        ofstream to_write(links[file_num]);
        if (ans == 1)
        {
            cin.get();
            create_student(to_write);
        }
        else work_with_files(folder, links, kol);
    }
    else
    {
        output_students(links[file_num]);
        cout << "Что Вы хотите сделать?\n1. Добавить студента\n2. Редактировать студента\n3. Удалить студента\n4. Вернуться\n";
        cin >> ans;
        if (ans == 1) add_student(links[file_num], folder);
        if (ans == 2)
        {
            cout << "Введите номер студента, с которым Вы бы хотели начать работу:\n";
            cin >> ans;
            work_with_student(find_student(links[file_num], ans), folder, links, kol, file_num);
        }
        if (ans == 3) delete_student(links[file_num], folder);
        if (ans == 4) work_with_files(folder, links, kol);

    }
}

void create_student(ofstream& file)
{
    string name; int kol;
    cout << "Пожалуйста, введите имя студента:\n";
    getline(cin, name);
    file << name << "\n";
    cout << "Пожалуйста, введите количество характеристик студента:\n";
    cin >> kol;
    file << kol << "\n";
    cin.get();
    for (int i = 0; i < kol; i++) 
    {
        string charact;
        cout << "Пожалуйста, введите характеристику студента:\n";
        getline(cin, charact);
        file << charact<<"\n";
    }
    file << "\n";
}

void create_file(string folder)
{
    int ans = 0, kol; string name, group_name;
    cout << "Вы хотите создать...\n1. Файл группы\n2. Файл студента\n";
    cin >> ans;
    cout << "Как Вы хотите назвать файл?\n";
    cin.get();
    getline(cin, name);
    if (ans == 1)
    {
        ofstream file("group_" + name + ".txt");
        cout << "Введите название группы:\n";
        getline(cin, group_name);
        file << group_name << "\n";
        cout << "Введите количество студентов в группе:\n";
        cin >> kol;
        file << kol << "\n";
        cin.get();
        for (int i = 0; i < kol; i++)
            create_student(file);
        file.close();
    }
    else
    {
        ofstream file("student_" + name + ".txt");
        create_student(file);
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    int kol = 0; bool loop = true;
    cout << "Пожалуйста, введите путь к файлу проекта (непосредственно в папку, где лежит файл Files.cpp):\n";
    string place;
    cin >> place;
    while(loop)
    {
        int ans = 0;
        if (analyt_folder(place, kol)) 
        {
            beauty_files_out(analyt_folder(place, kol), kol);
            cout << "Введите номер действия, соответствующего Вашим желаниям:\n1. Создать файл\n2. Работать с существующими файлами\n3. Выйти\n";
            cin >> ans;
            if (ans == 1) create_file(place);
            else if (ans == 2) work_with_files(place, analyt_folder(place, kol), kol);
            else if (ans == 3) loop = false;
            else cout << "Кажется, Вы что-то не поняли. Попробуйте еще раз";
        }
        else 
        {
            
            cout << "В папке нет файлов групп или учеников. Создайте один из таких файлов для работы с данной программой\n";
            cout << "Введите номер действия, соответствующего Вашим желаниям:\n1. Создать файл\n2. Выйти\n";
            cin >> ans;
            if (ans == 1) create_file(place);
            else if (ans == 2) loop = false;
            else cout << "Кажется, Вы что-то не поняли. Попробуйте еще раз";
        }
    }
    return 0;
}