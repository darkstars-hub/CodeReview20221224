#ifndef MESSAGE_MESSAGE_SUBJECT_H
#define MESSAGE_MESSAGE_SUBJECT_H


#include <vector>

#include "MessageManager.h"


namespace message {
class MessageSubject {
public:
    using Message = Message<message::MessageId>;
private:
    using Element = MessageListener*;
    using MessageListenerVector = std::vector<Element>;
    using MessageTypeVector = std::vector<message::MessageId>;

    MessageListener* const _owner;
    MessageManager* const _message_manager;
    MessageListenerVector _message_listeners;
    MessageTypeVector _receive_messages;
    MessageTypeVector _send_messages;
public:
    MessageSubject(
        MessageListener* const owner,
        MessageManager* message_manager,
        MessageTypeVector receive_messages,
        MessageTypeVector send_messages
    ) :
        _owner(owner),
        _message_manager(message_manager),
        _receive_messages(receive_messages),
        _send_messages(send_messages) {
        _message_manager->Add(_owner);
    }
    ~MessageSubject() = default;

    const MessageTypeVector& ReceiveMessages(void) const noexcept {
        return this->_receive_messages;
    }

    const MessageTypeVector& SendMessages(void) const noexcept {
        return this->_send_messages;
    }

    void Notify(const Message& message) const {
        for(auto message_listener : _message_listeners) {
            message_listener->Recieve(message);
        } // for
    }
    void Broadcast(const MessageListener::Message& message, MessageListener* exclude) {
        _message_manager->Broadcast(message, exclude);
    }

    bool Contain(Element listener) {
        for(auto message_listener : _message_listeners) {
            if(message_listener == listener) {
                return true;
            } // if
        } // for
        return false;
    }
    void Attach(Element listener) {
        _message_listeners.push_back(listener);
    }
    void Dettach(Element listener) {
        auto& v = _message_listeners;
        v.erase(std::remove(v.begin(), v.end(), listener), v.end());
    }


    //! IDを比較し送信するデータタイプが受信するデータに含まれていれば通知先に追加する。
    void UtilInsert(Element listener,
                    const MessageTypeVector& recieve_messages,
                    const MessageTypeVector& send_messages) {
        if(!this->Contain(listener)) {
            if(::EqualBruteforce(recieve_messages, send_messages)) {
                this->Attach(listener);
            } // if
        } // if
    }
    void Destroy(void) {
        _message_manager->Remove(_owner);
    }
};

}
#endif // !MESSAGE_MESSAGE_SUBJECT_H