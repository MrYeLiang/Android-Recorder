//
// Created by 叶亮 on 2019/7/15.
//

#ifndef ANDROIDRECORDER_HANDLER_H
#define ANDROIDRECORDER_HANDLER_H

#include "message_queue.h"
class Message;
class MessageQueue;

class Handler{
private:
    MessageQueue* mQueue;

public:
    Handler(MessageQueue* mQueue);
    ~Handler();

    int postMessage(Message* msg);
    int getQueueSize();
    virtual void handleMessage(Message* msg){};

};

#endif //ANDROIDRECORDER_HANDLER_H
