#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <Windows.h>
#include "inicializacao.h"
#include "cores.h"

using namespace std;

// Numero de linhas do ficheiro dos primeiros nomes
int numeroLinhasPrimeiroNome(){
    ifstream myFile("primeiro_nome");
    string line;
    if(myFile.is_open()){
        int numeroLinha = 0;
        while(!myFile.eof()){
            getline(myFile, line);
            numeroLinha++;
        }
        myFile.close();
        return numeroLinha;
    }
}

// Função para obter o primeiro nome
string primeiroNome(){
    int linhaEscolhida = rand() % numeroLinhasPrimeiroNome();
    ifstream myFile("primeiro_nome");
    string line;
    if(myFile.is_open()){
        int numeroLinha = 0;
        while(!myFile.eof() && linhaEscolhida != (numeroLinha - 1)){
            getline(myFile, line);
            numeroLinha++;
        }
        myFile.close();
    }
    return line;
}

// Numero de linhas do ficheiro dos ultimos nomes
int numeroLinhasUltimoNome(){
    ifstream myFile("ultimo_nome");
    string line;
    if(myFile.is_open()){
        int numeroLinha = 0;
        while(!myFile.eof()){
            getline(myFile, line);
            numeroLinha++;
        }
        myFile.close();
        return numeroLinha;
    }
}

// Função para obter o ultimo nome
string ultimoNome(){
    int linhaEscolhida = rand() % numeroLinhasUltimoNome();
    ifstream myFile("ultimo_nome");
    string line;
    if(myFile.is_open()){
        int numeroLinha = 0;
        while(!myFile.eof() && linhaEscolhida != (numeroLinha - 1)){
            getline(myFile, line);
            numeroLinha++;
        }
        myFile.close();
    }
    return line;
}

// Função para obter o número do bilhete
bool primeiro = true;
int numeroBilhete(){
    int bilhete = rand() % 99999 + 1;
    ifstream myFile("numeroBilhetes.txt");
    bool naoExiste = true;
    string line;
    if(myFile.is_open()){
        while(!myFile.eof()){
            getline(myFile, line);
            if(line == to_string(bilhete)){
                naoExiste = false;
            }
        }
        myFile.close();
    }
    if(naoExiste){
        ofstream documento;
        if(primeiro){
            primeiro = false;
            remove("numeroBilhetes.txt");
            documento.open("numeroBilhetes.txt");
        }else{
            documento.open("numeroBilhetes.txt", std::ios::app);
        }
        documento << bilhete << endl;
        return bilhete;
    }else{
        numeroBilhete();
    }
}

// Função para criar um passageiro
passageiro* criaPassageiro(){
    passageiro* p = new passageiro;
    p->primeiroNome = primeiroNome();
    p->ultimoNome = ultimoNome();
    p->numeroBilhete = numeroBilhete();
    return p;
}

// Função para inicializar uma nova Fila
void novaFila(Fila& f){
    f.ap_inicio = NULL;
}

// Função para adicionar elementos à fila
void adicionaElementos(Fila& f, passageiro* passageiro){
    Fila::Item* aux = new Fila::Item;
    aux->passageiro = passageiro;
    if(f.ap_inicio == NULL){
        aux->next = NULL;
        f.ap_inicio = aux;
    }else{
        aux->next = f.ap_inicio;
        f.ap_inicio = aux;
    }
}

// Função para obter o tamanho da fila
int tamanhoFila(Fila& f){
    if(f.ap_inicio == NULL){
        return 0;
    }else{
        Fila::Item* aux = f.ap_inicio;
        int numFila = 1;
        while(aux->next != NULL){
            aux = aux->next;
            numFila++;
        }
        return numFila;
    }
}

// Função para remover elemento da fila
passageiro* removeElemento(Fila& f){
    int tamFila = tamanhoFila(f);
    passageiro* auxPassageiro;
    if(tamFila == 1){
        auxPassageiro = f.ap_inicio->passageiro;
        delete f.ap_inicio;
        f.ap_inicio = NULL;
    }else if(tamFila == 2){
        auxPassageiro = f.ap_inicio->next->passageiro;
        delete f.ap_inicio->next;
        f.ap_inicio->next = NULL;
    }else{
        Fila::Item* aux = f.ap_inicio;
        while(aux->next->next != NULL){
            aux = aux->next;
        }
        auxPassageiro = aux->next->passageiro;
        delete aux->next->next;
        aux->next = NULL;
    }
    return auxPassageiro;
}

// Função que cria 30 passageiros e coloca numa fila de espera seguindo um paradigma FIFO
Fila inicializaPassageiros(){
    Fila aux;
    novaFila(aux);
    for(int i = 0; i < 30; i++){
        passageiro* passageiro = criaPassageiro();
        adicionaElementos(aux, passageiro);
    }
    return aux;
}

// Função para obter matricula
string obterMatricula(){
    string valoresHexadecimais[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
    string matricula = "";
    for(int i = 0; i < 4; i++){
        matricula += valoresHexadecimais[rand() % 16];
    }
    return matricula;
}

// Função para criar autocarro
autocarro* inicializaAutocarro(Fila& f){
    autocarro* aux = new autocarro;
    aux->capacidadePassageiros = (rand() % 6) + 5;
    aux->matricula = obterMatricula();
    aux->motorista = primeiroNome() + " " + ultimoNome();
    for(int i = 0; i < aux->capacidadePassageiros; i++){
        aux->passageiros[i] = removeElemento(f);
    }
    return aux;
}

// Função para inserir novo passageiro na árvore de pesquisa binária
paragem::todosPassageiros* inserirNovoPassageiro(struct paragem::todosPassageiros* tPassageiros, int bilhete){
    if(tPassageiros == NULL){
        paragem::todosPassageiros* aux = new paragem::todosPassageiros;
        aux->bilhete = bilhete;
        aux->esquerda = NULL;
        aux->direita = NULL;
        return aux;
    }else{
        if(bilhete <= tPassageiros->bilhete){
            tPassageiros->esquerda = inserirNovoPassageiro(tPassageiros->esquerda, bilhete);
        }else{
            tPassageiros->direita = inserirNovoPassageiro(tPassageiros->direita, bilhete);
        }
    }
    return tPassageiros;
}

// Numero de linhas do ficheiro de paragens
int numeroLinhasParagem(){
    ifstream myFile("paragens");
    string line;
    if(myFile.is_open()){
        int numeroLinha = 0;
        while(!myFile.eof()){
            getline(myFile, line);
            numeroLinha++;
        }
        myFile.close();
        return numeroLinha;
    }
}

// Função para verificar se é uma paragem diferente
bool verificaParagem(int* paragensAtuais, int tamanhoVetor, int aux){
    bool verifica = true;
    for(int i = 0; i < tamanhoVetor; i++){
        if(paragensAtuais[i] == aux){
            verifica = false;
            break;
        }
    }
    return verifica;
}

// Função para criar todas as paragens
paragem* criacaoParagens(){
    paragem* final = new paragem;
    string line;
    int paragens = (rand() % 6 + 4) + 2;
    int* paragensAtuais = new int[paragens];
    int aux = 0;
    int indice = 0;
    while(paragens > 0){
        if(verificaParagem(paragensAtuais, paragens, aux = rand() % numeroLinhasParagem())){
            ifstream myFile("paragens");
            if(myFile.is_open()){
                int numeroLinhas = 0;
                paragem* paragemAux = new paragem;
                while(!myFile.eof()){
                    getline(myFile, line);
                    if(numeroLinhas == aux){
                        paragensAtuais[indice] = aux;
                        paragemAux->nome = line;
                        paragemAux->arvore = NULL;
                        if(indice == 0){
                            paragemAux->next = NULL;
                        }else{
                            paragemAux->next = final;
                        }
                        indice++;
                        final = paragemAux;
                        break;
                    }
                    numeroLinhas++;
                }
                myFile.close();
            }
            paragens--;            
        }
    }
    delete[] paragensAtuais;
    return final;
}

// Função para saber número de paragens
int numeroParagens(paragem* p){
    int nParagens = 0;
    paragem* aux = p;
    while(aux != NULL){
        nParagens++;
        aux = aux->next;
    }
    return nParagens;
}

// Função para inicializar o percurso com capacidade máxima igual ao número de paragens
percurso* inicializaPercurso(Fila& fila){
    percurso* final = new percurso;
    final->autocarro = NULL;
    final->next = NULL;
    return final;
}