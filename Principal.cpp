#define NEURONIOS_CAMADA_ENTRADA 16
#define NEURONIOS_CAMADA_OCULTA 7
#define NEURONIOS_CAMADA_SAIDA 3
#define QTD_AMOSTRAS 10
#define TAM_PESOS_OCULTA QTD_AMOSTRAS + 1
#define TAM_PESOS_SAIDA NEURONIOS_CAMADA_OCULTA + 1
#define TAXA_APRENDIZAGEM 0.4
#define TOLERANCIA 0.1
#define EPOCAS 100
#include <iostream>
#include <math.h>
using namespace std;

enum funcoes {SIGMOIDE,LINEAR,TANGENTE_HIPERBOLICA};

double calcularNets(double peso[],double conjunto_dados[], double vies, int qtdDados) {
	double net = peso[0] * vies;
	for(int i=0; i < qtdDados; i++){
        net += conjunto_dados[i] * peso[i + 1];
    }
	return net;
}
double funcaoTransferencia(int funcao, double x) {
	switch (funcao) {
	case SIGMOIDE: return (1.0 / (1.0 + exp(-x))); break;
	default: break;
	}
}

double derivadaFuncaoTransferencia(int funcao, double i) {
	switch(funcao) {
		case LINEAR:
			if(i >= 0) return 1.0;
			else return 0.0; 
			break;
		case SIGMOIDE: return i * (1 - i); break;
		case TANGENTE_HIPERBOLICA: return 1 - pow(i,2); break;
		default: break;
	}
}

void inicializarPesos(double p[], int tamanho_pesos) {
	for(int i = 0; i < tamanho_pesos; i++) {
		p[i] = (double)rand()/RAND_MAX; //Gera pesos entre 0 e 1
	}
}

int main() {
	srand(time(NULL));
	int epocasTotais = 0;
	double vies_oculta[NEURONIOS_CAMADA_OCULTA] = {1.0};
	double vies_saida[NEURONIOS_CAMADA_SAIDA] = {1.0};
	double nets_oculta[NEURONIOS_CAMADA_OCULTA]; //Nets da camada oculta
	double nets_saida[NEURONIOS_CAMADA_SAIDA]; //Nets da camada de saida
	double pesos_oculta[NEURONIOS_CAMADA_OCULTA][TAM_PESOS_OCULTA]; //pesos da camada oculta
	double pesos_saida[NEURONIOS_CAMADA_SAIDA][TAM_PESOS_SAIDA]; //pesos da camada de saída
	double saidas_oculta[NEURONIOS_CAMADA_OCULTA]; //Saídas dos neurônios da camada oculta
	double saidas_saida[NEURONIOS_CAMADA_SAIDA]; //Saídas dos neurônios da camada de saída

	//Inicializar pesos da camada oculta
	for(int i = 0; i < NEURONIOS_CAMADA_OCULTA; i++){
    	inicializarPesos(pesos_oculta[i], TAM_PESOS_OCULTA);
	}
	//Inicializar pesos da camada de saída
	for(int i = 0; i < NEURONIOS_CAMADA_SAIDA; i++){
    	inicializarPesos(pesos_saida[i], TAM_PESOS_SAIDA);
	}
	double conjunto_treinamento[QTD_AMOSTRAS][NEURONIOS_CAMADA_ENTRADA] {
		{1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,0},
		{1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1}
	};
	//Conjunto de saídas desejáveis
	double saidas_desejaveis[QTD_AMOSTRAS][NEURONIOS_CAMADA_SAIDA] {
		{1,0,0}, //Quando é 5
		{0,1,0}, //Quando é 7
		{0,0,1} //Quando não é 5 nem 7
	};
	//Iterações
	for(int i = 0; i < EPOCAS; i++) {
		//Início da fase forward
		//Para cada amostra de treinamento, fazer:
		for(int j = 0; j < QTD_AMOSTRAS; j++) {
			for(int k = 0; k < NEURONIOS_CAMADA_OCULTA; k++) {
				//Cálculo dos nets da camada oculta
				nets_oculta[k] = calcularNets(pesos_oculta[k], conjunto_treinamento[j],vies_oculta[k],
				NEURONIOS_CAMADA_ENTRADA);
				//Aplicação da função de transferência nos neurônios da camada oculta
				saidas_oculta[k] = funcaoTransferencia(SIGMOIDE,nets_oculta[k]);
			}

			for(int k = 0; k < NEURONIOS_CAMADA_SAIDA; k++) {
				//Cálculo dos nets da camada de saída
				nets_saida[k] = calcularNets(pesos_saida[k],saidas_oculta,vies_saida[k],NEURONIOS_CAMADA_OCULTA);
				//Aplicação da função de transferência nos neurônios da camada de saída
				saidas_saida[k] = funcaoTransferencia(SIGMOIDE,nets_saida[k]);
			}
		}
	}
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

