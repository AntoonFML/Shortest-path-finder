// TP2024 - Projekt 1, ACiR gr.2, Antoni Rumowski nr 193087, Marek Marcinko nr 197870
#include <iomanip>
#include <iostream>
#include <ctime>
using namespace std;

const int kolumny = 40;
const int wiersze = 20;
const int maxPole = 800;
const int bok = 5;
const int ileSegment = 6;

// tablice - czarne pole == 0 , biale pole == 1, start == 2, meta == 3, trasa == 4

const int segmenty[ileSegment][bok][bok] = {
    // Tablica A
    {
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 1, 0, 1, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
    },
    // Tablica B
    {
        {0, 0, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 1, 0},
    },
    // Tablica C
    {
        {1, 1, 1, 1, 1},
        {1, 1, 0, 1, 1},
        {1, 1, 0, 0, 1},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 1},
    },
    // Tablica D
    {
        {0, 0, 1, 1, 1},
        {0, 1, 1, 0, 1},
        {1, 1, 0, 1, 1},
        {1, 0, 1, 1, 0},
        {0, 1, 1, 0, 0},
    },
    // Tablica E
    {
        {1, 1, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1},
        {1, 1, 1, 1, 0},
    },
    // Tablica F
    {
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
    }
};

struct wezel {
    int wiersz, kolumna, odleglosc;
};

void odtworzTrase(int startX, int startY, int metaX, int metaY, int poprzedni[wiersze][kolumny], int tablica[wiersze][kolumny]) {
    int x = metaX;
    int y = metaY;

    while (x != startX || y != startY) { 
        tablica[y][x] = 4; 

        int tempX = poprzedni[y][x] % kolumny; // "wyciagamy" wspolrzedne punktu ktory chcemy odtworzyc ze wzoru (wiersz * kolumny + kolumna) 
        int tempY = poprzedni[y][x] / kolumny; // reszta z dzielenia przez kolumny == kolumna, wynik int dzielenia == wiersz 

        x = tempX;
        y = tempY;
    }
    tablica[startY][startX] = 2; 
    tablica[metaY][metaX] = 3; 
}

int bfs(int startX, int startY, int koniecX, int koniecY, int tablica[wiersze][kolumny], int poprzedni[wiersze][kolumny]){

    bool sprawdzone[wiersze][kolumny] = {false}; 
    int poziom[] = {-1, 0, 1, 0}; //współrzędne odpowiedzialne za sprawdzanie sasiednich pol
    int pion[] = {0, 1, 0, -1}; 
    
    wezel kolejka[maxPole]; //kolejka zawierająca dane każdego przeszukiwanego pola
    int poczatek = 0; 
    int tyl = 0;

    wezel wezelStart = {startY, startX, 0}; 
    kolejka[tyl++] = wezelStart; //dodajemy wezel na koniec kolejki i przesuwamy w tyl o jeden aby pokazac kolejny mozliwy indeks do dodania 
    sprawdzone[startY][startX] = true; 
    
    while(poczatek < tyl){ 
        wezel obecnyWezel = kolejka[poczatek++]; //z kolejki pobieramy wezel w ktorym obecnie sie znajdujemy 
        int obecnyWiersz = obecnyWezel.wiersz; // "rozbijamy" obecny wezel
        int obecnaKolumna = obecnyWezel.kolumna; // na 3 osobne zmienne
        int dlugoscTrasy = obecnyWezel.odleglosc; // aby latwiej korzystac z tego w dalszej czesci kodu

        if(obecnyWiersz == koniecY && obecnaKolumna == koniecX){      
            return dlugoscTrasy;
        }

        for (int i = 0; i < 4; i++){ 
            int sprawdzWiersz = obecnyWiersz + pion[i];
            int sprawdzKolumne = obecnaKolumna + poziom[i];

            if (sprawdzWiersz >=0 && sprawdzWiersz < wiersze && sprawdzKolumne >=0 && sprawdzKolumne < kolumny && tablica[sprawdzWiersz][sprawdzKolumne] != 0 && !sprawdzone[sprawdzWiersz][sprawdzKolumne]){ 
                wezel nowyWezel = {sprawdzWiersz, sprawdzKolumne, dlugoscTrasy+1}; //nowa struktura wezel która zawiera wspolrzedne pola, oraz dlugosc trasy od punktu A powiekszamy o 1
                kolejka[tyl++] = nowyWezel;  
                poprzedni[sprawdzWiersz][sprawdzKolumne] = obecnyWiersz * kolumny + obecnaKolumna; // zapisujemy poprzedni punkt laczac jego wspolrzedne w jeden numer
                sprawdzone[sprawdzWiersz][sprawdzKolumne] = true; //odznaczamy dany punkt jako sprawdzony aby nie wejsc na niego ponownie
            }
        }

    }
    return -1; // jeżeli coś pójdzie nie tak (trasa nie zostanie odnaleziona) zwracamy -1
}

int losujPunkty(int tablica[wiersze][kolumny], int& startX, int& startY, int& metaX, int& metaY) {
  
    while (true) {
         startY = rand() % wiersze;
         startX = rand() % kolumny;
        if (tablica[startY][startX] != 0) break;
    }

    while (true) {
         metaY = rand() % wiersze;
         metaX = rand() % kolumny;
        if (metaY != startY && metaX != startX && tablica[metaY][metaX] == 1) break;
    }

    tablica[startY][startX] = 2;
    tablica[metaY][metaX] = 3;

    return tablica[wiersze][kolumny];
}

void piszMape(int tablica[wiersze][kolumny]) {
    for (int i = 0; i < wiersze; i++) {
        for (int j = 0; j < kolumny; j++) {
            if (tablica[i][j] == 1) std::cout << setw(2) <<char(254);
            else if (tablica[i][j] == 0) std::cout << setw(2) <<char(255);
            else if (tablica[i][j] == 2) std::cout << setw(2) << 'A';
            else if (tablica[i][j] == 3) std::cout << setw(2) << 'B';
            else if (tablica[i][j] == 4) std::cout << setw(2) << '*';
        }
        std::cout << endl;
    }
}

int liczWynik(int wynik[wiersze][kolumny]) {
    for(int i = 0; i < wiersze; i+=bok){
        for(int j = 0; j < kolumny; j+=bok){
            int segment = rand() % 6;
            for(int k = 0; k < bok; ++k){
                for(int l = 0; l < bok; ++l){
                    wynik[i+k][j+l]=segmenty[segment][k][l];
                }
            }
        }
    }
    return wynik[wiersze][kolumny];
}

int main() {
    int metaX, metaY;
    int startY, startX;
    int wynik[wiersze][kolumny];
    int trasa[wiersze][kolumny]={0};
    int poprzedni[wiersze][kolumny] = {0};
    srand(time(NULL));

    liczWynik(wynik);
    losujPunkty(wynik, startX, startY, metaX, metaY);
    int najkrotszaTrasa = bfs(startX, startY, metaX, metaY, wynik, poprzedni);
      if (najkrotszaTrasa !=-1){
         odtworzTrase(startX, startY, metaX, metaY, poprzedni, wynik);
         piszMape(wynik);
        cout << "najkrotsza trasa miedzy A i B wynosi: " << najkrotszaTrasa << endl;
    }
    else{
        cout << "NOOOOO WAAAAYYYY :O" << endl;
    }
        return 0;
}