#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <Windows.h>
#include "cores.h"
#include "inicializacao.h"
#include <iomanip>

using namespace std;

// Função para imprimir ultimo nome dos passageiros em 3 colunas, em que os elementos mais à direita e mais abaixo serão os primeiros a ser colocados em autocarros
void imprimeFilaEspera(Fila& fila){
    cout << ANSI_COLOR_BLUE << "Fila de espera: " << ANSI_COLOR_GREEN << "(" << tamanhoFila(fila) << ")" << ANSI_COLOR_RESET << endl;
    Fila::Item* aux = fila.ap_inicio;
    string* todosPassageiros = new string[tamanhoFila(fila)];
    for(int i = 0; i < tamanhoFila(fila); i++){
        todosPassageiros[i] = aux->passageiro->ultimoNome + " | " + to_string(aux->passageiro->numeroBilhete);
        aux = aux->next;
    }
    delete aux;
    // Número de linhas, e quais as colunas que vão ter valores da última linha
    float numero = tamanhoFila(fila) / 3.0;
    int numeroLinhas = (int)numero;
    float ultimaLinha = numero - numeroLinhas;
    ultimaLinha = roundf(ultimaLinha * 10) / 10;
    int colunaUm = 0;
    int colunaDois = 0;
    int colunaTres = 0;
    if(ultimaLinha == 0){
        colunaDois = numeroLinhas;
        colunaTres = 2 * numeroLinhas;
    }else if(ultimaLinha == roundf(3 * 10) / 100){
        colunaDois = numeroLinhas + 1;
        colunaTres = 2 * numeroLinhas + 1;
    }else if(ultimaLinha == roundf(7 * 10) / 100){
        colunaDois = numeroLinhas + 1;
        colunaTres = 2 * numeroLinhas + 2;
    }
    for(int i = 0; i < numeroLinhas; i++){
        cout << right << setw(0) << todosPassageiros[colunaUm] << setw(30) << todosPassageiros[colunaDois] << setw(30) << todosPassageiros[colunaTres] << endl;
        colunaUm++;
        colunaDois++;
        colunaTres++;
    }
    if(ultimaLinha == roundf(3 * 10) / 100){
        cout << todosPassageiros[colunaUm] << endl;
    }else if(ultimaLinha == roundf(7 * 10) / 100){
        cout << todosPassageiros[colunaUm] << setw(30) << todosPassageiros[colunaDois] << endl;
    }
    delete[] todosPassageiros;
}

// Função para imprimir as paragens
void imprimeParagens(paragem* pa, percurso* pe){
    cout << ANSI_COLOR_BLUE << "\nParagens: " << ANSI_COLOR_GREEN << "(" << numeroParagens(pa) << ")" << ANSI_COLOR_RESET << endl;
    paragem* auxParagem = pa;
    percurso* auxPercurso = pe;
    while(auxParagem != NULL){
        cout << ANSI_COLOR_BOLD << "Paragem: " << ANSI_COLOR_RESET << auxParagem->nome << endl;
        if(auxPercurso == NULL || auxPercurso->autocarro == NULL){
            cout << ANSI_COLOR_YELLOW << "Nenhum autocarro" << ANSI_COLOR_RESET << endl << endl;
        }else{
            cout << ANSI_COLOR_BOLD << "Autocarro: " << ANSI_COLOR_RESET << auxPercurso->autocarro->matricula << ANSI_COLOR_BOLD << " Motorista: " << ANSI_COLOR_RESET << auxPercurso->autocarro->motorista << endl;
            cout << ANSI_COLOR_BOLD << "Passageiros: " << ANSI_COLOR_RESET;
            for(int i = 0; i < auxPercurso->autocarro->capacidadePassageiros; i++){
                if(auxPercurso->autocarro->passageiros[i]->primeiroNome != "Removido"){
                    cout << auxPercurso->autocarro->passageiros[i]->primeiroNome << " " << auxPercurso->autocarro->passageiros[i]->ultimoNome << " " << auxPercurso->autocarro->passageiros[i]->numeroBilhete << ", ";
                }
            }
            cout << endl << endl;
            auxPercurso = auxPercurso->next;
        }
        auxParagem = auxParagem->next;
    }
}

// Função para imprimir cada iteração
string imprimeDados(Fila& fila, paragem* paragem, percurso* percurso){
    imprimeFilaEspera(fila);
    imprimeParagens(paragem, percurso);
    // Codigo para imprimir paragens
    cout << endl << ANSI_COLOR_YELLOW << "(s)" << ANSI_COLOR_BLUE << "eguinte " << ANSI_COLOR_YELLOW << "(o)" << ANSI_COLOR_BLUE << "pções" << ANSI_COLOR_RESET << endl;
    cout << "Opção: ";
    string opcao = "vazio";
    bool primeiro = true;
    while(opcao != "s" && opcao != "o"){
        if(!primeiro){
            cout << ANSI_COLOR_RED << "Opção escolhida não existe!" << ANSI_COLOR_RESET << endl;
            cout << "Opção: ";
        }
        getline(cin, opcao);
        primeiro = false;
    }
    return opcao;
}

// Função para criar 15 novos passageiros e colocá-los na fila de espera
void novosUtilizadores(Fila& fila){
    for(int i = 0; i < 15; i++){
        passageiro* passageiro = criaPassageiro();
        adicionaElementos(fila, passageiro);
    }
}

// Função para adicionar na arvore binária
paragem::todosPassageiros* adicionaArvoreBinaria(paragem* pa, int bilhete){
    paragem::todosPassageiros* arvBinaria = pa->arvore;
    arvBinaria = inserirNovoPassageiro(arvBinaria, bilhete);
    return arvBinaria;
}

// Função para remover um conjunto aleatório de passageiros de cada autocarro
void chanceRemoverPassageiros(percurso** pe, paragem* pa){
    percurso* aux = *pe;
    while(aux->next != NULL){
        for(int i = 0; i < aux->autocarro->capacidadePassageiros; i++){
            int remove = rand() % 4;
            if(remove == 0 && aux->autocarro->passageiros[i]->primeiroNome != "Removido"){
                aux->autocarro->passageiros[i]->primeiroNome = "Removido";
                pa->arvore = adicionaArvoreBinaria(pa, aux->autocarro->passageiros[i]->numeroBilhete);
            }
        }
        aux = aux->next;
        pa = pa->next;
    }
}

// Função para realizar o pretendido após ser escolhido "seguinte" pelo utilizador
void utilizadorSeguinte(Fila& fila, percurso** pe, paragem* pa){
    novosUtilizadores(fila);
    chanceRemoverPassageiros(pe, pa);
}

// Função para remover passageiros nos autocarros
void removerPassageiros(percurso* pe, paragem* pa){
    cout << ANSI_COLOR_BLUE << "\nRemover passageiros nos autocarros" << ANSI_COLOR_RESET << endl;
    cout << ANSI_COLOR_BOLD << "Número do bilhete do passageiro a remover: " << ANSI_COLOR_RESET;
    string bilhete;
    getline(cin, bilhete);
    percurso* auxPe = pe;
    paragem* auxPa = pa;
    bool naoExiste = true;
    while(auxPe->autocarro != NULL){
        for(int i = 0; i < auxPe->autocarro->capacidadePassageiros; i++){
            if(auxPe->autocarro->passageiros[i]->numeroBilhete == stoi(bilhete)){
                naoExiste = false;
                auxPe->autocarro->passageiros[i]->primeiroNome = "Removido";
                auxPa->arvore = adicionaArvoreBinaria(auxPa, auxPe->autocarro->passageiros[i]->numeroBilhete);
                cout << ANSI_COLOR_GREEN << "Passageiro removido com sucesso!" << ANSI_COLOR_RESET << endl;
                break;
            }
        }
        auxPe = auxPe->next;
        auxPa = auxPa->next;
    }
    if(naoExiste){
        cout << ANSI_COLOR_RED << "Não existe nenhum passageiro a circular com este número de bilhete!" << ANSI_COLOR_RESET << endl;
    }
}

// Função para remover passageiros da fila de espera
void removePassageiroFilaEspera(Fila& fila){
    cout << ANSI_COLOR_BLUE << "Remover passageiros em fila de espera" << ANSI_COLOR_RESET << endl;
    cout << ANSI_COLOR_BOLD << "Introduza o número do bilhete do passageiro a remover da fila de espera: " << ANSI_COLOR_RESET;
    string bilhete;
    getline(cin, bilhete);
    Fila::Item* aux = fila.ap_inicio;
    bool naoExiste = true;
    if(aux->passageiro->numeroBilhete == stoi(bilhete)){
        naoExiste = false;
        fila.ap_inicio = aux->next;
        cout << ANSI_COLOR_GREEN << "Passageiro removido com sucesso da fila de espera!" << ANSI_COLOR_RESET << endl;
    }else{
        while(aux->next != NULL){
            if(aux->next->passageiro->numeroBilhete == stoi(bilhete)){
                naoExiste = false;
                aux->next = aux->next->next;
                cout << ANSI_COLOR_GREEN << "Passageiro removido com sucesso da fila de espera!" << ANSI_COLOR_RESET << endl;
                break;
            }
            aux = aux->next;
        }
    }
    if(naoExiste){
        cout << ANSI_COLOR_RED << "Não existe nenhum passageiro com este número de bilhete na fila de espera!" << ANSI_COLOR_RESET << endl;
    }
}

// Função para alterar o nome do motorista
void alteraMotorista(percurso* pe){
    string matricula, novoNome;
    cout << ANSI_COLOR_BLUE << "\nAlterar Motorista" << ANSI_COLOR_RESET << endl;
    cout << ANSI_COLOR_BOLD << "Matricula do autocarro a alterar o motorista: " << ANSI_COLOR_RESET;
    getline(cin, matricula);
    cout << ANSI_COLOR_BOLD << "Introduza o novo nome do motorista: " << ANSI_COLOR_RESET;
    getline(cin, novoNome);
    percurso* auxPe = pe;
    bool naoExiste = true;
    while(auxPe->autocarro != NULL){
        if(auxPe->autocarro->matricula == matricula){
            naoExiste = false;
            auxPe->autocarro->motorista = novoNome;
            cout << ANSI_COLOR_GREEN << "Nome alterado com sucesso!" << ANSI_COLOR_RESET << endl;
            break;
        }
        auxPe = auxPe->next;
    }
    if(naoExiste){
        cout << ANSI_COLOR_RED << "Não existe nenhum autocarro a circular com esta matrícula!" << ANSI_COLOR_RESET << endl;
    }
}

// Função recrusiva para imprimir a árvore binária
void imprimeArvore(paragem::todosPassageiros* arv, int nivel){
    if(arv == NULL){
        cout << endl;
        return;
    }
    imprimeArvore(arv->direita, nivel + 1);
    for(int i = 0; i < nivel; i++){
        cout << "\t";
    }

    cout << arv->bilhete << endl;
    imprimeArvore(arv->esquerda, nivel + 1);
}

// Função para apresentar bilhetes por paragem
void imprimeBilhetes(paragem* pa, int nivel){
    cout << ANSI_COLOR_BLUE << "\nApresentar bilhetes por paragem" << ANSI_COLOR_RESET << endl;
    cout << ANSI_COLOR_BOLD << "Qual paragem que pretende visualizar: " << ANSI_COLOR_RESET << endl;
    paragem* auxPa = pa;
    int indice = 1;
    while(auxPa != NULL){
        cout << ANSI_COLOR_BOLD << indice << "- " << ANSI_COLOR_RESET << auxPa->nome << endl;
        indice++;
        auxPa = auxPa->next;
    }
    cout << ANSI_COLOR_BOLD << "Insira o número da paragem que pretende apresentar os bilhetes: " << ANSI_COLOR_RESET;
    string opcao;
    getline(cin, opcao);
    if(stoi(opcao) > 0 && stoi(opcao) < indice){
        auxPa = pa;
        int inicio = 1;
        while(inicio != stoi(opcao)){
            auxPa = auxPa->next;
            inicio++;
        }
        paragem::todosPassageiros* aux = auxPa->arvore;
        imprimeArvore(aux, 0);
    }else{
        cout << ANSI_COLOR_RED << "Opção inserida não é válida!" << ANSI_COLOR_RESET << endl;
    }
}

// Função para remover bilhete da arvore binaria
void removeArvore(paragem::todosPassageiros* arv, paragem::todosPassageiros* aux, paragem::todosPassageiros* anterior){
    if(anterior == NULL){
        if(aux->esquerda == NULL & aux->direita == NULL){
            arv = NULL;
        }else if(arv->direita == NULL){
            arv = arv->esquerda;
        }else if(arv->esquerda == NULL){
            arv = arv->direita;
        }
        else{
            paragem::todosPassageiros* temp = aux->esquerda;
            while(temp->direita != NULL){
                temp = temp->direita;
            }
            temp->direita = aux->direita;
            arv = aux->esquerda;
        }
    }
    else{
        if(aux->direita == NULL){
            if(anterior->bilhete > aux->bilhete){
                anterior->esquerda = aux->esquerda;
            }else{
                anterior->direita = aux->esquerda;
            }
        }else if(aux->esquerda == NULL){
            if(anterior->bilhete > aux->bilhete){
                anterior->esquerda = aux->direita;
            }else{
                anterior->direita = aux->direita;
            }
        }else{
            paragem::todosPassageiros* temp = aux->esquerda;
            while(temp->direita != NULL){
                temp = temp->direita;
            }
            temp->direita = aux->direita;
            if(anterior->bilhete > aux->bilhete){
                anterior->esquerda = aux->esquerda;
            }else{
                anterior->direita = aux->esquerda;
            }
        }
    }
    delete aux;
}

// Função para remover bilhete da arvore binaria
void removeArvore(paragem::todosPassageiros* arv, int bilhete){
    paragem::todosPassageiros* aux = arv;
    paragem::todosPassageiros* anterior = NULL;
    while(aux != NULL){
        if(aux->bilhete == bilhete){
            return removeArvore(arv, aux, anterior);
        }else if(bilhete < aux->bilhete){
            anterior = aux;
            aux = aux->esquerda;
        }else{
            anterior = aux;
            aux = aux->direita;
        }
    }
}

// Função para remover bilhete da paragem
void removerBilheteParagem(paragem* pa){
    cout << ANSI_COLOR_BLUE << "\nRemover bilhete da paragem" << ANSI_COLOR_RESET << endl;
    cout << ANSI_COLOR_BOLD << "Qual paragem que pretende visualizar: " << ANSI_COLOR_RESET << endl;
    paragem* auxPa = pa;
    int indice = 1;
    while(auxPa != NULL){
        cout << ANSI_COLOR_BOLD << indice << "- " << ANSI_COLOR_RESET << auxPa->nome << endl;
        indice++;
        auxPa = auxPa->next;
    }
    cout << ANSI_COLOR_BOLD << "Insira o número da paragem que pretende remover o bilhete: " << ANSI_COLOR_RESET;
    string opcao;
    getline(cin, opcao);
    if(stoi(opcao) > 0 && stoi(opcao) < indice){
        auxPa = pa;
        int inicio = 1;
        while(inicio != stoi(opcao)){
            auxPa = auxPa->next;
            inicio++;
        }
        cout << ANSI_COLOR_BOLD << "Insira o número do bilhete a remover: " << ANSI_COLOR_RESET;
        string bilhete;
        getline(cin, bilhete);
        paragem::todosPassageiros* temp = auxPa->arvore;
        removeArvore(temp, stoi(bilhete));
        cout << ANSI_COLOR_GREEN << "Bilhete removido com sucesso!" << ANSI_COLOR_RESET << endl;
    }else{
        cout << ANSI_COLOR_RED << "Opção inserida não é válida!" << ANSI_COLOR_RESET << endl;
    }
}

// Função para realizar as operações
void utilizadorOperacoes(percurso* pe, paragem* pa, Fila& fila){
    cout << ANSI_COLOR_BLUE << "\nMenu Operações:" << ANSI_COLOR_RESET << endl;
    cout << ANSI_COLOR_YELLOW << "1- " << ANSI_COLOR_RESET << "Remover passageiros nos autocarros" << endl;
    cout << ANSI_COLOR_YELLOW << "2- " << ANSI_COLOR_RESET << "Remover passageiros em fila de espera" << endl;
    cout << ANSI_COLOR_YELLOW << "3- " << ANSI_COLOR_RESET << "Apresentar bilhetes por paragem" << endl;
    cout << ANSI_COLOR_YELLOW << "4- " << ANSI_COLOR_RESET << "Alterar motorista" << endl;
    cout << ANSI_COLOR_YELLOW << "5- " << ANSI_COLOR_RESET << "Remover bilhete da paragem" << endl;
    cout << ANSI_COLOR_BOLD << "Escolha uma opção: " << ANSI_COLOR_RESET;
    string opcao = "nada";
    bool primeira = true;
    while(opcao != "1" && opcao != "2" && opcao != "3" && opcao != "4" && opcao != "5"){
        if(!primeira){
            cout << ANSI_COLOR_RED << "Opção escolhida não é válida!" << ANSI_COLOR_RESET << endl;
            cout << ANSI_COLOR_BOLD << "Escolha uma opção: " << ANSI_COLOR_RESET;
        }
        getline(cin, opcao);
        primeira = false;
    }
    switch (stoi(opcao)){
        case 1:
            removerPassageiros(pe, pa);
            break;
        case 2:
            removePassageiroFilaEspera(fila);
            break;
        case 3:
            imprimeBilhetes(pa, 0);
            break;
        case 4:
            alteraMotorista(pe);
            break;
        case 5:
            removerBilheteParagem(pa);
            break;
    }
}

// Função para movimentar o autocarro
void movimentaAutocarro(Fila& fila, percurso** pe, paragem* pa){
    percurso* aux = new percurso;
    autocarro* autocarro = inicializaAutocarro(fila);
    aux->autocarro = autocarro;
    aux->next = *pe;
    *pe = aux;
    int numeroAutocarros = 1;
    paragem* auxPa = pa;
    while(aux->next->next != NULL){
        if(numeroAutocarros == numeroParagens(pa)){
            aux->next->autocarro = NULL;
            aux->next->next = NULL;
            break;
        }
        aux = aux->next;
        auxPa = auxPa->next;
        numeroAutocarros++;
    }
}