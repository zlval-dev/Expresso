#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <Windows.h>
#include "inicializacao.h"
#include "funcionamento.h"

using namespace std;

int main(){
    SetConsoleOutputCP(CP_UTF8); // Para poder imprimir os caracteres utf-8 na linha de comando
    srand(time(NULL));
    // Inicialização
    Fila fila = inicializaPassageiros();
    paragem* paragem = criacaoParagens();
    percurso* pe = inicializaPercurso(fila);
    // Funcionamento
    while(true){
        string opcao = imprimeDados(fila, paragem, pe);
        if(opcao == "s"){
            utilizadorSeguinte(fila, &pe, paragem);
            movimentaAutocarro(fila, &pe, paragem);
        }else if(opcao == "o"){
            utilizadorOperacoes(pe, paragem, fila);
        }
        system("pause");
        system("cls");
    }
    return 0;
}