#include <iostream>

#include "MovementArray.h"

using namespace std;

int movements[][2] = {
    {0, 0},
    {2, 2},
    {16, 38},
    {-6, 2},
    {1, 2},
    {0, 2},
    {2, 0},
    {2, 1},
    {2, -1},
    {2, -2},
    {2, -3},
    {2, -4},
    {2, -5}};

int main()
{
    // Funktionierendes Testbeispiel
    int arr[6][5] = {
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5},
        {1,2,3,4,5}
    };
    int rows = sizeof(arr)/sizeof(arr[0]);
    int cols = sizeof(arr[0])/sizeof(arr[0][0]);
    cout<<rows<<" "<<cols<<endl;

    cout << "Start" << endl;

    cout << "Rows: " << sizeof(movements)/sizeof(movements[0]) << endl;

    MovementArray.GoToStart();
    MotorDaten *md;
    do{
        md = MovementArray.GetNextMovement(movements, sizeof(movements)/sizeof(movements[0])); // die sizeof Funktion kann nur hier korrekt aufgerufen werden.
        if(md != NULL)
        {
            cout << md->MotorSpeedL << " " << md->MotorSpeedR << endl;
        }
    }while(md != NULL);

    cout << "MovementArray beendet.";

    return 0;
}
