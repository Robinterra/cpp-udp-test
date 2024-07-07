#include "IMessageRepo.h"
#include "../../Data/Net/UdpControl.h"

class UdpMessageRepo : public IMessageRepo
{
    // Prevent copies and assignments
    UdpMessageRepo(const UdpMessageRepo&) = delete;
    UdpMessageRepo& operator=(const UdpMessageRepo&) = delete;

    private:
    UdpControl m_udpControl;
    sockaddr_in m_listenAddr;
    sockaddr_in m_sendAddr;

    public:

    UdpMessageRepo(sockaddr_in listenAddr, sockaddr_in sendAddr)
    {
        m_listenAddr = listenAddr;
        m_sendAddr = sendAddr;
    }

    ~UdpMessageRepo() {}

    Response<UdpControl::Result> Setup()
    {
        Response<UdpControl::Result> res = m_udpControl.Start((sockaddr*)&m_listenAddr);
        return res;
    }

    Response<int> SendMessage(const char * message, int size) override;

    Response<int> GetMessage(char * message, int bufferSize) override;
};