#include "Statsd.h"

Statsd::Statsd(UDP &udp, String host, uint16_t port):
    _udp(udp), _host(host), _port(port), _tags(String("")) {
}

Statsd::Statsd(UDP &udp, String host, uint16_t port, String tags):
    _udp(udp), _host(host), _port(port), _tags(formatTags(tags)) {
}

int Statsd::begin() {
    return _udp.beginPacket(_host.c_str(), _port);
}

String Statsd::formatTags(String tags) {
    if(tags.length() == 0) {
        return tags;
    }
    return String(",") + tags;
}

void Statsd::test() {
    _udp.write((uint8_t *)"hello", 6);
    _udp.endPacket();
}

bool Statsd::shouldSend(float sample_rate) {
    if(sample_rate >= 1.0)
        return true;

    return random(100) < (sample_rate * 100);
}

void Statsd::send(String metric, int value, String tags, const char type, float sample_rate) {
    if(!shouldSend(sample_rate))
        return;
    String msg = metric + _tags + formatTags(tags) + ':' + value + '|' + type;
    const char *c_msg = msg.c_str();
    _udp.write((uint8_t *)c_msg, strlen(c_msg));
    _udp.endPacket();
}

void Statsd::count(String metric, int value, String tags, float sample_rate) {
    send(metric, value, tags, 'c', sample_rate);
}

void Statsd::gauge(String metric, int value, String tags, float sample_rate) {
    send(metric, value, tags, 'g', sample_rate);
}

void Statsd::timing(String metric, int value, String tags, float sample_rate) {
    send(metric, value, tags, 'ms', sample_rate);
}

void Statsd::increment(String metric, String tags, float sample_rate) {
    count(metric, 1, tags, sample_rate);
}

void Statsd::decrement(String metric, String tags, float sample_rate) {
    count(metric, -1, tags, sample_rate);
}