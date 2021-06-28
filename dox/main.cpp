/**
* @file main.cpp
*/
#include <Client.h>
using namespace std;

int main (int argc, char **argv)
{
    try {
        string ID, nomer;
        cout << "Введите логин и номер лицензии" << endl;
        cin >> ID;
        cin >> nomer;

        CheckClient obj;
        if (!obj.check_license(ID, nomer))
            cout << "Invalid license" << endl;
        else
            cout << "Valid license" << endl;

    } catch (const CCError & e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}
