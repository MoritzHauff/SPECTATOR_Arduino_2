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

        cout << "Additionswinkel eingeben: " << endl;
        float wplus;
        cin >> wplus;

        cout << win << " + " << wplus << " = ";
        cout << mpuFahrer.winkelvergroessern(win, wplus) << endl;

        if(win == -100)
        {
            break;
        }
    }

    return 0;
}
