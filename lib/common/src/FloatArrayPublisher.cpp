#include "FloatArrayPublisher.h"

FloatArrayPublisher::FloatArrayPublisher(
    MicroRosPublisher *publisher,
    size_t length
)
{
    this->publisher = publisher;
    msg = createFloatArrayMessage(length);
    this->length = length;
}

void FloatArrayPublisher::publish(float *data_array)
{
    for (size_t i = 0; i < this->length; i++)
    {
        msg->data.data[i] = data_array[i];
    }
    publisher->publish(msg);
}

FloatArrayPublisher::~FloatArrayPublisher() {
    if (msg) {
        free(msg->data.data);
        delete msg;
    }
    delete publisher;
}