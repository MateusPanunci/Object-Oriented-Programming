#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;


class CsvReader{
    private:
        string arq_name;
        string type_array;
        char delimiter_cell;
        int qtd_lines;
        int qtd_columns;
        int** int_array;
        float** float_array;
        int* label_array;

        void countLinesAndColumns(ifstream &file);
        void identify_type(ifstream &file);
        void analizeFile(string name_arq);
        void readingCode(ifstream &file, bool &split_label);

    public: 
        CsvReader(string name_arq, char delimiter);
        int** readIntArray(bool split_label);
        float** readFloatArray(bool split_label);
        void show_array();
        int getQtdLines();
        int getQtdColumns();
        string getType();
        int* getLabels();
        int* flattenIntMatrix();
        float* flattenFloatMatrix();
};



// -------------------------- Private Implementation ----------------------------- //

void CsvReader::countLinesAndColumns(ifstream &file){
    string line;
    bool count_columns = true;

    while(getline(file, line)){
        if(line.find_first_not_of("0123456789.,;-+/") != string::npos) continue;
        
        qtd_lines++;

        if (count_columns == true){
            stringstream ss(line);
            string cell;

            while(getline(ss, cell, delimiter_cell)){
                qtd_columns++;
            }
        }
        count_columns = false;
    }
    file.clear();
    file.seekg(0, std::ios::beg);
}


void CsvReader::identify_type(ifstream &file){
    type_array = "int";
    string line;

    while(getline(file, line)){
        if(line.find_first_not_of("0123456789.,;-+/") != string::npos) continue;

        stringstream ss(line);
        string cell;

        while(getline(ss, cell, delimiter_cell)){
            size_t pos = cell.find(".");
            stringstream cc(cell);

            if (pos != string::npos){
                if (cell.find_first_not_of('0', pos+1) != string::npos) {
                    type_array = "float";
                }
            }
        }
    }
    file.clear();
    file.seekg(0, std::ios::beg);
}


void CsvReader::analizeFile(string name_arq){
    ifstream file (name_arq);
    identify_type(file);
    countLinesAndColumns(file);
    file.close();
}


void CsvReader::readingCode(ifstream &file, bool &split_label){
    string line;

    if (split_label == true) {
        qtd_columns -= 1;
        label_array = new int [qtd_lines];
    }

    if (type_array == "int") {
        int_array = new int* [qtd_lines];

        for (int i = 0; i < qtd_lines; i++){
            int_array[i] = new int [qtd_columns];
        }

    } else {
        float_array = new float* [qtd_lines];

        for (int i = 0; i < qtd_lines; i++){
            float_array[i] = new float [qtd_columns];
        }

    }

    int i = 0;
    while(getline(file, line)){
        if (line.find_first_not_of("0123456789.,;-+/") != string::npos) continue;

        int j = 0, int_val;
        float float_val;

        stringstream ss(line);
        string cell;

        while(getline(ss, cell, delimiter_cell)){
            stringstream val(cell);
            if(j == qtd_columns && split_label == true){
                val >> int_val;
                label_array[i] = int_val;
                continue;
            }

            if (type_array == "int"){
                if (cell.empty()) int_val = 0;
                else val >> int_val;

                int_array[i][j] = int_val;

            } else {
                if (cell.empty()) float_val = 0;
                else val >> float_val;
                float_array[i][j] = float_val;
            }
            j++;
        }
        i++;
    }
}

// --------------------------------------------------------------------- //


// -------------------- Public Implementation ------------------------- //

CsvReader::CsvReader(string name_arq, char delimiter = ','){
    arq_name = name_arq;
    delimiter_cell = delimiter;
    float_array = nullptr;
    int_array = nullptr;
    qtd_lines = 0;
    qtd_columns = 0;
    label_array = nullptr;
    analizeFile(name_arq);
}   


/**
 * @brief Cria uma matriz 2d do tipo int com o conteúdo de um CSV.
 * @param split_label Separa o label das features se estiverem no mesmo arquivo. Por padrão
 * o parâmetro é false. Coloque true se desejar separá-los.
 * @return Um ponteiro para a matriz 2d criada.
 */

int** CsvReader::readIntArray(bool split_label = false){
    ifstream file(arq_name);
    if (!file.is_open()){
        cerr << "Erro na abertura do arquivo!" << endl;
        return nullptr;
    } else {
        readingCode(file, split_label);
        file.close();
        return int_array;
    }
}


/**
 * @brief Cria uma matriz 2d do tipo float com o conteúdo de um CSV.
 * @param split_label Separa o label das features se estiverem no mesmo arquivo. Por padrão
 * o parâmetro é false. Coloque true se desejar separá-los.
 * @return Um ponteiro para a matriz 2d criada.
 */
float** CsvReader::readFloatArray(bool split_label = false){
    ifstream file(arq_name);
    if (!file.is_open()){
        cerr << "Erro na abertura do arquivo!" << endl;
        return nullptr;
    } else {
        readingCode(file, split_label);
        file.close();
        return float_array;
    }
}


void CsvReader::show_array(){
    cout << "Array bidimensional lido do CSV: \n\n";
    if (type_array == "float"){
        for (int i = 0; i < qtd_lines; i++){
            for (int j = 0; j < qtd_columns; j++){
                cout << "  " << float_array[i][j];
            }
            cout << "\n";
        }
    } else {
        for (int i = 0; i < qtd_lines; i++){
            for (int j = 0; j < qtd_columns; j++){

            }
            cout << "\n";
        }
    }
}

/**
 * @brief Retorna o número da quantidade de linhas da matrix.
 */
int CsvReader::getQtdLines(){
    return qtd_lines;
}


/**
 * @brief Retorna o número da quantidade de colunas da matrix.
 * @return tipo int.
 */
int CsvReader::getQtdColumns(){
    return qtd_columns;
}

string CsvReader::getType(){
    return type_array;
}
/**
 * @brief Retorna um ponteiro para o array de labels lido, se o argumento do método read() for incluído um true.
 * @return tipo ponteiro de int.
 */
int* CsvReader::getLabels(){
    if (label_array == nullptr){
        cerr << "Não há labels no arquivo lido!";
        return nullptr;
    } else return label_array;
}


/**
 * @brief Converte uma matriz do tipo int com x linhas e 1 coluna para um array 1d.
 * @return Um ponteiro para o array unidimensional criado.
 */
int* CsvReader::flattenIntMatrix(){
    if (qtd_columns == 1){
        int* array = new int [qtd_lines];
        for (int i = 0; i < qtd_lines; i++){
            array[i] = int_array[i][0];
        }
        return array;
    } else {
        cerr << "A matriz não possui 1 coluna apenas" << endl;
        return nullptr;
    }
}


/**
 * @brief Converte uma matriz do tipo float com x linhas e 1 coluna para um array 1d.
 * @return Um ponteiro para o array unidimensional criado.
 */
float* CsvReader::flattenFloatMatrix(){
    if (qtd_columns == 1){
        float* array = new float [qtd_lines];
        for (int i = 0; i < qtd_lines; i++){
            array[i] = float_array[i][0];
        }
        return array;
    } else {
        cerr << "A matriz não possui 1 coluna apenas" << endl;
        return nullptr;
    }
}


