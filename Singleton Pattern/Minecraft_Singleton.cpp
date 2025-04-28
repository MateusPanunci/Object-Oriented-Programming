#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;


class WorldMineConfig{
    private:
        string gamemode, 
               difficult,
               gamerule, 
               pvp,
               time;

        static WorldMineConfig* config;

        WorldMineConfig(){   
            gamemode = "";
            difficult = "";
            gamerule = "";
            pvp = "";
            time = "";
        }

    public:
        static WorldMineConfig* getInstance(){
            if(config == nullptr){
                config = new WorldMineConfig();
            }

            return config;
        }

        void setConfigs(string gamemode = "",
                        string difficult = "", 
                        string gamerule = "", 
                        string pvp = "", 
                        string time = "")
        {
            if (gamemode != "") this -> gamemode = gamemode;
            if (difficult != "") this -> difficult = difficult;
            if (gamerule != "") this -> gamerule = gamerule;
            if (pvp != "") this -> pvp = pvp;
            if (time != "") this -> time = time;
        }

        
        void printConfig(){
            cout << "Configurações:\n\n"
                 << "Gamemode: " << gamemode << endl
                 << "Dificuldade: " << difficult << endl
                 << "Gamerule: " << gamerule << endl
                 << "Pvp: " << pvp << endl
                 << "Tempo: " << time << endl << endl;
        }
};

WorldMineConfig* WorldMineConfig::config = nullptr;

int main(){ 
    WorldMineConfig* mundo = WorldMineConfig::getInstance();
    mundo -> setConfigs("Sobrevivência", "Fácil", "KeepInventory", "Habilitado", "Dia");
    mundo -> printConfig();

    
    mundo -> setConfigs("Hardcore", "Difícil", "ShowDeathMessages", "",  "Noite");
    cout << " ------ Configurações alteradas! ------\n\n";
    mundo -> printConfig();
}