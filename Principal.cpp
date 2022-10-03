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
#include <cmath>
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
		case LINEAR: return x; break;
		case SIGMOIDE: return (1.0 / (1.0 + exp(-x))); break;
		case TANGENTE_HIPERBOLICA: return (1.0 - exp(-2 * x) / (1.0 + exp(-2 * x))); break;
		default: break;
	}
	return 0.0;
}

double derivadaFuncaoTransferencia(int funcao, double y) {
	switch(funcao) {
		case LINEAR:
			return 1.0;
			break;
		case SIGMOIDE: return y * (1 - y); break;
		case TANGENTE_HIPERBOLICA: return 1 - pow(y,2); break;
		default: break;
	}
	return 0.0;
}

double calcularErroSaida(int funcao, double d, double y) {
	double erro = d - y;
	double derivada = derivadaFuncaoTransferencia(funcao,y);
	return erro * derivada;
}

double calcularErroOculta(int funcao, double y, double erroSaida[], int indice, 
double pesoSaida[][TAM_PESOS_SAIDA]) {
	double derivada = derivadaFuncaoTransferencia(funcao,y);
	double somaErro = 0;
	for(int i = 0; i < NEURONIOS_CAMADA_SAIDA; i++) {
		somaErro += erroSaida[i] * pesoSaida[i][indice + 1];
	}
	return derivada * somaErro;
}

void inicializarPesos(double p[], int tamanho_pesos) {
	for(int i = 0; i < tamanho_pesos; i++) {
		p[i] = (double)rand()/RAND_MAX; //Gera pesos entre 0 e 1
	}
}

int main() {
	srand(time(NULL));
	double vies_oculta[NEURONIOS_CAMADA_OCULTA] = {1.0};
	double vies_saida[NEURONIOS_CAMADA_SAIDA] = {1.0};
	double nets_oculta[NEURONIOS_CAMADA_OCULTA]; 
	double nets_saida[NEURONIOS_CAMADA_SAIDA]; 
	double pesos_oculta[NEURONIOS_CAMADA_OCULTA][TAM_PESOS_OCULTA]; 
	double pesos_saida[NEURONIOS_CAMADA_SAIDA][TAM_PESOS_SAIDA]; 
	double saidas_oculta[NEURONIOS_CAMADA_OCULTA]; 
	double saidas_saida[NEURONIOS_CAMADA_SAIDA]; 
	double erroOculta[NEURONIOS_CAMADA_OCULTA];
	double erroSaida[NEURONIOS_CAMADA_SAIDA];
	double erroRede;
	//Inicializar pesos da camada oculta
	for(int i = 0; i < NEURONIOS_CAMADA_OCULTA; i++){
    	inicializarPesos(pesos_oculta[i], TAM_PESOS_OCULTA);
	}
	//Inicializar pesos da camada de saída
	for(int i = 0; i < NEURONIOS_CAMADA_SAIDA; i++){
    	inicializarPesos(pesos_saida[i], TAM_PESOS_SAIDA);
	}
	double conjunto_treinamento[QTD_AMOSTRAS][NEURONIOS_CAMADA_ENTRADA] {
		{0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0}, //0
		{0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1}, //1
		{0,1,1,0,0,0,0,1,0,1,1,1,0,1,1,1}, //2
		{0,1,1,1,0,0,1,1,0,0,0,1,0,1,1,0}, //3
		{1,0,1,0,1,1,1,0,0,0,1,0,0,0,1,0}, //4
		{1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,0}, //5
		{1,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0}, //6
		{1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1}, //7
		{0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0}, //8
		{0,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0}  //9
	};
	//Conjunto de saídas desejáveis
	double saidas_desejaveis[QTD_AMOSTRAS][NEURONIOS_CAMADA_SAIDA] {
		{0,0,1}, //Quando não é 5 nem 7
		{0,0,1}, 
		{0,0,1},
		{0,0,1}, 
		{0,0,1}, 
		{1,0,0}, //Quando é 5
		{0,0,1}, 
		{0,1,0}, //Quando é 7
		{0,0,1}, 
		{0,0,1} 
		
	};
	//Iterações
	int epocasTotais = 0;
	do {
		erroRede = 0;
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

			//Início da fase backward
			for(int k = 0; k < NEURONIOS_CAMADA_SAIDA; k++) {
				//Calcular os erros para os neurônios da camada de saída
				erroSaida[k] = calcularErroSaida(SIGMOIDE,saidas_desejaveis[j][k],saidas_saida[k]);
			}

			for(int k = 0; k < NEURONIOS_CAMADA_OCULTA; k++) {
				//Calcular os erros nos neurônios da camada oculta
				erroOculta[k] = calcularErroOculta(SIGMOIDE,saidas_oculta[k],erroSaida,k,pesos_saida);

			}
			//Ajuste de pesos da camada de saída
			for(int k = 0; k < NEURONIOS_CAMADA_SAIDA; k++) {
				//Atualizando o viés da camada de saída
				pesos_saida[k][0] += TAXA_APRENDIZAGEM * (erroSaida[k] * vies_saida[k]);
				//Atualizando os pesos
				for(int l = 1; l < TAM_PESOS_SAIDA; l++) {
					pesos_saida[k][l] += TAXA_APRENDIZAGEM * (erroSaida[k] * saidas_oculta[l-1]);
				}
			}
			//Ajuste de pesos da camada oculta
			for(int k = 0; k < NEURONIOS_CAMADA_OCULTA; k++) {
				//Atualizando o viés da camada oculta
				pesos_oculta[k][0] += TAXA_APRENDIZAGEM * (erroOculta[k] * vies_oculta[k]);
				//Atualizando os pesos
				for(int l = 0; l < TAM_PESOS_OCULTA; l++) {
					pesos_oculta[k][l] += TAXA_APRENDIZAGEM * (erroOculta[k] * conjunto_treinamento[k][l-1]);
				}
			}
			//Calcular erro de rede 
			for(int k = 0; k < NEURONIOS_CAMADA_SAIDA; k++) {
				erroRede += pow(erroSaida[k],2) / 2; 
			}
		}
		epocasTotais++;
		cout << epocasTotais << " - " << erroRede << endl;
	} while (epocasTotais < EPOCAS || erroRede > TOLERANCIA);
	cout << "Rede treinada com " << epocasTotais << " épocas" << endl;
	cout << "Erro de rede: " << erroRede << endl;
	//cout << "Erro de rede: " << erroRede << endl;
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
	0 = 0110100110010110
	1 = 0010011000100111
	2 = 0110000101110111
	3 = 0111001100010110
	4 = 1010111000100010
	5 = 1111111000101110
	6 = 1110100011101110
	1111000100010001
	8 = 0110011001100110
	9 = 0110011000100110
	*/	

}

