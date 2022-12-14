#define NEURONIOS_CAMADA_ENTRADA 16
#define NEURONIOS_CAMADA_OCULTA 7
#define NEURONIOS_CAMADA_SAIDA 3
#define QTD_AMOSTRAS 10
#define TAM_PESOS_OCULTA QTD_AMOSTRAS + 1
#define TAM_PESOS_SAIDA NEURONIOS_CAMADA_OCULTA + 1
#define TAXA_APRENDIZAGEM 0.4
#define TOLERANCIA 0.0001
#define EPOCAS 1000
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

enum funcoes
{
	SIGMOIDE,
	LINEAR,
	TANGENTE_HIPERBOLICA
};

double calcularNets(double peso[], double conjunto_dados[], double vies, int qtdDados)
{
	double net = peso[0] * vies;
	for (int i = 0; i < qtdDados; i++)
	{
		net += conjunto_dados[i] * peso[i + 1];
	}
	return net;
}
double funcaoTransferencia(int funcao, double x)
{
	double e;
	switch (funcao)
	{
	case LINEAR:
		return x;
		break;
	case SIGMOIDE:
		return (1.0 / (1.0 + exp(-x)));
		break;
	case TANGENTE_HIPERBOLICA:
		e = exp(2 * x);
		return (2 - 1.0) / (2 + 1.0);
		break;
	default:
		break;
	}
	return 0.0;
}

double derivadaFuncaoTransferencia(int funcao, double y)
{
	switch (funcao)
	{
	case LINEAR:
		return 1.0;
		break;
	case SIGMOIDE:
		return y * (1 - y);
		break;
	case TANGENTE_HIPERBOLICA:
		return 1 - pow(y, 2);
		break;
	default:
		break;
	}
	return 0.0;
}

double calcularErroSaida(int funcao, double d, double y)
{
	double erro = d - y;
	double derivada = derivadaFuncaoTransferencia(funcao, y);
	return erro * derivada;
}

double calcularErroOculta(int funcao, double y, double erroSaida[], int indice,
						  double pesoSaida[][TAM_PESOS_SAIDA])
{
	double derivada = derivadaFuncaoTransferencia(funcao, y);
	double somaErro = 0;
	for (int i = 0; i < NEURONIOS_CAMADA_SAIDA; i++)
	{
		somaErro += erroSaida[i] * pesoSaida[i][indice + 1];
	}
	return derivada * somaErro;
}

void inicializarPesos(double p[], int tamanho_pesos)
{
	for (int i = 0; i < tamanho_pesos; i++)
	{
		p[i] = (double)rand() / RAND_MAX; // Gera pesos entre 0 e 1
	}
}

int main()
{
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
	// Inicializar pesos da camada oculta
	for (int i = 0; i < NEURONIOS_CAMADA_OCULTA; i++)
	{
		inicializarPesos(pesos_oculta[i], TAM_PESOS_OCULTA);
	}
	// Inicializar pesos da camada de sa??da
	for (int i = 0; i < NEURONIOS_CAMADA_SAIDA; i++)
	{
		inicializarPesos(pesos_saida[i], TAM_PESOS_SAIDA);
	}
	double conjunto_treinamento[QTD_AMOSTRAS][NEURONIOS_CAMADA_ENTRADA]{
		{0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0}, // 0
		{0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1}, // 1
		{0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1}, // 2
		{0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0}, // 3
		{1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0}, // 4
		{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0}, // 5
		{1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0}, // 6
		{1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1}, // 7
		{0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0}, // 8
		{0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0}  // 9
	};
	// Conjunto de sa??das desej??veis
	double saidas_desejaveis[QTD_AMOSTRAS][NEURONIOS_CAMADA_SAIDA]{
		{0, 0, 1}, // Quando n??o ?? 5 nem 7
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{0, 0, 1},
		{1, 0, 0}, // Quando ?? 5
		{0, 0, 1},
		{0, 1, 0}, // Quando ?? 7
		{0, 0, 1},
		{0, 0, 1}};
	// Itera????es
	int epocasTotais = 0;
	for (int j = 0; j < QTD_AMOSTRAS; j++)
	{
		cout << j+1 << "?? amostra" << endl;
		do
		{
			erroRede = 0;
			// In??cio da fase forward
			// Para cada amostra de treinamento, fazer:
			for (int k = 0; k < NEURONIOS_CAMADA_OCULTA; k++)
			{
				// C??lculo dos nets da camada oculta
				nets_oculta[k] = calcularNets(pesos_oculta[k], conjunto_treinamento[j], vies_oculta[k],
											  NEURONIOS_CAMADA_ENTRADA);
				// Aplica????o da fun????o de transfer??ncia nos neur??nios da camada oculta
				saidas_oculta[k] = funcaoTransferencia(SIGMOIDE, nets_oculta[k]);
			}

			for (int k = 0; k < NEURONIOS_CAMADA_SAIDA; k++)
			{
				// C??lculo dos nets da camada de sa??da
				nets_saida[k] = calcularNets(pesos_saida[k], saidas_oculta, vies_saida[k], NEURONIOS_CAMADA_OCULTA);
				// Aplica????o da fun????o de transfer??ncia nos neur??nios da camada de sa??da
				saidas_saida[k] = funcaoTransferencia(SIGMOIDE, nets_saida[k]);
			}

			// In??cio da fase backward
			for (int k = 0; k < NEURONIOS_CAMADA_SAIDA; k++)
			{
				// Calcular os erros para os neur??nios da camada de sa??da
				erroSaida[k] = calcularErroSaida(SIGMOIDE, saidas_desejaveis[j][k], saidas_saida[k]);
				// Calcular erro da rede
				erroRede += erroSaida[k];
			}
			erroRede = pow(erroRede, 2) / 2;
			for (int k = 0; k < NEURONIOS_CAMADA_OCULTA; k++)
			{
				// Calcular os erros nos neur??nios da camada oculta
				erroOculta[k] = calcularErroOculta(SIGMOIDE, saidas_oculta[k], erroSaida, k, pesos_saida);
			}
			// Ajuste de pesos da camada de sa??da
			for (int k = 0; k < NEURONIOS_CAMADA_SAIDA; k++)
			{
				// Atualizando o vi??s da camada de sa??da
				pesos_saida[k][0] += TAXA_APRENDIZAGEM * (erroSaida[k] * vies_saida[k]);
				// Atualizando os pesos
				for (int l = 1; l < TAM_PESOS_SAIDA; l++)
				{
					pesos_saida[k][l] += TAXA_APRENDIZAGEM * (erroSaida[k] * saidas_oculta[l - 1]);
				}
			}
			// Ajuste de pesos da camada oculta
			for (int k = 0; k < NEURONIOS_CAMADA_OCULTA; k++)
			{
				// Atualizando o vi??s da camada oculta
				pesos_oculta[k][0] += TAXA_APRENDIZAGEM * (erroOculta[k] * vies_oculta[k]);
				// Atualizando os pesos
				for (int l = 0; l < TAM_PESOS_OCULTA; l++)
				{
					pesos_oculta[k][l] += TAXA_APRENDIZAGEM * (erroOculta[k] * conjunto_treinamento[k][l - 1]);
				}
			}
			epocasTotais++;
			cout << epocasTotais << " - " << erroRede << endl;
		} while (epocasTotais < EPOCAS && erroRede > TOLERANCIA);
	}
	/*cout << "Rede treinada com " << epocasTotais << " ??pocas" << endl;
	/*cout << "Erro da rede: " << erroRede << endl
		 << endl;*/
	cout << "FASE DE TESTES" << endl;
	for (int j = 0; j < QTD_AMOSTRAS; j++)
	{
		for (int k = 0; k < NEURONIOS_CAMADA_OCULTA; k++)
		{
			// C??lculo dos nets da camada oculta
			nets_oculta[k] = calcularNets(pesos_oculta[k], conjunto_treinamento[j], vies_oculta[k],
										  NEURONIOS_CAMADA_ENTRADA);
			// Aplica????o da fun????o de transfer??ncia nos neur??nios da camada oculta
			saidas_oculta[k] = funcaoTransferencia(SIGMOIDE, nets_oculta[k]);
		}
		cout << "SA??DA: ";
		for (int k = 0; k < NEURONIOS_CAMADA_SAIDA; k++)
		{
			// C??lculo dos nets da camada de sa??da
			nets_saida[k] = calcularNets(pesos_saida[k], saidas_oculta, vies_saida[k], NEURONIOS_CAMADA_OCULTA);
			// Aplica????o da fun????o de transfer??ncia nos neur??nios da camada de sa??da
			saidas_saida[k] = funcaoTransferencia(SIGMOIDE, nets_saida[k]);
			cout << saidas_saida[k] << ',';
		}
		/*cout << endl;
		for(int k = 0; k < NEURONIOS_CAMADA_SAIDA; k++) {
			cout << saidas_desejaveis[j][k] << ',';
			if(k == 0 && saidas_desejaveis[j][k] > 0.5) {
				cout << "O n??mero ?? 5";
			} else if(k == 1 && saidas_desejaveis[j][k] > 0.5) {
				cout << "O n??mero ?? 7";
			} else {
				cout << "N??o ?? nenhum desses n??meros";
			}

		}*/
		cout << endl;
	}
	// cout << "Erro de rede: " << erroRede << endl;
	/*
	n??meros escolhidos para representar as duas sa??da:
	Primeiro n??mero = 5 (cinco)
	JOAO
	2+1+1+1 = 5

	Segundo n??mero = 7 (sete)
	MELQUISEDEQUE
	(6*2+7)/3 = 6,33 = 7

	Os demais n??meros ser??o parte da sa??da desconhecida:
	Sa??da desconhecida = 0,1,2,3,4,6,7,9
	0 = 0110100110010110
	1 = 0010011000100111
	2 = 0110000101110111
	3 = 0111001100010110
	4 = 1010111000100010
	5 = 1111111000101110
	6 = 1110100011101110
	7 = 1111000100010001
	8 = 0110011001100110
	9 = 0110011000100110
	*/
}
