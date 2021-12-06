#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

struct Alfa
{
    int licznik;
    int mianownik;
};

class Punkt
{
public:
    int x;
    int y;
    string nazwa;
    Alfa alfa;
    Punkt(int x = 0, int y = 0);
    Punkt(string nazwa, int x, int y);
    void wyswietlPunktInfo();
    bool operator!=(const Punkt& punkt) const;
};

Punkt::Punkt(int x, int y)
{
    this->x = x;
    this->y = y;
}

Punkt::Punkt(string nazwa, int x, int y)
{
    this->nazwa = nazwa;
    this->x = x;
    this->y = y;
}

bool Punkt::operator!=(const Punkt& punkt) const
{
    if (this->x == punkt.x && this->y == punkt.y) return false;
    else return true;
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

class Wielokat
{
public:
    vector <Punkt> punkty;
    void dodajPunkt(Punkt punkt);
    long long obliczPoleRazy2();
    Punkt punktMinY();
    void przesunOWektor(int x, int y);
    vector <Punkt> posortujBiegunowo(Punkt punktC);
};

void Wielokat::dodajPunkt(Punkt punkt)
{
    punkty.push_back(punkt);
}

long long Wielokat::obliczPoleRazy2()
{
    long long pole = 0;
    if (punkty.size() < 3) return pole;
    else
    {
        for (int i = 0; i < punkty.size() - 1; i++)
        {
            pole += wyznacznik3punktow(Punkt(0, 0), punkty[i], punkty[i + 1]);
        }
        pole += wyznacznik3punktow(Punkt(0, 0), punkty.back(), punkty.front());
        return abs(pole);
    }
}

Punkt Wielokat::punktMinY()
{
    if (punkty.size() <= 0) return NULL;
    Punkt punktMinY = punkty[0];
    for (int i = 1; i < punkty.size(); i++)
    {
        if (punkty[i].y < punktMinY.y) punktMinY = punkty[i];
    }
    return punktMinY;
}

Alfa alfa(int x, int y, int d)
{
    Alfa wynik;
    if (x > 0 && y >= 0) wynik.licznik = y;
    else if (x <= 0 && y > 0) wynik.licznik = 2 * d - y;
    else if (x < 0 && y <= 0) wynik.licznik = 2 * d + abs(y);
    else if (x >= 0 && y < 0) wynik.licznik = 4 * d - abs(y);
    wynik.mianownik = d;
    return wynik;
}

bool porownaniePunktAlfaOdleglosc(Punkt punkt1, Punkt punkt2)
{
    if (punkt1.alfa.licznik * punkt2.alfa.mianownik < punkt2.alfa.licznik * punkt1.alfa.mianownik)
    {
        return true;
    }
    else if (punkt1.alfa.licznik * punkt2.alfa.mianownik == punkt2.alfa.licznik * punkt1.alfa.mianownik)
    {
        if ((long long)punkt1.x * punkt1.x + (long long)punkt1.y * punkt1.y < (long long)punkt2.x * punkt2.x + (long long)punkt2.y * punkt2.y)
        {
            return false;
        }
        else return true;
    }
    else return false;
}

void Wielokat::przesunOWektor(int x, int y)
{
    for (int i = 0; i < punkty.size(); i++)
    {
        punkty[i].x += x;
        punkty[i].y += y;
    }
}

vector <Punkt> Wielokat::posortujBiegunowo(Punkt punktC)
{
    vector <Punkt> lista;
    for (Punkt punkt : punkty)
    {
        if (punkt != punktC)
        {
            int x = punkt.x - punktC.x;
            int y = punkt.y - punktC.y;
            int d = abs(x) + abs(y);
            punkt.alfa = alfa(x, y, d);
            lista.push_back(punkt);
        }
    }
    sort(lista.begin(), lista.end(), porownaniePunktAlfaOdleglosc);
    return lista;
}

int main()
{
    int liczbaZestawow;
    cin >> liczbaZestawow;

    for (int i = 1; i <= liczbaZestawow; i++)
    {
        int liczbaPunktow;
        cin >> liczbaPunktow;

        Wielokat zbiorPunktow;
        for (int j = 1; j <= liczbaPunktow; j++)
        {
            int x, y;
            cin >> x;
            cin >> y;

            Punkt punkt(to_string(j), x, y);
            zbiorPunktow.dodajPunkt(punkt);
        }
        Punkt punktC = zbiorPunktow.punktMinY();
        //
        zbiorPunktow.przesunOWektor(-punktC.x, -punktC.y);
        punktC.x = 0;
        punktC.y = 0;
        //
        vector <Punkt> lista = zbiorPunktow.posortujBiegunowo(punktC);

        vector <Punkt> stosCH;
        stosCH.push_back(punktC);
        stosCH.push_back(lista.back());
        lista.pop_back();
        stosCH.push_back(lista.back());
        lista.pop_back();
        Punkt z = lista.back();
        lista.pop_back();
        while (z != NULL)
        {
            Punkt y = stosCH.back();
            stosCH.pop_back();
            Punkt x = stosCH.back();
            stosCH.pop_back();
            if (wyznacznik3punktow(x, y, z) > 0)
            {
                stosCH.push_back(x);
            }
            else
            {
                stosCH.push_back(x);
                stosCH.push_back(y);
                stosCH.push_back(z);
                if (lista.empty() == false)
                {
                    z = lista.back();
                    lista.pop_back();
                }
                else
                {
                    z = NULL;
                }
            }
        }
        Wielokat otoczka;
        for (Punkt punkt : stosCH)
        {
            otoczka.dodajPunkt(punkt);
        }
        //zle
        //cout << otoczka.obliczPoleRazy2()/2.0 << endl;
        //poprawne
        long long polePodwojone = otoczka.obliczPoleRazy2();
        long long pole = polePodwojone / 2;
        cout << pole;
        if (polePodwojone % 2 == 1) cout << ".5";
        cout << endl;
        //
    }
}