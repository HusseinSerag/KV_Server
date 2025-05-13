
#pragma once
#include <string>


class Value {

    public:
        virtual ~Value() = default;
        virtual void save(std::ostream& out) = 0;
        virtual void load(std::istream& in) = 0;
        virtual Value* clone() const = 0;
        virtual std::string toString() const = 0;
        static Value* get(std::string& key);
        static void del(std::string& key);
        template <typename T,typename V>
        static void set(std::string& key, V value);


};





