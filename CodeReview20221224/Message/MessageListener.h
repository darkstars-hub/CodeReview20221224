#ifndef MESSAGE_MESSAGE_LISTENER_H
#define MESSAGE_MESSAGE_LISTENER_H


#include <type_traits>
#include <iostream>
#include <vector>

#include "Message.h"
#include "MessageId.h"


template<typename Container>
bool EqualBruteforce(const Container& left, const Container& right) {
    for(auto& l : left) {
        for(auto& r : right) {
            if(l == r) {
                return true;
            } // if
        } // for
    } // for
    return false;
};


namespace message {
class MessageListener {
public:
    using Message = Message<message::MessageId>;
private:
public:
    MessageListener() = default;
    virtual ~MessageListener() = default;

    virtual void Recieve(const Message& message) = 0;
};
}
#endif // !MESSAGE_MESSAGE_LISTENER_H