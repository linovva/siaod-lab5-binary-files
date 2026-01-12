#include "Header.h"

int main() {
    setlocale(LC_ALL, "Russian");

    string textFile, binFile;
    cout << "Введите имя текстового файла: ";
    cin >> textFile;
    cout << "Введите имя двоичного файла: ";
    cin >> binFile;

    int choice;
    do {
        cout << "\nМеню управления читательскими абонементами\n";
        cout << "1 - Создать двоичный файл из текстового\n";
        cout << "2 - Вывести все записи из бинарного файла\n";
        cout << "3 - Получить запись по индексу\n";
        cout << "4 - Удалить запись по ключу (заменой на последнюю)\n";
        cout << "5 - Сохранить данные двоичного файла в текстовом\n";
        cout << "6 - Удалить запись по ключу (через временный файл)\n";
        cout << "7 - Вывести запись по ключу\n";
        cout << "0 - Выход\n";
        cout << "Выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: createBinFile(textFile, binFile); break;
        case 2: outBinFile(binFile); break;
        case 3: {
            int idx; cout << "Индекс: "; cin >> idx;
            ReaderTicket r = getRecordByIndex(binFile, idx);
            if (r.ticketNumber != -1) {
                cout << "Билет: " << r.ticketNumber
                    << ", ФИО: " << r.fullName
                    << ", Адрес: " << r.address << endl;
            }
            break;
        }
        case 4: {
            int k; cout << "Ключ (номер билета): "; cin >> k;
            delRecordByKey1(binFile, k);
            break;
        }
        case 5: binToTxt(binFile); break;
        case 6: {
            int k; cout << "Ключ (номер билета): "; cin >> k;
            delRecordByKey2(binFile, k);
            break;
        }
        case 7: {
            int k; cout << "Ключ (номер билета): "; cin >> k;
            displayRecordByKey(binFile, k);
            break;
        }
        case 0: cout << "Выход.\n"; break;
        default: cout << "Неверный выбор.\n";
        }
    } while (choice != 0);

    cout << "\nРазмер структуры: " << sizeof(ReaderTicket) << " байт" << endl;
    return 0;
}