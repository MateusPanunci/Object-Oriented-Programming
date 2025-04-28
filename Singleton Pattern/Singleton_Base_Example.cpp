#include <iostream>
#include <string>
using namespace std;

class Singleton{
    private:
        string nome, origem; //variaveis            
        static Singleton *instancePtr; //inicializando ponteiro
        
        Singleton(){}// contrutor
        
    public:
        Singleton(const Singleton &obj) = delete; //limpando copia 
        
        // Instanciando e retornando 
        static Singleton *getInstance(){
            if (instancePtr == nullptr) 
                instancePtr = new Singleton();
    
            return instancePtr;   
        }

        //setter
        void setValues(string nome,string origem){
            this -> nome = nome;
            this -> origem = origem;
        }

        void print(){
            cout << nome << " nasceu em  " << origem << "." << endl;
        }
};

//Inicializa o ponteiro como NULL
Singleton* Singleton::instancePtr = nullptr;

int main()
{
  Singleton *hei = Singleton::getInstance();
  hei->setValues("Heitor","Londrina");
  hei->print();
  cout << "Address of hei : " << hei << endl;
  cout << endl; 
      
  Singleton *mat = Singleton::getInstance();
  mat->setValues("Mateus", "Londrina");
  mat->print();
  cout << "Address of mat : " << mat << endl;
  cout << endl;

  Singleton *egl = Singleton::getInstance();
  egl->setValues("Eglius","Ibipora");
  egl->print();
  cout << "Address of egl : " << egl << endl;
  cout << endl;

  return 0;
}
