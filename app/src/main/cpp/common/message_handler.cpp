//
// Created by 叶亮 on 2019/7/15.
//
#include "message_handler.h"

Handler::Handler(MessageQueue *queue) {
    this->mQueue = queue;
}

Handler::~Handler() {

}

int Handler::postMessage(Message *msg) {
    msg->handler = this;
    return mQueue->enqueueMessage(msg);
}

int Handler::getQueueSize() {
    return mQueue->size();
}