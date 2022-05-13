#include <iostream>
#include <string>
#include <set>
#include <map>

class PhoneNumber{
    //format +7 495 456 54 55
    int country {0};
    int region {0};
    int number {0};

public:
    std::string get_phoneNumberStr () {
        return '+' + std::to_string(country) + std::to_string(region) + std::to_string(number);
    }

    bool set_RussianPhoneNumber (const std::string & str) {
        std::string buffer;
        bool wasPlus = false;
        for (const auto & item : str) {
            if (item >= '0' and item <= '9' and wasPlus) {
                buffer += item;
            }
            else if (item == '+' and !wasPlus){
                wasPlus = true;
            }
            else {
                return false;
            }
        }

        if (buffer.length() != 11) {
            return false;
        }

        //the country code is only 7 according to the condition of the problem
        if (buffer[0] == '7') {
            country = 7;
        }
        else {
            return false;
        }

        region = std::stoi(buffer.substr(1, 3));
        number = std::stoi(buffer.substr(4));

        return true;
    };

    bool isEmpty () {
        return country == 0 and region == 0 and number == 0;
    }

    friend bool operator< (const PhoneNumber & lhs, const PhoneNumber & rhs) {
        if      (lhs.country < rhs.country) return true;
        else if (lhs.country > rhs.country) return false;
        else if (lhs.region < rhs.region)   return true;
        else if (lhs.region > rhs.region)   return false;
        else if (lhs.number < rhs.number)   return true;
        else                                return false;
    }

    friend bool operator== (const PhoneNumber & lhs, const PhoneNumber & rhs) {
        return (    lhs.country == rhs.country
                and lhs.region == rhs.region
                and lhs.number == rhs.number
               );
    }

};

class AddressEntry {
    std::string name;
    std::set<PhoneNumber> numbers;

public:
    AddressEntry (const std::string & sameName, const PhoneNumber & sameNumber) {
        name = sameName;
        numbers.emplace(sameNumber);
    }

    PhoneNumber get_firstNumber () {
        return this->numbers.begin().operator*();
    }

    bool addNumber (const PhoneNumber & newNumber) {
       return numbers.emplace(newNumber).second;
    }

    //compare by name
    friend bool operator< (const AddressEntry & lhs, const AddressEntry & rhs) {
        return lhs.name.compare(rhs.name) < 0;
    }

    friend bool operator== (const AddressEntry & lhs, const AddressEntry & rhs) {
        return lhs.name == rhs.name;
    }

};


class Mobile {
    std::map<PhoneNumber, std::string> numberMap;
    //pair of Number, Name. Guarantees a unique Number and used for search by number. value - ref name from nameMap

    std::map<std::string, AddressEntry> nameMap;
    //guarantees a unique Name (using compare by AddressEntry.name) and used for find of name

public:

    bool add (const std::string & newName, const std::string & newNumberStr){
        //name may not be unique, number just be unique
        //check number
        PhoneNumber newNumber;
        if (!newNumber.set_RussianPhoneNumber(newNumberStr)) return false;
        if (numberMap.find(newNumber) != numberMap.end()) return false;

        //add entry into nameMap;
        auto iter = nameMap.find(newName);
        if (iter == nameMap.end()){
            nameMap.emplace(newName, AddressEntry(newName, newNumber));

        }
        else {
            iter->second.addNumber(newNumber);
        }

        //add phone number into numberMap
        numberMap.emplace(newNumber, newName);

        return true;
    }

    PhoneNumber search (const std::string & key) {
        PhoneNumber number;
        if (number.set_RussianPhoneNumber(key)) {
            return number;
        }
        else {
            auto it = nameMap.find(key);
            if (it == nameMap.end()){
                return number;
            }
            else {
                return it->second.get_firstNumber();
            }
        }
    }
};

template<typename Type>
Type inputSomething (const std::string & previewText) {
    Type result;
    while (true) {
        std::cin.rdbuf()->pubsync();
        std::cin.clear();
        std::cout << previewText;
        std::cin >> result;
        if (!std::cin.fail()) {
            break;
        }
    }
    return result;
}

int main() {
    Mobile mobile;

    while (true) {
        auto command = inputSomething<std::string> ("Input command: ");

        if (command == "add") {
            auto name = inputSomething<std::string> ("Input name: ");
            auto number = inputSomething<std::string> ("Input phone number: ");
            if (!mobile.add(name, number)){
                std::cout << "Error!" << std::endl;
            }
        }
        else if (command == "call") {
            auto key = inputSomething<std::string> ("Input name or phone number for call: ");
            PhoneNumber number = mobile.search(key);
            if (number.isEmpty()) {
                std::cout << "The name is not find." << std::endl;
            }
            else {
                std::cout << "CALL " << number.get_phoneNumberStr() << std::endl;
            }
        }
        else if (command == "sms"){
            auto key = inputSomething<std::string> ("Input name or phone number for sms: ");
            PhoneNumber number = mobile.search(key);
            if (number.isEmpty()) {
                std::cout << "The name is not find." << std::endl;
            }
            else {
                auto message  = inputSomething<std::string> ("Input message: ");
                std::cout << "SMS " << number.get_phoneNumberStr() << std::endl;
            }
        }
        else if (command == "exit"){
            break;
        }
        else {
            std::cout << "Unknown command" << std::endl;
        }
    }
}