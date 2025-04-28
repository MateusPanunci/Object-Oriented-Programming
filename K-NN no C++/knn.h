#include <vector>
#include <algorithm>
#include <cmath>
#include <random>

class modelerKnn{
    private:
        int k;
        int qtdLinesTreino;
        int qtdColumnsTreino;
        int qtdLinesTeste;
        float** floatTreinoMatrix;
        int** intTreinoMatrix;
        float** floatTeste;
        int** intTeste;
        int* labelTreino;
        int* labelTeste;
        int qtdLabels;
        string type;
        vector<pair<int, int>> counter;

        void analizeDimensions(CsvReader arq_treino);
        void analizeDifferentLabels();
        void splitRandomSamples(int** matrix, int* labels, float percentualTreino);
        void splitRandomSamples(float** matrix, int* labels, float percentualTreino);

    public:
       modelerKnn();   
       modelerKnn(int val);
       void fit(float** matrix, int* labels, CsvReader &arq_treino, bool split_samples);
       void fit(int** matrix, int* labels, CsvReader &arq_treino, bool split_samples);
       int* predict(); 
       int* predict(float** matrix_teste, CsvReader arq_teste); 
       int* predict(int** matrix_teste, CsvReader arq_teste);
       int* getLabelTeste();
       int getQtdLinesTeste();
};

modelerKnn::modelerKnn(){
    k = 5;
}


modelerKnn::modelerKnn(int val){
    k = val;
}


void modelerKnn::analizeDimensions(CsvReader arq_treino){
    qtdLinesTreino = arq_treino.getQtdLines();
    qtdColumnsTreino = arq_treino.getQtdColumns();
}



void modelerKnn::analizeDifferentLabels(){
    for (int a = 0; a < qtdLinesTreino; a++){
        if(a == 0){
            counter.emplace_back(labelTreino[a], 0);
            continue;
        } else{
            if(labelTreino + a == find(labelTreino, labelTreino + a, labelTreino[a])){
                counter.emplace_back(labelTreino[a], 0);
            }
        }
    }
}


void modelerKnn::splitRandomSamples(int** matrix, int* labels, float percentualTreino){
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, qtdLinesTreino);  

    int prev_qtd = qtdLinesTreino; 
    qtdLinesTreino *= percentualTreino;
    qtdLinesTeste = prev_qtd - qtdLinesTreino;


    intTeste = new int* [qtdLinesTeste];
    intTreinoMatrix = new int*[qtdLinesTreino];
    
    for (int i = 0; i < qtdLinesTeste; i++){
        int rand_num = dist(mt);

        while (matrix == nullptr && rand_num < qtdLinesTreino)rand_num++;
        if (rand_num == qtdLinesTreino) rand_num = 0;

        intTeste[i] = matrix[rand_num] ;
        matrix[rand_num] == nullptr;

        labelTeste[i] = labels[rand_num];
    }

    int i = 0;
    for(int ind = 0; ind < prev_qtd; ind++){
        if (matrix[ind] != nullptr){
            intTreinoMatrix[i] = matrix[ind];
            matrix[ind] = nullptr;

            labelTreino[i] = labels[ind];
            
            i++;
        } 
    }
    for (int ind = 0; ind < prev_qtd; ind++) {delete[] matrix[ind];}
    delete [] matrix;
    delete [] labels;
}
    
void modelerKnn::splitRandomSamples(float **matrix, int*labels, float percentualTreino){
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<int> dist(0, qtdLinesTreino - 1);  

        int prev_qtd = qtdLinesTreino; 
        qtdLinesTreino *= percentualTreino;
        qtdLinesTeste = prev_qtd - qtdLinesTreino;

    
        labelTreino = new int [qtdLinesTreino];
        labelTeste = new int [qtdLinesTeste];
        floatTreinoMatrix = new float*[qtdLinesTreino];
        floatTeste = new float* [qtdLinesTeste];

        for (int i = 0; i < qtdLinesTeste; i++){
            int rand_num = dist(mt);
            while (matrix[rand_num] == nullptr && rand_num <= prev_qtd)rand_num++;
            if (rand_num == prev_qtd) rand_num = 0;

            floatTeste[i] = matrix[rand_num];
            matrix[rand_num] = nullptr;

            labelTeste[i] = labels[rand_num];
        }

        int i = 0;
        for(int ind = 0; ind < prev_qtd; ind++){
            if (matrix[ind] != nullptr){
                floatTreinoMatrix[i] = matrix[ind];
                matrix[ind] = nullptr;

                labelTreino[i] = labels[ind];
                
                i++;
            } 
        }

        for (int ind = 0; ind < prev_qtd; ind++) {delete[] matrix[ind];}
        delete [] matrix;
        delete [] labels;

}


void modelerKnn::fit(float** matrix, int* labels, CsvReader &arq_treino, bool split_samples = false){
    analizeDimensions(arq_treino);
    if (split_samples == true){
        splitRandomSamples(matrix, labels, 0.8);
        analizeDifferentLabels();
    } else{
        floatTreinoMatrix = matrix;
        labelTreino = labels;
        qtdLinesTeste = 0;
        analizeDifferentLabels();
    }
}


void modelerKnn::fit(int** matrix, int* labels, CsvReader &arq_treino, bool split_samples = false){
    analizeDimensions(arq_treino);
    if (split_samples = true){
        splitRandomSamples(matrix, labels, 0.8);
        analizeDifferentLabels();
    } else{
        intTreinoMatrix = matrix;
        labelTreino = labels;
        qtdLinesTeste = 0;
        analizeDifferentLabels();
    }
}


int* modelerKnn::predict(){
    int* label_knn = new int [qtdLinesTeste];

    for (int a = 0; a < qtdLinesTeste; a++){
        vector <pair<float, int>> distancias;
        distancias.reserve(500);
        for (int b = 0; b < qtdLinesTreino; b++){
            float temp = 0;
            for(int c = 0; c < qtdColumnsTreino; c++){
                if(type == "int") temp += pow(intTeste[a][c] - intTreinoMatrix[b][c], 2);
                else temp += pow(floatTeste[a][c] - floatTreinoMatrix[b][c], 2);
            } 

            distancias.emplace_back(sqrt(temp), labelTreino[b]);
        }

        sort(distancias.begin(), distancias.end(), [](const pair<float, int> &a, const pair<float, int> &b){
            return a.first < b.first;}
        );

        
        vector<pair<float, int>> neighbors;
        neighbors.resize(k);
        copy(distancias.begin(), distancias.begin() + k, neighbors.begin());

        int index = 0;
        while (index < k){
            for(int b = 0; b < counter.size(); b++){
                if (neighbors[index].second == counter[b].first){
                    counter[b].second += 1;
                }
            }
            index++;
        }
    
        sort(counter.begin(), counter.end(), [](const pair<int, int> &a, const pair<int, int> &b){return a.second < b.second;});

        if(counter.back().second == counter.at(counter.size() - 2).second) label_knn[a] = neighbors.front().first;
        else label_knn[a] = counter.back().first;
        for (int i = 0; i < counter.size(); i++) counter[i].second = 0;
    }

    return label_knn;
}


int* modelerKnn::predict(float** matrix_teste, CsvReader arq_teste){
    qtdLinesTeste = arq_teste.getQtdLines();
    if (matrix_teste == nullptr)matrix_teste = floatTeste;

    int* label_knn = new int [qtdLinesTeste];
    for (int a = 0; a < qtdLinesTeste; a++){
        vector <pair<float, int>> distancias;
        distancias.reserve(500);

        for (int b = 0; b < qtdLinesTreino; b++){
            float temp = 0;
            for(int c = 0; c < qtdColumnsTreino; c++){
                temp += pow(matrix_teste[a][c] - floatTreinoMatrix[b][c], 2);
            } 
            distancias.emplace_back(sqrt(temp), labelTreino[b]);
            
        }

        sort(distancias.begin(), distancias.end(), [](const pair<float, int> &a, const pair<float, int> &b){
            return a.first < b.first;}
        );

        vector<pair<float, int>> neighbors;
        neighbors.resize(k);
        copy(distancias.begin(), distancias.begin() + k, neighbors.begin());

        int index = 0;
        while (index < k){
            for(int b = 0; b < counter.size(); b++){
                
                if (neighbors[index].second == counter[b].first){
                counter[b].second += 1;
                }
            }
            index++;
        }
    
        sort(counter.begin(), counter.end(), [](const pair<int, int> &a, const pair<int, int> &b){return a.second < b.second;});

        if(counter.back().second == counter.at(counter.size() - 2).second) label_knn[a] = neighbors.front().first;
        else label_knn[a] = counter.back().first;
        
        for (int i = 0; i < counter.size(); i++) counter[i].second = 0;
    }

    return label_knn;
}


int* modelerKnn::predict(int** matrix_teste, CsvReader arq_teste){
    qtdLinesTeste = arq_teste.getQtdLines();
    if (matrix_teste == nullptr)matrix_teste = intTeste;

    int* label_knn = new int [qtdLinesTeste];

    for (int a = 0; a < qtdLinesTeste; a++){
        vector <pair<float, int>> distancias;
        distancias.reserve(500);

        for (int b = 0; b < qtdLinesTreino; b++){
            float temp = 0;
            for(int c = 0; c < qtdColumnsTreino; c++){
                temp += pow(matrix_teste[a][c] - intTreinoMatrix[b][c], 2);
            } 

            distancias.emplace_back(sqrt(temp), labelTreino[b]);
        }

        sort(distancias.begin(), distancias.end(), [](const pair<float, int> &a, const pair<float, int> &b){
            return a.first < b.first;}
        );

        vector<pair<float, int>> neighbors;
        copy(distancias.begin(), distancias.begin() + k, neighbors.begin());

        int index = 0;
        while (index < k){
            for(int b = 0; b < counter.size(); b++){
                 if (neighbors[index].second == counter[b].first){
                    counter[b].second += 1;
                 }
            }
            index++;
        }
    
        sort(counter.begin(), counter.end(), [](const pair<int, int> &a, const pair<int, int> &b){return a.second < b.second;});

        if(counter.back().second == counter.at(counter.size() - 2).second) label_knn[a] = neighbors.front().first;
        else label_knn[a] = counter.back().first;
        for (int i = 0; i < counter.size(); i++) counter[i].second = 0;
    }

    return label_knn;
}


int* modelerKnn::getLabelTeste(){
    //colocar um if
    return labelTeste;
}

int modelerKnn::getQtdLinesTeste(){
    //colocar um if
    return qtdLinesTeste;
}
