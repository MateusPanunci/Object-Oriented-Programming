#include "read_csv.h"

/* Se o terminal não imprimir todas as linhas, Pressione "Ctrl + Shift + P" 
e digite "Preferences: Open User Settings". Depois procure por Terminal > Integrated: Scrollback 
e mude de 1000 para umas 3000 linhas. Se não, imprima uma instância por vez que irá funcionar. */


int main(){
    CsvReader arq = CsvReader("Float_dataset_1.csv");
    CsvReader arq_2 = CsvReader("Float_dataset_2.csv");
    CsvReader arq_label = CsvReader("labels.csv");
    
    float** array = arq.readFloatArray();
    float** array_2 = arq_2.readFloatArray();
    
    int** array_int = arq_label.readIntArray();
    int* label = arq_label.flattenIntMatrix();

    cout << "---------------------------------------------\nPrimeiro Array bidimensiownal de tipo float: \n\n";
    for (int i = 0; i < arq.get_QtdLines(); i++){
        for (int j = 0; j < arq.get_QtdColumns(); j++){
             cout << "  " << array[i][j];
         }
         cout << "\n";
    }

    cout << "---------------------------------------------\nSegundo Array bidimensional de tipo flaot: \n\n";
    for (int i = 0; i < arq_2.get_QtdLines(); i++){
         for (int j = 0; j < arq_2.get_QtdColumns(); j++){
             cout << "  " << array_2[i][j];
         }
         cout << "\n";
    }

    cout << "---------------------------------------------\nArray unidimensional de tipo int: \n\n";
    for (int i = 0; i < arq_label.get_QtdLines(); i++){
        cout << label[i] << endl;
    }
    cout << "\n";
}




