#ifndef Statsd_h
#define Statsd_h

#include <Arduino.h>
#include <IPAddress.h>
#include <Udp.h>


class Statsd {
public:
    Statsd(UDP& udp, String host, uint16_t port);
    Statsd(UDP& udp, String host, uint16_t port, String tags);
    int begin();
    void test();
    void count(String metric, int value, String tags, float sample_rate);
    void gauge(String metric, int value, String tags, float sample_rate);
    void timing(String metric, int value, String tags, float sample_rate);
    void increment(String metric, String tags, float sample_rate);
    void decrement(String metric, String tags, float sample_rate);
private:
    String formatTags(String tags);
    bool shouldSend(float sample_rate);
    void send(String metric, int value, String tags, const char type, float sample_rate);
    UDP &_udp;
    String _host;
    uint16_t _port;
    String _tags;
};

#endif
