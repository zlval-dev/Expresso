#ifndef INICIALIZACAO_H
#define INICIALIZACAO_H
#include <string>
using namespace std;

struct passageiro{
    string primeiroNome;
    string ultimoNome;
    int numeroBilhete;
};

struct Fila{
    struct Item{
        passageiro* passageiro;
        Item* next;
    };
    Item* ap_inicio;
};

struct autocarro{
    int capacidadePassageiros;
    string motorista;
    string matricula;
    passageiro* passageiros[10];
};

struct paragem{
    string nome;
    struct todosPassageiros{
        int bilhete;
        todosPassageiros* esquerda;
        todosPassageiros* direita;
    };
    todosPassageiros* arvore;
    paragem* next;
};

struct percurso{
    autocarro* autocarro;
    percurso* next;
};

passageiro* criaPassageiro();
void novaFila(Fila& f);
void adicionaElementos(Fila& f, passageiro* passageiro);
int tamanhoFila(Fila& f);
passageiro* removeElemento(Fila& f);
Fila inicializaPassageiros();
autocarro* inicializaAutocarro(Fila& f);
paragem::todosPassageiros* inserirNovoPassageiro(struct paragem::todosPassageiros* tPassageiros, int bilhete);
paragem* criacaoParagens();
percurso* inicializaPercurso(Fila& fila);
int numeroParagens(paragem* p);

#endif