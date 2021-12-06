#include <iostream>
#include <math.h>

using namespace std;

class Punkt
{
public:
    int x = 0;
    int y = 0;
    Punkt() {};
    Punkt(int x, int y);
};

Punkt::Punkt(int x, int y)
{
    this->x = x;
    this->y = y;
}

double odlegloscPunktuOdProstej(Punkt p1, Punkt p2, Punkt punkt)
{
    double A = p2.y - p1.y;
    double B = p2.x - p1.x;
    return abs(A * (p1.x - punkt.x) + B * (punkt.y - p1.y)) / sqrt(A * A + B * B);
}

double odlegloscMiedzyPunktami(Punkt p1, Punkt p2)
{
    return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

long long wyznacznik3punktow(Punkt punkt1, Punkt punkt2, Punkt punkt3)
{
    long long Xa = punkt1.x;
    long long Ya = punkt1.y;
    long long Xb = punkt2.x;
    long long Yb = punkt2.y;
    long long Xc = punkt3.x;
    long long Yc = punkt3.y;

    return (long long)(Xb - Xa) * (Yc - Ya) - (Yb - Ya) * (Xc - Xa);
}

int main()
{
    int liczbaTestow;
    cin >> liczbaTestow;
    for (int i = 1; i <= liczbaTestow; i++)
    {
        int licznik = 0;
        int N, x, y;
        cin >> N;
        cin >> x;
        cin >> y;

        Punkt punkt1(0, 0);
        Punkt punkt2(x, y);
        Punkt punkt3(y, -x);    //wyznacza linię prostopadłą do strzału

        for (int i = 0; i < N; i++)
        {
            int xi, yi, ri;
            cin >> xi;
            cin >> yi;
            cin >> ri;

            Punkt punkt(xi, yi);

            //cout << odlegloscPunktuOdProstej(punkt1, punkt2, punkt) << ' ' << ri << endl;
            bool czyTestowac = true;
            if (wyznacznik3punktow(punkt1, punkt3, punkt) < 0) czyTestowac = false; //czy badane koło znajduje się w odpowiedniej połowie układu współrzędnych
            if (odlegloscMiedzyPunktami(punkt1, punkt) <= ri) czyTestowac = false;
            if (czyTestowac)
            {
                if (odlegloscPunktuOdProstej(punkt1, punkt2, punkt) <= ri) licznik++;
            }
        }
        cout << licznik << endl;
    }
}