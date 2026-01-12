#include "Header.h"
#include <filesystem>
#include <iostream>

using namespace std;
using namespace std::filesystem;

// Преобразование текстового файла в двоичный
void createBinFile(string& textFile, string& binFile) {
    ofstream fbout = openFile<ofstream>(binFile, ios::binary);
    ifstream fin = openFile<ifstream>(textFile);
    ReaderTicket record;

    while (fin >> record.ticketNumber) {
        fin.ignore();
        fin.getline(record.fullName, max_len);
        fin.getline(record.address, max_len);
        fbout.write((char*)&record, sizeof(ReaderTicket));
    }
    closeFile(fbout);
    closeFile(fin);
    cout << "Двоичный файл '" << binFile << "' создан." << endl;
}

// Вывод всех записей двоичного файла
void outBinFile(string& binFile) {
    ifstream fbin = openFile<ifstream>(binFile, ios::binary);
    ReaderTicket record;
    while (fbin.read((char*)&record, sizeof(ReaderTicket))) {
        cout << "Билет: " << record.ticketNumber
            << ", ФИО: " << record.fullName
            << ", Адрес: " << record.address << endl;
    }
    closeFile(fbin);
}

// Сохранение данных двоичного файла в текстовом
void binToTxt(string& binFile) {
    ifstream fbin = openFile<ifstream>(binFile, ios::binary);
    string textFile = binFile + ".txt";
    ofstream fout = openFile<ofstream>(textFile);
    ReaderTicket record;

    while (fbin.read((char*)&record, sizeof(ReaderTicket))) {
        fout << record.ticketNumber << '\n'
            << record.fullName << '\n'
            << record.address << '\n';
    }
    closeFile(fbin);
    closeFile(fout);
    cout << "Данные сохранены в '" << textFile << "'" << endl;
}

// Получение записи по индексу
ReaderTicket getRecordByIndex(string& binFile, int index) {
    ifstream fin = openFile<ifstream>(binFile, ios::binary);
    fin.seekg(0, ios::end);
    int fileSize = fin.tellg();
    int recordCount = fileSize / sizeof(ReaderTicket);

    if (index < 0 || index >= recordCount) {
        closeFile(fin);
        cout << "Запись с индексом " << index << " не существует." << endl;
        ReaderTicket empty;
        empty.ticketNumber = -1;
        return empty;
    }

    fin.seekg(index * sizeof(ReaderTicket), ios::beg);
    ReaderTicket record;
    fin.read((char*)&record, sizeof(ReaderTicket));
    closeFile(fin);
    return record;
}

// Удаление по ключу: замена на последнюю запись
void delRecordByKey1(string& binFile, int key) {
    fstream fbio = openFile<fstream>(binFile, ios::binary | ios::in | ios::out);
    fbio.seekg(0, ios::end);
    int fileSize = fbio.tellg();
    int recordCount = fileSize / sizeof(ReaderTicket);

    ReaderTicket record;
    int targetIndex = -1;

    for (int i = 0; i < recordCount; ++i) {
        fbio.seekg(i * sizeof(ReaderTicket));
        fbio.read((char*)&record, sizeof(ReaderTicket));
        if (record.ticketNumber == key) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) {
        cout << "Запись с ключом " << key << " не найдена!" << endl;
        closeFile(fbio);
        return;
    }

    // Читаем последнюю запись
    fbio.seekg((recordCount - 1) * sizeof(ReaderTicket));
    ReaderTicket lastRecord;
    fbio.read((char*)&lastRecord, sizeof(ReaderTicket));

    // Записываем её на место удаляемой
    fbio.seekp(targetIndex * sizeof(ReaderTicket));
    fbio.write((char*)&lastRecord, sizeof(ReaderTicket));

    // Обрезаем файл
    resize_file(binFile, (recordCount - 1) * sizeof(ReaderTicket));
    closeFile(fbio);
    cout << "Запись с ключом " << key << " удалена." << endl;
}

// Удаление по ключу: через временный файл
void delRecordByKey2(string& binFile, int key) {
    ifstream fin = openFile<ifstream>(binFile, ios::binary);
    string tempFile = "temp_" + binFile;
    ofstream fout = openFile<ofstream>(tempFile, ios::binary);

    ReaderTicket record;
    bool found = false;

    while (fin.read((char*)&record, sizeof(ReaderTicket))) {
        if (record.ticketNumber == key) {
            found = true;
        }
        else {
            fout.write((char*)&record, sizeof(ReaderTicket));
        }
    }

    closeFile(fin);
    closeFile(fout);

    if (found) {
        remove(binFile.c_str());
        rename(tempFile.c_str(), binFile.c_str());
        cout << "Запись с ключом " << key << " успешно удалена." << endl;
    }
    else {
        remove(tempFile.c_str());
        cout << "Запись с ключом " << key << " не найдена!" << endl;
    }
}

// Поиск и вывод записи по ключу
void displayRecordByKey(string& binFile, int key) {
    ifstream fin = openFile<ifstream>(binFile, ios::binary);
    fin.seekg(0, ios::end);
    int fileSize = fin.tellg();
    int recordCount = fileSize / sizeof(ReaderTicket);
    fin.seekg(0);

    ReaderTicket record;
    bool found = false;
    for (int i = 0; i < recordCount; ++i) {
        fin.read((char*)&record, sizeof(ReaderTicket));
        if (record.ticketNumber == key) {
            cout << "Найдена запись:\n"
                << "Индекс: " << i << "\n"
                << "Билет: " << record.ticketNumber << "\n"
                << "ФИО: " << record.fullName << "\n"
                << "Адрес: " << record.address << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Запись с ключом " << key << " не найдена!" << endl;
    }
    closeFile(fin);
}

// Добавление записи в конец
void addRecordToBinFile(string& binFile, const ReaderTicket& newRecord) {
    ofstream fout = openFile<ofstream>(binFile, ios::binary | ios::app);
    fout.write((char*)&newRecord, sizeof(ReaderTicket));
    closeFile(fout);
    cout << "Запись с билетом " << newRecord.ticketNumber << " добавлена." << endl;
}