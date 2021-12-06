#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

class Punkt
{
public:
    string nazwa;
    int x;
    int y;
    Punkt(int x = 0, int y = 0);
    Punkt(string nazwa, int x, int y);
    void wyswietlPunktInfo();
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

void Punkt::wyswietlPunktInfo()
{
    cout << "Punkt " << nazwa << ": x=" << x << " y=" << y << endl;
}

class Odcinek
{
    string nazwa;
public:
    Punkt punkt1;
    Punkt punkt2;
    Odcinek(Punkt punkt1, Punkt punkt2);
    Odcinek(string nazwa, Punkt punkt1, Punkt punkt2);
    void wyswietlOdcinekInfo();
    bool czyPunktLezyNaOdcinku(Punkt punkt);
};

Odcinek::Odcinek(Punkt punkt1, Punkt punkt2)
{
    this->punkt1 = punkt1;
    this->punkt2 = punkt2;
}

Odcinek::Odcinek(string nazwa, Punkt punkt1, Punkt punkt2)
{
    this->nazwa = nazwa;
    this->punkt1 = punkt1;
    this->punkt2 = punkt2;
}

void Odcinek::wyswietlOdcinekInfo()
{
    cout << "Odcinek " << nazwa << ": x1=" << punkt1.x << " y1=" << punkt1.y << " x2=" << punkt2.x << " y2=" << punkt2.y << endl;
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

bool Odcinek::czyPunktLezyNaOdcinku(Punkt punkt)
{
    if (wyznacznik3punktow(punkt1, punkt2, punkt) == 0)
        if (punkt.x >= min(punkt1.x, punkt2.x) && punkt.x <= max(punkt1.x, punkt2.x))
            if (punkt.y >= min(punkt1.y, punkt2.y) && punkt.y <= max(punkt1.y, punkt2.y)) return true;
    return false;
}

class Wielokat
{
    vector <Punkt> wierzcholki;
    vector <Odcinek> boki;
public:
    int maxX;
    void dodajWierzcholek(Punkt punkt);
    bool utworzBoki();
    void wyswietlBokiInfo();
    bool czyPunktLezyNaBoku(Punkt punkt);
    bool czyPunktNalezyDoWielokata(Punkt punkt);
};

void Wielokat::dodajWierzcholek(Punkt punkt)
{
    wierzcholki.push_back(punkt);
}

bool Wielokat::utworzBoki()
{
    if (wierzcholki.size() < 3) return false;
    else
    {
        maxX = wierzcholki[0].x;
        for (int i = 0; i < wierzcholki.size() - 1; i++)
        {
            maxX = max(wierzcholki[i].x, maxX);
            Odcinek odcinek(to_string(i + 1), wierzcholki[i], wierzcholki[i + 1]);
            boki.push_back(odcinek);
        }
        Odcinek odcinek(to_string(wierzcholki.size()), wierzcholki.back(), wierzcholki.front());
        boki.push_back(odcinek);

        return true;
    }
}

void Wielokat::wyswietlBokiInfo()
{
    for (int i = 0; i < boki.size(); i++)
    {
        boki[i].wyswietlOdcinekInfo();
    }
}

bool Wielokat::czyPunktLezyNaBoku(Punkt punkt)
{
    for (int i = 0; i < boki.size(); i++)
    {
        if (boki[i].czyPunktLezyNaOdcinku(punkt)) return true;
    }
    return false;
}

bool czy2odcinkiSiePrzecinaja(Odcinek odcinek1, Odcinek odcinek2)
{
    long long d1 = wyznacznik3punktow(odcinek1.punkt1, odcinek1.punkt2, odcinek2.punkt1);
    long long d2 = wyznacznik3punktow(odcinek1.punkt1, odcinek1.punkt2, odcinek2.punkt2);
    long long d3 = wyznacznik3punktow(odcinek2.punkt1, odcinek2.punkt2, odcinek1.punkt1);
    long long d4 = wyznacznik3punktow(odcinek2.punkt1, odcinek2.punkt2, odcinek1.punkt2);

    d1 = d1 / max((long long)1, abs(d1));   //zmniejszenie zakresu
    d2 = d2 / max((long long)1, abs(d2));
    d3 = d3 / max((long long)1, abs(d3));
    d4 = d4 / max((long long)1, abs(d4));

    if (d1 * d2 < 0 && d3 * d4 < 0) return true;
    else return false;
}

bool czy2odcinkiSaStyczne(Odcinek odcinek1, Odcinek odcinek2)
{
    if (odcinek1.czyPunktLezyNaOdcinku(odcinek2.punkt1)) return true;
    if (odcinek1.czyPunktLezyNaOdcinku(odcinek2.punkt2)) return true;
    if (odcinek2.czyPunktLezyNaOdcinku(odcinek1.punkt1)) return true;
    if (odcinek2.czyPunktLezyNaOdcinku(odcinek1.punkt2)) return true;
    return false;
}

bool Wielokat::czyPunktNalezyDoWielokata(Punkt punkt)
{
    if (czyPunktLezyNaBoku(punkt)) return true;
    Punkt punktX(maxX + 1, punkt.y);
    Odcinek odcinek(punkt, punktX);
    //odcinek.wyswietlOdcinekInfo();
    int licznik = 0;
    for (int i = 0; i < boki.size(); i++)
    {
        //cout << "czy2odcinkiSiePrzecinaja bok[" << i+1 << "] :" << czy2odcinkiSiePrzecinaja(boki[i], odcinek) << endl;
        if (czy2odcinkiSiePrzecinaja(boki[i], odcinek)) licznik++;
    }
    long long wyznacznikTymczasowy = 1;
    for (int i = 0; i < wierzcholki.size(); i++)
    {
        //cout << "czyPunktLezyNaOdcinku wierzcholek[" << i + 1 << "] :" << odcinek.czyPunktLezyNaOdcinku(wierzcholki[i]);
        if (odcinek.czyPunktLezyNaOdcinku(wierzcholki[i]))
        {
            long long wyznacznikPoprzedni = wyznacznik3punktow(odcinek.punkt1, odcinek.punkt2, wierzcholki[(wierzcholki.size() + i - 1) % wierzcholki.size()]);
            long long wyznacznikNastepny = wyznacznik3punktow(odcinek.punkt1, odcinek.punkt2, wierzcholki[(wierzcholki.size() + i + 1) % wierzcholki.size()]);
            //cout << ' ' << wyznacznikPoprzedni << ' ' << wyznacznikNastepny;
            if (wyznacznikPoprzedni * wyznacznikNastepny < 0) licznik++;
            else if (wyznacznikPoprzedni * wyznacznikNastepny == 0)
            {
                if (wyznacznikPoprzedni != 0)
                    wyznacznikTymczasowy = wyznacznikPoprzedni;
                if (wyznacznikNastepny != 0)
                {
                    if (wyznacznikTymczasowy * wyznacznikNastepny < 0)
                    {
                        //cout << ' ' << wyznacznikTymczasowy << ' ' << wyznacznikNastepny;
                        licznik++;
                    }
                }
            }
        }
        //cout << endl;
    }
    //cout << licznik << endl;
    if (licznik % 2 == 1) return true;
    else return false;
}

int main()
{
    int liczbaZestawow;
    cin >> liczbaZestawow;

    for (int i = 1; i <= liczbaZestawow; i++)
    {
        int liczbaWierzcholkow;
        cin >> liczbaWierzcholkow;

        int x, y;
        cin >> x;
        cin >> y;
        Punkt punktP("p", x, y);


        Wielokat wielokat;
        vector <Punkt> zbiorPunktow;
        for (int j = 1; j <= liczbaWierzcholkow; j++)
        {
            int x, y;
            cin >> x;
            cin >> y;

            Punkt punkt(to_string(j), x, y);
            wielokat.dodajWierzcholek(punkt);
        }
        wielokat.utworzBoki();
        if (wielokat.czyPunktNalezyDoWielokata(punktP)) cout << "TAK" << endl;
        else  cout << "NIE" << endl;
    }
}