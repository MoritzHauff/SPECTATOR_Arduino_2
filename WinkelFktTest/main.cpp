#include <iostream>

#include "MPUFahrer.h"

using namespace std;

int main()
{
    cout << "WinkelFktTest-Programm." << endl;

    MPUFahrerClass mpuFahrer = MPUFahrerClass();

    while(1)
    {
        cout << "Startwinkel eingeben: "<< endl;

        float win;
        cin >> win;

        mpuFahrer.SetNorden(win);

        cout << "Orientierundwinkel: " << endl;
        cout << "\tn:" << mpuFahrer.orientierungswinkel[mpuFahrer.CharToRichtung('n')] << endl;
        cout << "\to:" << mpuFahrer.orientierungswinkel[mpuFahrer.CharToRichtung('o')] << endl;
        cout << "\ts:" << mpuFahrer.orientierungswinkel[mpuFahrer.CharToRichtung('s')] << endl;
        cout << "\tw:" << mpuFahrer.orientierungswinkel[mpuFahrer.CharToRichtung('w')] << endl;

        if(win == -100)
        {
            break;
        }
    }

    return 0;
}
