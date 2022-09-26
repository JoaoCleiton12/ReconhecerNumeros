#define NEURONIOS_CAMADA_OCULTA 4
#define QTD_DADOS 10
#define NEURONIOS_CAMADA_SAIDA 1
#define TAM_PESOS_OCULTA QTD_DADOS + 1
#define TAM_PESOS_SAIDA NEURONIOS_CAMADA_OCULTA + 1
#define TAXA_APRENDIZAGEM 0.4
#define TOLERANCIA 0.1
#define EPOCAS 0
#include <iostream>
#include <math.h>
using namespace std;

//Função sigmoide para ativação dos nós
double sigmoide(double x) {
	return (1.0 / (1.0 + exp(-x)));
}

double sigmoideDerivada(double x) {
	return x * (1 - x);
}

void inicializarPesos(double m[], int tamanho_pesos) {
	for(int i = 0; i < tamanho_pesos; i++) {
		m[i] = (double)rand()/RAND_MAX; //Gera pesos entre 0 e 1
	}
}

int main() {
	srand(time(NULL));
	double w1[NEURONIOS_CAMADA_OCULTA][TAM_PESOS_OCULTA]; //pesos da camada oculta
	double w2[NEURONIOS_CAMADA_SAIDA][TAM_PESOS_SAIDA]; //pesos da camada de saída
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
	}*/
}

