#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

void list_add();
void list_edit();
void list_delete();
void list_search();
void list_print();
void file_load();
void file_save();
void out_put(int num);

struct book
{
    string name;
    string version;
    string auther;
    string pubisher;
    string isbn;
    string date;
};

vector<book> book_list;

int main()
{
    cout << "����ͼ����Ϣ����ϵͳv1.2" << endl << "By: Direktor 2020/10/4" << endl << endl;
    int key = -1;
    file_load();
    cout << setiosflags(ios::left);
    while(key != 0)
    {
        cout << "-------------------------���˵�-------------------------" << endl;
        cout << "1 - ¼����ͼ����Ϣ" << endl << "2 - �޸�ͼ����Ϣ" << endl << "3 - ɾ��ͼ����Ϣ" << endl;
        cout << "4 - ����ͼ����Ϣ" << endl << "5 - ��ʾ����ͼ��" << endl << "0 - �˳�����ϵͳ" << endl;
        cin >> key;
        if(key == 1)
            list_add();
        if(key == 2)
            list_edit();
        if(key == 3)
            list_delete();
        if(key == 4)
            list_search();
        if(key == 5)
            list_print();
        file_save();
    }
}

void list_add()
{
    book tmp;
    cout << "------------------------��Ϣ¼��------------------------" << endl;
    cout << "���������ƣ�";
    cin >> tmp.name;
    cout << "������汾��";
    cin >> tmp.version;
    cout << "���������ߣ�";
    cin >> tmp.auther;
    cout << "����������磺";
    cin >> tmp.pubisher;
    cout << "������ISBN��ţ�";
    cin >> tmp.isbn;
    cout << "������������ڣ�YYYY/MM/DD����";
    cin >> tmp.date;
    cout << "¼��ɹ�" << endl << endl;
    book_list.push_back(tmp);
}

void list_edit()
{
    int num, key;
    cout << "------------------------��Ϣ�޸�------------------------" << endl;
    cout << "������Ҫ�޸ĵ�ͼ����:";
    cin >> num;
    if(num > book_list.size() || num < 1)
    {
        cout << "���޴��飬�޸�ʧ��" << endl << endl;
        return;
    }
    out_put(-1);
    out_put(num - 1);
    cout << "��ѡ��Ҫ�޸ĵ���Ŀ��" << endl;
    cout << "1 - �޸�����" << endl << "2 - �޸İ汾" << endl << "3 - �޸�����" << endl;
    cout << "4 - �޸ĳ�����" << endl << "5 - �޸�ISBN" << endl << "6 - �޸ĳ�������" << endl;
    cin >> key;
    cout << "��������Ϣ��";
    if(key == 1)
        cin >> book_list[num-1].name;
    if(key == 2)
        cin >> book_list[num-1].version;
    if(key == 3)
        cin >> book_list[num-1].auther;
    if(key == 4)
        cin >> book_list[num-1].pubisher;
    if(key == 5)
        cin >> book_list[num-1].isbn;
    if(key == 6)
        cin >> book_list[num-1].date;
    cout << "�޸����" << endl << endl;
}

void list_delete()
{
    int num;
    char key = 0;
    cout << "------------------------��Ϣɾ��------------------------" << endl;
    cout << "������Ҫɾ����ͼ����:";
    cin >> num;
    if(num > book_list.size() || num < 1)
    {
        cout << "���޴��飬ɾ��ʧ��" << endl << endl;
        return;
    }
    out_put(-1);
    out_put(num - 1);
    cout << "��Yɾ������" << endl;
    cin >> key;
    if(key == 'Y' || key == 'y')
    {
        book_list.erase(book_list.begin() + num);
        cout << "ɾ���ɹ�" << endl << endl;
    }
    else
    {
        cout << "ɾ���ɹ�" << endl << endl;
    }
    
}

void list_search()
{
    int key;
    string contant;
    cout << "------------------------��Ϣ����------------------------" << endl;
    cout << "1 - ����������" << endl << "2 - ���汾����" << endl << "3 - ����������" << endl;
    cout << "4 - ������������" << endl << "5 - ��ISBN����" << endl;
    cout << "6 - ��������������" << endl << "7 - ȫ������" << endl;
    cin >> key;
    cout << "��������������:";
    cin >> contant;
    out_put(-1);
    if(key == 7)
    {
        for(int i = 0; i < book_list.size(); i++)
            if(book_list[i].name == contant || book_list[i].version == contant\
            || book_list[i].auther == contant || book_list[i].pubisher == contant\
            || book_list[i].isbn == contant || book_list[i].date == contant)
                out_put(i);
        cout << endl;
        return;
    }
    for(int i = 0; i < book_list.size(); i++)
        if(key == 1 && book_list[i].name == contant || key == 2 && book_list[i].version == contant\
        || key == 3 && book_list[i].auther == contant || key == 4 && book_list[i].pubisher == contant\
        || key == 5 && book_list[i].isbn == contant || key == 6 && book_list[i].date == contant)
            out_put(i);
        cout << endl;
}

void list_print()
{
    cout << "------------------------ͼ���б�------------------------" << endl;
    out_put(-1);
    for(int i = 0; i < book_list.size(); i++)
        out_put(i);
    cout << endl;
}

void file_load()
{
    ifstream lib("lib.txt");
    if(!lib)
    {
        cout << "�ļ���ȡʧ��" << endl << endl;
        lib.close();
        return;
    }
    while(!lib.eof())
    {
        book tmp;
        lib >> tmp.name >> tmp.version >> tmp.auther >> tmp.pubisher >> tmp.isbn >> tmp.date;
        if(tmp.name.size() != 0)
            book_list.push_back(tmp);
    }
    lib.close();
    cout << "�ļ��Ѷ�ȡ" << endl << endl;
}

void file_save()
{
    ofstream lib("lib.txt");
    if(!lib)
    {
        cout << "�ļ�д��ʧ��" << endl << endl;
        lib.close();
        return;
    }
    for(auto i = book_list.begin(); i != book_list.end(); i++)
    {
        lib << i->name << " " << i->version << " ";
        lib << i->auther << " " << i->pubisher << " ";
        lib << i->isbn << " " << i->date << endl;
    }
    lib.close();
    cout << "�ļ��ѱ���" << endl << endl;
}

void out_put(int num)
{
    if(num == -1)
    {
        cout << endl << setw(10) << "���" << setw(20) << "����" << setw(10) << "�汾" << setw(20) << "����";
        cout << setw(20) << "������" << setw(20) << "ISBN" << setw(20) << "��������" << endl;
        return;
    }
    cout << setw(10) << num + 1;
    cout << setw(20) << book_list[num].name << setw(10) << book_list[num].version;
    cout << setw(20) << book_list[num].auther << setw(20) << book_list[num].pubisher;
    cout << setw(20) << book_list[num].isbn << setw(20) << book_list[num].date << endl;
}