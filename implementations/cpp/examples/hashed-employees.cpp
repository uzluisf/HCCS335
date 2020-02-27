#include "../lib/HashTable.h"

class Employee {
    public:
        Employee(std::string name, double salary, int seniority) : _name{ name }, _salary{ salary }, _seniority{seniority} {}

        const std::string& get_name() const {
            return _name;
        }

        bool operator==( const Employee& rhs ) const {
            return get_name() == rhs.get_name();
        }

        bool operator!=( const Employee& rhs ) const {
            return !(*this == rhs);
        }

    private:
        std::string _name;
        double _salary;
        int _seniority;
};

// Providing Employee class's hash function
template<>
class hash<Employee> {
    public:
        size_t operator()( const Employee& item ) {
            static hash<std::string> hf;
            return hf(item.get_name());
        }
};

int main() {
    Employee johnl{"John Letter", 65000, 3};
    Employee janew{"Jane Word", 78000, 4};
    Employee petert{"Peter Trubonm", 90000, 5};

    HashTable<Employee> ht;
    ht.insert(johnl);
    ht.insert(janew);
    ht.insert(petert);
}
