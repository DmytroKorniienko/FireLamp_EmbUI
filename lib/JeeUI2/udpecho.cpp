#include "JeeUI2.h"
#include <WiFiUdp.h>

WiFiUDP Udp;
bool udpApply = false;

void jeeui2::udp(){
    getAPmac();
    strncpy(udpMessage,mc,sizeof(udpMessage)-1); // udpMessage = mc;
}

void jeeui2::udp(const String &message){
    strncpy(udpMessage,message.c_str(),sizeof(udpMessage)-1); //udpMessage = message;
}

void jeeui2::udpBegin(){
    Udp.begin(localUdpPort);
    if(*udpMessage) udpApply = true;
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
        strncpy(udpRemoteIP,Udp.remoteIP().toString().c_str(),sizeof(udpRemoteIP)-1); // udpRemoteIP = Udp.remoteIP().toString();
        if(dbg)Serial.printf(PSTR("Received %d bytes from %s, port %d\n"), packetSize, udpRemoteIP, Udp.remotePort());
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