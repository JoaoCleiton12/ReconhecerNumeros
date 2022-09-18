#define TAMANHO_CONJUNTO_TREINO 2

#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

//Função sigmoide para ativação dos nós
double sigmoide(double x) {
	return (1.0 / (1.0 + exp(-x)));
} 

int main() {

	vector<pair<double, double>> conjuntoTreino;
	conjuntoTreino.resize(TAMANHO_CONJUNTO_TREINO);
	/*
	números escolhidos para representar as duas saída:
	Primeiro número = 5 (cinco)
	JOAO
	2+1+1+1 = 5

	Segundo número = 7 (sete)
	MELQUISEDEQUE
	(6*2+7)/3 = 6,33 = 7

	Os demais números serão parte da saída desconhecida:
	Saída desconhecida = 0,1,2,3,4,6,7,9
	*/

	//se for 5 ou 7 irá entrar para verificar qual dos dois número é.
	if () {
		//se for 5 irá entrar aqui
		if () {
			cout << "Primeiro número.\n";
		}
		//se não for o 5, então é o 7 
		else{
			cout << "Segundo número.\n";
		}
	}
	//se não for 5 ou 7, então é um número desconhecido.
	else () {
		cout << "Numero nao reconhecido\n";
	}

}
