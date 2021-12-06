#include <iostream>
#include <list>
#include <numeric>
#include <stack>

using namespace std;

class Graf
{
    int liczbaWierzcholkow;
    list<int>* listaSasiedztwa;
    list<int>* listaWag;
public:
    Graf(int liczbaWierzcholkow);
    void dodajKrawedz(int wierzcholek1, int wierzcholek2, int waga);
    int liczbaChromatyczna(int limitKolorow);
};

Graf::Graf(int liczbaWierzcholkow)
{
    this->liczbaWierzcholkow = liczbaWierzcholkow;
    listaSasiedztwa = new list<int>[liczbaWierzcholkow];
    listaWag = new list<int>[liczbaWierzcholkow];
}

void Graf::dodajKrawedz(int wierzcholek1, int wierzcholek2, int waga = 1)
{
    listaSasiedztwa[wierzcholek1].push_back(wierzcholek2);
    listaWag[wierzcholek1].push_back(waga);
}

int Graf::liczbaChromatyczna(int limitKolorow)
{
    int* tablicaKolorow = new int[liczbaWierzcholkow];
    for (int i = 0; i < liczbaWierzcholkow; i++)
        tablicaKolorow[i] = 0;
    int maxKolor = 0;
    stack <int> stos;
    stos.push(0);
    bool czyPokolorowane = true;
    for (int i = liczbaWierzcholkow - 1; i > 0 && czyPokolorowane; i--)
        if (tablicaKolorow[i] == 0) czyPokolorowane = false;

    while (czyPokolorowane == false)
    {
        //pokoloruj na kolor niekolidujący z sąsiadami
        bool czyKoliduje;
        do
        {
            tablicaKolorow[stos.top()] = tablicaKolorow[stos.top()] + 1;
            if (tablicaKolorow[0] == 2) return false;   //jeśli zmieni się kolor pierwszego wierzchołka to nie da się pokolorować grafu
            czyKoliduje = false;
            for (auto it = listaSasiedztwa[stos.top()].begin(); it != listaSasiedztwa[stos.top()].end() && !czyKoliduje; it++)
            {
                if (tablicaKolorow[stos.top()] == tablicaKolorow[*it]) czyKoliduje = true;
            }
        } while (czyKoliduje == true);

        //sprawdź, czy kolor mieści się w zakresie, jeśli nie to się cofnij
        if (tablicaKolorow[stos.top()] > limitKolorow)
        {
            tablicaKolorow[stos.top()] = 0;
            stos.pop();
        }
        else
        {
            //ustaw nową liczbę kolorów
            if (tablicaKolorow[stos.top()] > maxKolor) maxKolor = tablicaKolorow[stos.top()];
            stos.push(stos.top() + 1);
        }

        //sprawdź, czy cały graf został pokolorowany
        czyPokolorowane = true;
        for (int i = liczbaWierzcholkow - 1; i > 0 && czyPokolorowane; i--)
            if (tablicaKolorow[i] == 0)
            {
                czyPokolorowane = false;
            }
    }
    //wyświetl pokolorowanie
    for (int i = 0; i < liczbaWierzcholkow; i++)
        cout << tablicaKolorow[i] << ' ';

    delete[] tablicaKolorow;
    return maxKolor;
}

int main()
{
    int liczbaZestawow;
    cin >> liczbaZestawow;

    for (int i = 1; i <= liczbaZestawow; i++)
    {
        int liczbaWierzcholkow;
        cin >> liczbaWierzcholkow;
        int liczbaKolorow;
        cin >> liczbaKolorow;
        Graf graf(liczbaWierzcholkow);
        //pobieranie danych do list sąsiedztwa
        for (int wierzcholek1 = 0; wierzcholek1 < liczbaWierzcholkow; wierzcholek1++)
        {
            for (int wierzcholek2 = 0; wierzcholek2 < liczbaWierzcholkow; wierzcholek2++)
            {
                int sasiad;
                cin >> sasiad;
                if (sasiad == 1)
                {
                    graf.dodajKrawedz(wierzcholek1, wierzcholek2);
                }
            }
        }

        if (graf.liczbaChromatyczna(liczbaKolorow) == 0)
            cout << "NIE";

        if (i < liczbaZestawow)
            cout << endl;
    }
}
