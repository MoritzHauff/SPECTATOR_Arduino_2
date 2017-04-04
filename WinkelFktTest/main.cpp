#include <iostream>

#include "MPUFahrer.h"

using namespace std;

int main()
{
    cout << "WinkelFktTest-Programm." << endl;

    MPUFahrerClass mpuFahrer = MPUFahrerClass();

    while(1)
    {
        cout << "aktuellen Winkel eingeben: ";
        float aktW;
        cin >> aktW;

        cout << "Zielrichtung eingeben (n, o, s, w): ";
        char richtung;
        cin >> richtung;

        int l, r;

        mpuFahrer.BerechneDrehen(mpuFahrer.CharToRichtung(richtung), aktW, &l, &r);

        cout << "Drehgeschwindigkeiten: " << endl;
        cout << "\tl: " << l << endl;
        cout << "\tr: " << r << endl;

        if(aktW == -100)
        {
            break;
        }
    }

    return 0;
}
