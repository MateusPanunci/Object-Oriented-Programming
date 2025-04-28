#include <typeinfo>
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

        void countLinesAndColumns(ifstream &file);
        void identify_type(ifstream &file);
        void analizeFile(string name_arq);
        void readingCode(ifstream &file);

    public: 
        CsvReader(string name_arq, char delimiter);
        ~CsvReader();
        int** readIntArray();
        float** readFloatArray();
        void show_array();
        int get_QtdLines();
        int get_QtdColumns();
        int* flattenIntMatrix();
        float* flattenFloatMatrix();
};



// -------------------------- Private Implementation ----------------------------- //

void CsvReader::countLinesAndColumns(ifstream &file){
    string line;
    bool count_columns = true;

    while(getline(file, line)){
        if(line.find_first_not_of("0123456789.,") != string::npos) continue;
        
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
        if(line.find_first_not_of("0123456789.,") != string::npos) continue;

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


void CsvReader::readingCode(ifstream &file){
    string line;
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
        if (line.find_first_not_of("0123456789.,") != string::npos) continue;

        int j = 0, int_val;
        float float_val;

        stringstream ss(line);
        string cell;

        while(getline(ss, cell, delimiter_cell)){
            stringstream val(cell);

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
    float_array = NULL;
    int_array = NULL;
    qtd_lines = 0;
    qtd_columns = 0;
    analizeFile(name_arq);
}   

CsvReader::~CsvReader(){
    if(type_array == "int"){
        for (int i = 0; i < qtd_lines; i++){
            delete [] int_array[i];
        }
        delete [] int_array;
    } else if (type_array == "float"){
        for (int i = 0; i < qtd_lines; i++){
            delete [] float_array[i];
        }
        delete [] float_array;
    }
}


/**
 * @brief Cria uma matriz 2d do tipo int com o conteúdo de um CSV.
 * @return Um ponteiro para a matriz 2d criada.
 */
int** CsvReader::readIntArray(){
    ifstream file(arq_name);
    if (!file.is_open()){
        cerr << "Erro na abertura do arquivo!" << endl;
        return NULL;
    } else {
        readingCode(file);
        file.close();
        return int_array;
    }
}


/**
 * @brief Cria uma matriz 2d do tipo float com o conteúdo de um CSV.
 * @return Um ponteiro para a matriz 2d criada.
 */
float** CsvReader::readFloatArray(){
    ifstream file(arq_name);
    if (!file.is_open()){
        cerr << "Erro na abertura do arquivo!" << endl;
        return NULL;
    } else {
        readingCode(file);
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
                cout << "  " << int_array[i][j];
            }
            cout << "\n";
        }
    }
}


int CsvReader::get_QtdLines(){
    return qtd_lines;
}


int CsvReader::get_QtdColumns(){
    return qtd_columns;
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
        cerr << "A matriz não possui 1 coluna apenas!" << endl;
        return NULL;
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
        cerr << "A matriz não possui 1 coluna apenas!" << endl;
        return NULL;
    }
}

