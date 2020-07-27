#ifndef FUNCIONAMENTO_H
#define FUNCIONAMENTO_H
#include <string>
using namespace std;

string imprimeDados(Fila& fila, paragem* paragem, percurso* percurso);
void utilizadorSeguinte(Fila& fila, percurso** pe, paragem* pa);
void utilizadorOperacoes(percurso* pe, paragem* pa, Fila& fila);
void movimentaAutocarro(Fila& fila, percurso** percurso, paragem* paragem);

#endif