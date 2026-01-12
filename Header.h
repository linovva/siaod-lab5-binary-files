#ifndef HEADER1_H
#define HEADER1_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

const int max_len = 50;

// Структура записи: читательский абонемент
struct ReaderTicket {
    int ticketNumber;
    char fullName[max_len];
    char address[max_len];
};

// Функции открытия файла
template<typename T>
inline T openFile(const string& fileName, ios_base::openmode mode = ios_base::openmode()) {
    T file(fileName, mode);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл: " << fileName << endl;
        exit(1);
    }
    return file;
}

template<typename T>
inline void closeFile(T& file) {
    if (!file.good() && !file.eof()) {
        cout << "Ошибка при работе с файлом." << endl;
    }
    file.close();
}

// Объявления функций
void createBinFile(string& textFileName, string& binFileName);
void outBinFile(string& binFile);
void binToTxt(string& binFile);
ReaderTicket getRecordByIndex(string& binFile, int index);
void delRecordByKey1(string& binFile, int key);
void delRecordByKey2(string& binFile, int key);
void displayRecordByKey(string& binFile, int key);
void addRecordToBinFile(string& binFile, const ReaderTicket& newRecord);

#endif