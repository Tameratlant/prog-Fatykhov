#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <windows.h>

using namespace std;



class Player {
public:
    std::vector<std::string> files;
    void SetMusic(const std::vector<std::string>& musicFiles) {
        files = musicFiles;
    }
    void Play(int period) {
        int i = 0;
        while (i < files.size()) {
            sf::Music music;
            if (!music.openFromFile(files[i]))
            {
                cout << "Can't open this file";
                exit(-1);
            }
            music.play();
            Sleep(period * 1000);
            music.Stopped;
            i++;
        }
    }
};

int main()
{
    Player player;
    std::vector<std::string> files({"ISB - Who I Am (online-audio-converter.com).wav" ,  
        "Dope_-_Die_Mother_Fucker_Die (online-audio-converter.com).wav" ,  
        "Sabaton_-_The_Unkillable_Soldier (online-audio-converter.com).wav" });
    player.SetMusic(files);
    player.Play(5);
        return 0;
}
