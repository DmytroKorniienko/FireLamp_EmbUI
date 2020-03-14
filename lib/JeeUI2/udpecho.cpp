#include "JeeUI2.h"
#include <WiFiUdp.h>

WiFiUDP Udp;
bool udpApply = false;

void jeeui2::udp(){
    getAPmac();
    udpMessage = mc;
}

void jeeui2::udp(const String &message){
    udpMessage = message;
}

void jeeui2::udpBegin(){
    Udp.begin(localUdpPort);
    if(udpMessage != F("")) udpApply = true;
}

void jeeui2::udpLoop(){
    static bool st = false;
    if(!st){
        st = true;
        udpBegin();
    }

    if(!udpApply) return;

    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        udpRemoteIP = Udp.remoteIP().toString();
        if(dbg)Serial.printf(PSTR("Received %d bytes from %s, port %d\n"), packetSize, udpRemoteIP.c_str(), Udp.remotePort());
        int len = Udp.read(incomingPacket, 64);
        if (len > 0)
        {
        incomingPacket[len] = 0;
        }
        if(dbg)Serial.printf(PSTR("UDP packet contents: %s\n"), incomingPacket);

        // send back a reply, to the IP address and port we got the packet from
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        //Udp.write(ip.c_str());
        if(dbg)Serial.println(String(F("Send UDP: ")) + udpMessage);
        Udp.print(udpMessage);
        Udp.endPacket();
    }
}