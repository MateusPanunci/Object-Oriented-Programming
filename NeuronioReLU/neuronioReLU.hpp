#include "neuronio.hpp"
#include <iostream>


class NeuronioReLu : public Neuronio {
    public:
        NeuronioReLu(const vector<double>& peso, double b): Neuronio(peso, b){};

        double predict(const vector<double> entradas) const{
            if (entradas.size() != pesos.size()){
               std::cout << "Quantidade de entradas diferente da de pesos ! ";
                return -1; // Erro
            }

            double account;
            for (int i = 0; i< entradas.size(); i++){
                account += (pesos[i] * entradas[i]);
            }

            account += bias;

            return account > 0 ? account : 0;
        };

        ~NeuronioReLu(){};



};