#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

typedef vector<char> VC;
typedef vector<VC> VVC;

VVC crearTauler(int Recipients, int Tamany) {
    VVC Tauler(Recipients, VC(Tamany, ' '));
    // Creem un vector amb tots els caracters
    int Colors = Recipients - 2;
    VC V(Colors*Tamany);
    for (int i = 0; i < Colors; ++i) {
        for (int j = 0; j < Tamany; ++j) V[i*Tamany + j] = char('A' + i);
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(V.begin(), V.end(), gen);

    for (int i = 0; i < Recipients; ++i) {
        for (int j = 0; j < Tamany; ++j) {
            if (not V.empty()) {
                Tauler[i][j] = V[V.size() - 1];
                V.pop_back();
            }
        }
    }
    return Tauler;
}


bool recipientComplert(const VC& Recipient) {
    for (int i = 0; i < Recipient.size() - 1; ++i) {
        if (Recipient[i] != Recipient[i + 1]) return false;
    }
    return true;
}

bool victoria(const VVC& Tauler) {
    for (int i = 0; i < Tauler.size(); ++i) {
        if (not recipientComplert(Tauler[i])) return false;
    } 
    return true;
}

void imprimirTauler(const VVC& Tauler) {
    for (int i = 0; i < Tauler.size(); ++i) {
        cout << "------------------" << endl;
        cout << i + 1 << ": ";
        for (int j = 0; j < Tauler[0].size(); ++j) {
            cout << Tauler[i][j] << " | ";
        }
        cout << endl << "------------------" << endl;
    }
    cout << endl;
}

// Torna el numero de caselles superiors amb el mateix color
int mateixColor(const VC& Recipient) {
    int cont = 1;
    int i = 0;
    while (i < Recipient.size() - 1 and Recipient[3 - i] == ' ') ++i;
    while (i < Recipient.size() - 1 and Recipient[3 - i] == Recipient[2 - i]) {
        ++cont;
        ++i;
    }
    return cont;
}

// Mou el liquid superior del recipient 1 al recipient 2
void moure(VVC& Tauler, int r1, int r2) {
    if (r1 < Tauler.size() and r2 < Tauler.size()) {
        int i1, i2;
        i1 = i2 = 3;
        while (i1 >= 0 and Tauler[r1][i1] == ' ') --i1;
        while (i2 >= 0 and Tauler[r2][i2] == ' ') --i2;

        if (i2 == -1 or (i2 >= 0 and Tauler[r1][i1] == Tauler[r2][i2])) {
            int espaiLliure = Tauler[0].size() - (i2 + 1);
            int seguits = mateixColor(Tauler[r1]);
            int dif = min(espaiLliure, seguits);
            for (int i = 1; i <= dif; ++i) {
                Tauler[r2][i2 + i] = Tauler[r1][i1 + 1 - i];
                Tauler[r1][i1  + 1 - i] = ' ';
            }  
        }
    }
}

void imprimirMoviments(int m) {
    cout << "Numero de moviments: " << m << endl;
}

int main() {
    VVC Tauler = crearTauler(8, 4);
    VVC TaulerInicial = Tauler;
    VVC TaulerPrev, TaulerPrev2;
    TaulerPrev = TaulerPrev2 = Tauler;
    imprimirTauler(Tauler);

    int r1, r2;
    int moviments = 0;
    imprimirMoviments(moviments);

    int moviments_prev, moviments_prev2;
    moviments_prev = moviments_prev2 = 0;
    
    while (not victoria(Tauler) and cin >> r1 >> r2) {
        if (r1 > 0 and r1 <= Tauler.size() and r2 > 0 and r2 <= Tauler.size()) {
            moure(Tauler, r1 - 1, r2 - 1);
            if (Tauler != TaulerPrev) ++moviments;
        }
        else if (r1 == -1) {    // Posicio inicial
            Tauler = TaulerInicial;
            moviments = 0;
        }
        else if (r1 == -2){     // Posicio anterior
            Tauler = TaulerPrev2;
            moviments = moviments_prev2;
        }
        imprimirTauler(Tauler);
        imprimirMoviments(moviments);

        // Retrocedir
        TaulerPrev2 = TaulerPrev;
        TaulerPrev = Tauler;
        moviments_prev2 = moviments_prev;
        moviments_prev = moviments;
    }
}