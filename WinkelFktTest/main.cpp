#include <iostream>

#include "MPUFahrer.h"

using namespace std;

int main()
{
    cout << "WinkelFktTest-Programm." << endl;

    MPUFahrerClass mpuFahrer = MPUFahrerClass();

    while(1)
    {
        cout << "Winkel 1 eingeben: ";
        float weins;
        cin >> weins;

        cout << "Winkel 2 eingeben: ";
        float wzwei;
        cin >> wzwei;

        cout << "pos. Winkelabstand: ";
        cout << mpuFahrer.winkelAbstand(weins, wzwei) << endl;

        cout << "min. Winkelabstand: ";
        cout << mpuFahrer.minWinkelAbstand(weins, wzwei) << endl;

        if(weins == -100)
        {
            break;
        }
    }

    return 0;
}
