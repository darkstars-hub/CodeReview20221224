#ifndef MESSAGE_MESSAGE_MANAGER_H
#define MESSAGE_MESSAGE_MANAGER_H


#include <vector>

#include "MessageListener.h"


namespace message {
class MessageManager {
public:
    template<typename T>
    using Container = std::vector<T>;
    using Listener = message::MessageListener*;
    using ListenerContainer = Container<Listener>;
private:
    Container<Listener> _listeners;
public:
    MessageManager() = default;
    ~MessageManager() = default;

    void Broadcast(const MessageListener::Message& message, Listener exclude) {
        for(auto l : _listeners) {
            if(l != exclude) {
                l->Recieve(message);
            } // if
        } // for
    }
    void Clear(void) {
        _listeners.clear();
    }
    void Add(const Listener listener) {
        _listeners.push_back(listener);
    }
    void Remove(const Listener listener) {
        auto& v = _listeners;
        v.erase(std::remove(v.begin(), v.end(), listener), v.end());
    }
};
}
#endif // !MESSAGE_MESSAGE_MANAGER_H