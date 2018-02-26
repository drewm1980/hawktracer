#ifndef HAWKTRACER_CLIENT_TCP_CLIENT_STREAM_HPP
#define HAWKTRACER_CLIENT_TCP_CLIENT_STREAM_HPP

#include "parser/stream.hpp"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

namespace HawkTracer
{
namespace client
{

class TCPClientStream : public parser::Stream
{
public:
    TCPClientStream(const std::string& ip_address, uint16_t port, bool wait_for_server = true);
    ~TCPClientStream();

    bool start() override;
    void stop() override;

    bool is_connected() const;

    int read_byte() override;
    bool read_data(char* buff, size_t size) override;

private:
    void _run();

    bool _wait_for_data(std::unique_lock<std::mutex>& l);
    void _pop_if_used() { if (_datas.front().second.size() == _datas.front().first) { _datas.pop(); } }

    std::queue<std::pair<size_t, std::vector<char>>> _datas;
    std::mutex _datas_mtx;
    std::condition_variable _datas_cv;
    std::thread _thread;

    std::atomic<int> _sock_fd = -1;

    std::string _ip_address;
    uint16_t _port;

    bool _wait_for_server;
};

} // namespace client
} // namespace HawkTracer

#endif // HAWKTRACER_CLIENT_TCP_CLIENT_STREAM_HPP
