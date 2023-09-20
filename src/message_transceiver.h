#pragma once

#include "ChatMessagePubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

using namespace eprosima::fastdds::dds;

class SubListener : public DataReaderListener{
    public:

        SubListener(const char* name) : name_(name){
        }

        void on_data_available(DataReader* reader) override
        {
            SampleInfo info;
            if (reader->take_next_sample(&msg_, &info) == ReturnCode_t::RETCODE_OK){
                if (info.valid_data && name_ != msg_.name()){
                    std::cout << "\r[" << msg_.name() << "]: " << msg_.data() << "\n";
                }
            }
        }

        ChatMessage msg_;
        std::string name_;
};

class MessageTransceiver{

    std::string name_;

    DomainParticipant* participant_;

    Publisher* publisher_;
    Subscriber* subscriber_;

    Topic* topic_;

    DataWriter* writer_;
    DataReader* reader_;
    
    SubListener listener_;

    TypeSupport type_;

    public:

    MessageTransceiver(const char* name);

    ~MessageTransceiver();

    bool publish(const ChatMessage&);

    void run();
};