//
// Copyright (C) 2005 Andras Varga
//
// Based on the video streaming app of the similar name by Johnny Lai.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/applications/udpapp/UdpVideoStreamClient.h"

#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"

namespace inet {

Define_Module(UdpVideoStreamClient);

void UdpVideoStreamClient::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        selfMsg = new cMessage("UDPVideoStreamStart");
    }
}

void UdpVideoStreamClient::finish()
{
    ApplicationBase::finish();
}

void UdpVideoStreamClient::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        requestStream();
    }
    else
        socket.processMessage(msg);
}

void UdpVideoStreamClient::socketDataArrived(UdpSocket* socket, Packet *msg)
{
    // process incoming packet
    receiveStream(msg);
}

void UdpVideoStreamClient::socketErrorArrived(UdpSocket* socket, cMessage *msg)
{
    EV_WARN << "Ignoring UDP error report " << msg->getName() << endl;
    delete msg;
}

void UdpVideoStreamClient::requestStream()
{
    int svrPort = par("serverPort");
    int localPort = par("localPort");
    const char *address = par("serverAddress");
    L3Address svrAddr = L3AddressResolver().resolve(address);

    if (svrAddr.isUnspecified()) {
        EV_ERROR << "Server address is unspecified, skip sending video stream request\n";
        return;
    }

    EV_INFO << "Requesting video stream from " << svrAddr << ":" << svrPort << "\n";

    socket.setOutputGate(gate("socketOut"));
    socket.bind(localPort);
    socket.setCallbackObject(this);

    Packet *pk = new Packet("VideoStrmReq");
    const auto& payload = makeShared<ByteCountChunk>(B(1));    //FIXME set packet length
    pk->insertAtBack(payload);
    socket.sendTo(pk, svrAddr, svrPort);
}

void UdpVideoStreamClient::receiveStream(Packet *pk)
{
    EV_INFO << "Video stream packet: " << UdpSocket::getReceivedPacketInfo(pk) << endl;
    emit(packetReceivedSignal, pk);
    delete pk;
}

bool UdpVideoStreamClient::handleNodeStart(IDoneCallback *doneCallback)
{
    simtime_t startTimePar = par("startTime");
    simtime_t startTime = std::max(startTimePar, simTime());
    scheduleAt(startTime, selfMsg);
    return true;
}

bool UdpVideoStreamClient::handleNodeShutdown(IDoneCallback *doneCallback)
{
    cancelEvent(selfMsg);
    //TODO if(socket.isOpened()) socket.close();
    return true;
}

void UdpVideoStreamClient::handleNodeCrash()
{
    cancelEvent(selfMsg);
}

} // namespace inet

