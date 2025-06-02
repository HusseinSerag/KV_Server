#include "NumberList.h"
#include "Value.h"
#include "Response.h"
#include "datastore.h"

NumberList::NumberList(Value* val){
    this->val = val;
}

int8_t NumberList::read(std::vector<std::string>& request, Response& res) {

}

void NumberList::execute(Storage* storage, Response& res) {

}
