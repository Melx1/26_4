#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

class Track {
    std::string name;
    std::tm creationDate {0};
    int duration;

public:
    Track(const std::string & newName, const std::string & newDate, const int & newDuration){
        if (!newName.empty()) {
            name = newName;
        }
        else {
            name = "Unknown";
        }

        std::istringstream ssDate(newDate);
        std::tm tm {0};
        ssDate >> std::get_time(&tm, "%Y.%m.%d");
        if (ssDate.fail()) {
            std::cout << "Date error." << std::endl;
        }
        creationDate = tm;

        if (newDuration > 0) {
            duration = newDuration;
        }
        else {
            duration = 0;
        }
    }

    std::string getName() {
        return name;
    }

    void show () {
        std::cout << "Name: " << name << "\nDate: " << std::put_time(& creationDate,"%Y.%m.%d") << "\nDuration:" << duration << std::endl;
    }

};

class Player {
    std::vector<Track> listTrack;
    unsigned int currentTrackNumber = 0;

public:
    bool isEmpty = true;
    bool isPlaying = false;
    bool isPause = false;

    void addTrack (const std::string & name, const std::string & createDate, const int & duration) {
        listTrack.emplace_back(name, createDate, duration);
        isEmpty = false;
    }

    void playTrack (const std::string & name) {
        if (isEmpty) {
            std::cout << "Play list is empty" << std::endl;
            isPlaying = false;
            return;
        }
        bool isFound = false;
        unsigned int idx = 0;
        for (auto & item : listTrack) {
            if (name == item.getName()) {
                isFound = true;
                isPlaying = true;
                currentTrackNumber = idx;
                item.show();
                break;
            }
            ++idx;
        }
        if (!isFound) {
            std::cout << "The track is not found" << std::endl;
            isPlaying = false;
        }
    }

    void playTrack (const int & idx) {
        if (isEmpty) {
            std::cout << "Play list is empty" << std::endl;
            isPlaying = false;
            return;
        }
        if (idx >= 0 and idx < listTrack.size()) {
            currentTrackNumber = idx;
            listTrack[idx].show();
            isPlaying = true;
            return;
        }
        isPlaying = false;
   }

    unsigned int getCurrentTrack () {
        return currentTrackNumber;
    }

    void setCurrentTrack (unsigned int newTrackNumber) {
        currentTrackNumber = newTrackNumber;
    }

    unsigned int size () {
        return listTrack.size();
    }
};


int main() {
    std::mt19937 randGenerator;
    randGenerator.seed(std::time(nullptr));

    Player myPlayer {};
    myPlayer.addTrack("track1", "2021.05.12", 180);
    myPlayer.addTrack("track2", "1879.01.01", 19875);
    myPlayer.addTrack("track3", "1900.12.30", -1);
    myPlayer.addTrack("track4", "1998.07.25", 60);

    std::string command;
    while (true) {
        while (true) {
            std::cin.rdbuf()->pubsync();
            std::cin.clear();
            std::cout << "Input command: ";
            std::cin >> command;
            if (!std::cin.fail()) {
                break;
            }
        }

        if (command == "play") {
            if (!myPlayer.isPlaying) {
                std::string buffer;
                while (true) {
                    std::cin.rdbuf()->pubsync();
                    std::cin.clear();
                    std::cout << "Input track name: ";
                    std::cin >> buffer;
                    if (!std::cin.fail()) {
                        break;
                    }
                }
                myPlayer.playTrack(buffer);
            }
        }
        else if (command == "pause"){
            if (!myPlayer.isPause and myPlayer.isPlaying){
                myPlayer.isPause = true;
                std::cout << "Playback paused" << std::endl;
            }
        }
        else if (command == "next"){
            if (myPlayer.isEmpty) {
                std::cout << "Play list is empty" << std::endl;
                continue;
            }
            std::uniform_int_distribution<int> distribution(0, myPlayer.size() - 1);
            myPlayer.playTrack(distribution(randGenerator));
        }
        else if (command == "stop") {
            if (myPlayer.isPlaying) {
                myPlayer.isPlaying = false;
                myPlayer.isPause = false;
                std::cout << "Playback stopped" << std::endl;
            }
        }
        else if (command == "exit"){
            break;
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }

    return 0;
}
