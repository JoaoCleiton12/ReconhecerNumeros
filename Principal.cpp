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
	*/	

	bool mat1[4][4];
	
	/*
	se todos esses bits de entrada forem iguais a 1, ou sejs true, então o numero é 5
	formato1 do numero 5 = 1111111000101110
	mat1[0][0] && mat1[0][1] && mat1[0][2] && mat1[0][3] && mat1[1][0] && mat1[1][1] && mat1[1][2] && mat1[2][2] && mat1[3][0] && mat1[3][1] && mat1[3][2] == true;
	mat1[1][3] && mat1[2][0] && mat1[2][1] && mat1[2][3] && mat1[3][3] == false;

	formato1 do numero 7 = 1111000100010001
	se todos esses bits de entrada forem iguais a 1, ou seja, true, então o numero é 7
	mat1[0][0] && mat1[0][1] && mat1[0][2] && mat1[0][3] && mat1[1][3] && mat1[2][3] && mat1[3][3])  == true

	mat1[1][0] && mat1[1][1] && mat1[1][2] && mat1[2][0] && mat1[2][1] && mat1[2][2] && mat1[3][0] && mat1[3][1] && mat1[3][2] == false;
	*/

	//a leitura dos valores em binário esta sendo feita de maneira manual por enquanto
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cin >> mat1[i][j];
		}
	}

	//se for 5 ou 7 irá entrar para verificar qual dos dois número é.
	if (((((mat1[0][0] && mat1[0][1] && mat1[0][2] && mat1[0][3] && mat1[1][0] && mat1[1][1] && mat1[1][2] && mat1[2][2] && mat1[3][0] && mat1[3][1] && mat1[3][2]) == true) && 
		((mat1[1][3] && mat1[2][0] && mat1[2][1] && mat1[2][3] && mat1[3][3]) == false)) ||
		(((mat1[0][0] && mat1[0][1] && mat1[0][2] && mat1[0][3] && mat1[1][3] && mat1[2][3] && mat1[3][3]) == true) && 
		((mat1[1][0] && mat1[1][1] && mat1[1][2] && mat1[2][0] && mat1[2][1] && mat1[2][2] && mat1[3][0] && mat1[3][1] && mat1[3][2]) == false) ))) {
		//se for 5 irá entrar aqui
		if ((((mat1[0][0] && mat1[0][1] && mat1[0][2] && mat1[0][3] && mat1[1][0] && mat1[1][1] && mat1[1][2] && mat1[2][2] && mat1[3][0] && mat1[3][1] && mat1[3][2]) == true) &&
			((mat1[1][3] && mat1[2][0] && mat1[2][1] && mat1[2][3] && mat1[3][3]) == false))) {
			cout << "Primeiro numero.\n";
		}
		//se não for o 5, então é o 7 
		else{
			cout << "Segundo número.\n";
		}
	}
	/*
	//se não for 5 ou 7, então é um número desconhecido.
	else () {
		cout << "Numero nao reconhecido\n";
<<<<<<< HEAD
	}
}
	*/
}

