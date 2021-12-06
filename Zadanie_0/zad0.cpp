#include <iostream>

using namespace std;

int main()
{
    int Z;
    cin >> Z;
    for (int i = 0; i < Z; i++)
    {
        unsigned int liczba1, liczba2;
        cin >> liczba1;
        cin >> liczba2;
        while (liczba1 != liczba2)
        {
            if (liczba1 < liczba2)
                liczba2 = liczba2 - liczba1;
            else
                liczba1 = liczba1 - liczba2;
        }
        cout << liczba1 << endl;
    }
}
