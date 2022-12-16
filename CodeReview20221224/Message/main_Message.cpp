#include "MessageId.h"
#include "MessageManager.h"
#include "MessageSubject.h"

#include <string>


using MessageIdContainer = std::vector<message::MessageId>;

struct MessageListenerAddData {
    message::MessageListener* const listener;
    const MessageIdContainer& recieve_messages;
    MessageListenerAddData(
        message::MessageListener* const listener,
        const MessageIdContainer& recieve_messages
    ) :
        listener(listener),
        recieve_messages(recieve_messages) {
    }
};
struct MessageListenerAddReplyData {
    message::MessageListener* const listener;
    const MessageIdContainer& recieve_messages;
    MessageListenerAddReplyData(
        message::MessageListener* const listener,
        const MessageIdContainer& recieve_messages
    ) :
        listener(listener),
        recieve_messages(recieve_messages) {
    }
};
struct MessageListenerRemoveData {
    message::MessageListener* listener;
};
struct ApplicationUpdateData {
};


class Actor : public message::MessageListener {
private:
    std::string _name;
    message::MessageSubject _message_subject;
public:
    Actor(const char* name, message::MessageManager* message_manager) :
        _name(name),
        _message_subject(this, message_manager, { message::MessageId::ApplicationUpdate }, {}) {
        auto add_data = MessageListenerAddData({ this, _message_subject.ReceiveMessages() });
        message_manager->Broadcast(message::Message(message::MessageId::MessageListenerAdd, &add_data), this);
    }
    void OnMessageListenerAdd(const Message& message) {
        _ASSERT_EXPR(message.HasData(), std::string(__FILE__) + std::to_string(__LINE__));
        auto add_data = message.GetDataPtr<MessageListenerAddData>();
        _ASSERT_EXPR(!_message_subject.Contain(add_data->listener), std::string(__FILE__) + std::to_string(__LINE__));

        _message_subject.UtilInsert(
            add_data->listener, add_data->recieve_messages, _message_subject.SendMessages());

        //! 自身が通知するメッセージを返信する。
        auto reply_data = MessageListenerAddReplyData({ this, _message_subject.ReceiveMessages() });
        _message_subject.Broadcast(message::Message(message::MessageId::MessageListenerAddReply, &reply_data), this);
        return;
    }
    void OnMessageListenerAddReply(const Message& message) {
        _ASSERT_EXPR(message.HasData(), std::string(__FILE__) + std::to_string(__LINE__));
        auto reply_data = message.GetDataPtr<MessageListenerAddReplyData>();

        _message_subject.UtilInsert(
            reply_data->listener, reply_data->recieve_messages, _message_subject.SendMessages());
        return;
    }
    void OnMessageListenerRemove(const Message& message) {
        _ASSERT_EXPR(message.HasData(), std::string(__FILE__) + std::to_string(__LINE__));
        auto data = message.GetDataPtr<MessageListenerRemoveData>();
        _message_subject.Dettach(data->listener);
        return;
    }
    virtual void Recieve(const Message& message) override {
        switch(message.GetType()) {
            case message::MessageId::MessageListenerAdd:
                this->OnMessageListenerAdd(message);
                break;
            case message::MessageId::MessageListenerAddReply:
                this->OnMessageListenerAddReply(message);
                break;
            case message::MessageId::MessageListenerRemove:
                this->OnMessageListenerRemove(message);
                break;
            case message::MessageId::ApplicationUpdate:
                this->OnUpdate();
                break;
            default:
                break;
        }
    }
    void OnUpdate(void) {
        std::cout << _name.c_str() << "Update" << "\n";
    }
};
class System : public message::MessageListener {
    message::MessageSubject _message_subject;
public:
    System(message::MessageManager* message_manager) :
        _message_subject(this, message_manager, {}, { message::MessageId::ApplicationUpdate }) {
        auto add_data = MessageListenerAddData({ this, _message_subject.ReceiveMessages() });
        message_manager->Broadcast(message::Message(message::MessageId::MessageListenerAdd, &add_data), this);
    }
    void OnMessageListenerAdd(const Message& message) {
        _ASSERT_EXPR(message.HasData(), std::string(__FILE__) + std::to_string(__LINE__));
        auto add_data = message.GetDataPtr<MessageListenerAddData>();
        _ASSERT_EXPR(!_message_subject.Contain(add_data->listener), std::string(__FILE__) + std::to_string(__LINE__));

        _message_subject.UtilInsert(
            add_data->listener, add_data->recieve_messages, _message_subject.SendMessages());

        //! 自身が通知するメッセージを返信する。
        auto reply_data = MessageListenerAddReplyData({ this, _message_subject.ReceiveMessages() });
        _message_subject.Broadcast(message::Message(message::MessageId::MessageListenerAddReply, &reply_data), this);
        return;
    }
    void OnMessageListenerAddReply(const Message& message) {
        _ASSERT_EXPR(message.HasData(), std::string(__FILE__) + std::to_string(__LINE__));
        auto reply_data = message.GetDataPtr<MessageListenerAddReplyData>();

        _message_subject.UtilInsert(
            reply_data->listener, reply_data->recieve_messages, _message_subject.SendMessages());
        return;
    }
    void OnMessageListenerRemove(const Message& message) {
        _ASSERT_EXPR(message.HasData(), std::string(__FILE__) + std::to_string(__LINE__));
        auto data = message.GetDataPtr<MessageListenerRemoveData>();
        _message_subject.Dettach(data->listener);
        return;
    }
    virtual void Recieve(const Message& message) override {
        switch(message.GetType()) {
            case message::MessageId::MessageListenerAdd:
                this->OnMessageListenerAdd(message);
                break;
            case message::MessageId::MessageListenerAddReply:
                this->OnMessageListenerAddReply(message);
                break;
            case message::MessageId::MessageListenerRemove:
                this->OnMessageListenerRemove(message);
                break;
            default:
                break;
        }
    }

    void Update(void) {
        std::cout << "System" << "Update" << "\n";
        auto data = ApplicationUpdateData();
        _message_subject.Notify(message::Message(message::MessageId::ApplicationUpdate, &data));
    }
};


int main(int, char* []) {
    message::MessageManager message_manager;

    Actor a("A", &message_manager);
    System s(&message_manager);
    Actor b("B", &message_manager);

    s.Update();
    return 0;
}