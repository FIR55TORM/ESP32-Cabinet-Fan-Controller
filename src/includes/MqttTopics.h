#include <map>
#include "includes/MqttTopicsEnum.h"

class MqttTopics
{
private:
    std::map<std::string, MqttTopicsEnum> _topics;

public:
    // Subscriptions
    static constexpr const char *speedTopic = "cabfan/speed";
    static constexpr const char *autoModeTopic = "cabfan/autoMode";
    static constexpr const char *targetTemperatureTopic = "cabfan/targetTemperature";

    // Publications
    static constexpr const char *temperatureHumidityTopic = "cabfan/temperatureHumidity";
    static constexpr const char *fanTachoTopic = "cabfan/fanTacho";

    MqttTopics()
    {
        _topics.insert(std::pair<std::string, MqttTopicsEnum>(speedTopic, MqttTopicsEnum::speed));
        _topics.insert(std::pair<std::string, MqttTopicsEnum>(autoModeTopic, MqttTopicsEnum::autoMode));
        _topics.insert(std::pair<std::string, MqttTopicsEnum>(targetTemperatureTopic, MqttTopicsEnum::targetTemperature));
    };

    MqttTopicsEnum ContainsTopic(char *topic)
    {
        auto el = _topics.find(topic);
        if (el != _topics.end())
        {
            return el->second;
        }
        else
        {
            return MqttTopicsEnum::topicNotFound;
        }
    };
};