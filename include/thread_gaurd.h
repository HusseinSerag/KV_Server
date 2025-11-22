#include <thread>

//RAII design to make sure threads are joined or detched when used
// mode = 0 join, mode = 1 then detach
class thread_gaurd {
    std::thread &t;
    uint8_t mode;
    public:
    thread_gaurd(std::thread& _t, uint8_t mode = 0): t(_t), mode(mode) {} 
    thread_gaurd(const thread_gaurd&)=delete;
    thread_gaurd& operator=(const thread_gaurd&)=delete;
    ~thread_gaurd() {
        if(mode == 0){
            if(t.joinable()){
                t.join();
            }
        } else {
            t.detach();
        }
    }
};