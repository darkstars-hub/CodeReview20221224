#ifndef MESSAGE_MESSAGE_ID_H
#define MESSAGE_MESSAGE_ID_H


namespace message {
enum class MessageId {
    MessageListenerAdd,
    MessageListenerAddReply,
    MessageListenerRemove,

    ApplicationUpdate,
    Max,
};
}
#endif // !MESSAGE_MESSAGE_ID_H