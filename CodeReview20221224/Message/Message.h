#ifndef MESSAGE_MESSAGE_H
#define MESSAGE_MESSAGE_H


#include <any>


namespace message {
template<typename Enum>
class Message {
private:
    Enum type;
    std::any data;
public:
    Message() = default;
    
    Message(Enum type) :
        type(type),
        data() {
    }

    template<typename Data>
    Message(Enum type, const Data& data) :
        type(type),
        data(data) {
    }
    virtual ~Message() = default;

    Enum GetType(void) const noexcept {
        return this->type;
    }
    template<typename Data>
    Data* GetDataPtr(void)const {
        return std::any_cast<Data*>(data);
    }
    bool HasData(void) const noexcept {
        return data.has_value();
    }
};
}
#endif // !MESSAGE_MESSAGE_H