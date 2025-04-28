#include "read_csv.h"
#include "knn.h"
using namespace std;

/* Para o programa funcionar, abra a pasta, enviada no classroom, aqui no vs code com
todos os arquivos necessários */

void acuracia(int* labels_knn, int* label_teste, int qtd_lines){
    int acertos = 0;
    float acuracia;
    for (int i = 0; i < qtd_lines; i++){
        if (labels_knn[i] == label_teste[i]){
            acertos++;
        }
    }
    acuracia = float(acertos)/qtd_lines;
    cout << "Acurácia: " << acuracia*100 << " %";
}

/*Código 1: o programa separa as amostras aleatoriamente se os labels estiverem no mesmo arquivo das features*/
void codigo1(){
    CsvReader arq = CsvReader("Dataset_feat_labels.csv", ';');
    float** dataset = arq.readFloatArray(true);
    int* labels = arq.getLabels();
    modelerKnn Knn = modelerKnn(3);
    Knn.fit(dataset, labels, arq, true);


    int* labels_knn = Knn.predict();
    int* labels_teste = Knn.getLabelTeste();
    int qtdLinesTeste = Knn.getQtdLinesTeste();
    acuracia(labels_knn, labels_teste, qtdLinesTeste);
}

/*Código 2: com as amostras e os labels já separados */
void codigo2(){
    CsvReader arqTreino = CsvReader("Float_Dataset_Treino.csv");
    CsvReader arqTeste = CsvReader("Float_Dataset_Teste.csv");
    CsvReader arqLabelTreino = CsvReader("labels_treino.csv");
    CsvReader arqLabelTeste = CsvReader("labels_teste.csv");
    
    float** dataset_treino = arqTreino.readFloatArray();
    float** dataset_teste = arqTeste.readFloatArray();
    int** matrix_labels = arqLabelTreino.readIntArray();
    arqLabelTeste.readIntArray();
    int* labels_treino = arqLabelTreino.flattenIntMatrix();
    int* labels_teste = arqLabelTeste.flattenIntMatrix();

    modelerKnn Knn = modelerKnn(10);
    Knn.fit(dataset_treino, labels_treino, arqTreino);

    int* labels_knn = Knn.predict(dataset_teste, arqTeste);

    // Compara Labels:
    // for (int i = 0; i < arqLabelTeste.getQtdLines(); i++){
    //     cout << labels_knn[i] << " " << labels_teste[i] << endl;
    // }

    acuracia(labels_knn, labels_teste, arqLabelTeste.getQtdLines());
}


int main(){
   codigo1();
}




