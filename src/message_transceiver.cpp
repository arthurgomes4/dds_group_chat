#include "message_transceiver.h"

static void exitIfNull(void* ptr){
    if(ptr == nullptr){
        std::cout << "object pointer is nullptr\n";
        exit(1);
    }
}

MessageTransceiver::MessageTransceiver(const char* name) 
    : participant_(nullptr)
    , publisher_(nullptr)
    , topic_(nullptr)
    , writer_(nullptr)
    , type_(new ChatMessagePubSubType())
    , listener_(name){

        name_ = std::string(name);

        DomainParticipantQos qos;
        qos.name("transceiver");

        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, qos);
        exitIfNull(participant_);

        type_.register_type(participant_);

        topic_ = participant_->create_topic("chat", "ChatMessage", TOPIC_QOS_DEFAULT);
        exitIfNull(topic_);

        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT);
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT);
        exitIfNull(publisher_);
        exitIfNull(subscriber_);;

        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT);
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);
        exitIfNull(writer_);
        exitIfNull(reader_);
}


MessageTransceiver::~MessageTransceiver(){
    if(writer_ != nullptr)
        publisher_->delete_datawriter(writer_);
    
    if(publisher_ != nullptr)
        participant_->delete_publisher(publisher_);

    if(topic_ != nullptr)
        participant_->delete_topic(topic_);

    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

void MessageTransceiver::run(){

    std::string input;
    ChatMessage msg;
    msg.name(name_);

    while (true)
    {
        std::getline(std::cin, input);
        msg.data(input);
        publish(msg);
    }
    
}

bool MessageTransceiver::publish(const ChatMessage& msg){
    return writer_->write((void*)&msg);
}
