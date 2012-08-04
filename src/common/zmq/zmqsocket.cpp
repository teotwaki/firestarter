/*
 * Copyright (C) 2012  Sebastian Lauwers <sebastian.lauwers@gmail.com>
 *
 * This file is part of Firestarter.
 *
 * Firestarter is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Firestarter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "zmq/zmqsocket.hpp"

namespace firestarter { namespace sockets {
	DECLARE_LOG(logger, "firestarter.sockets");
} }

using namespace firestarter::sockets;

bool ZMQSendingSocket::send() {
	LOG_INFO(logger, "Sending empty message on socket (" << &(this->socket) << ").");
	zmq::message_t message(0);
	bool sent = socket->send(message, 0);
	if (sent) {
		LOG_DEBUG(logger, "Empty message sent succesfully.");
	}
	else {
		LOG_ERROR(logger, "Empty message couldn't be sent.");
	}
	return sent;
}

bool ZMQSendingSocket::send(google::protobuf::Message const & pb_message, bool send_more) {
	LOG_INFO(logger, "Sending message (" << pb_message.GetTypeName() << ") on socket (" << &(this->socket) << ").");

	LOG_DEBUG(logger, "Serialising message.");
	std::string pb_serialised;
	pb_message.SerializeToString(&pb_serialised);

	LOG_DEBUG(logger, "Copying message data to socket (" << &(this->socket) << ").");
	zmq::message_t message(pb_serialised.size());
	memcpy(static_cast<void *>(message.data()), pb_serialised.c_str(), pb_serialised.size());

	int flags = 0;
	if (send_more) {
		LOG_DEBUG(logger, "Added ZMQ_SNDMORE flag to socket.");
		flags = ZMQ_SNDMORE;
	}

	else {
		LOG_DEBUG(logger, "No flags to add.");
	}

	LOG_DEBUG(logger, "Sending message.");
	return this->socket->send(message, flags);
}

bool ZMQReceivingSocket::receive(bool blocking) {
	LOG_INFO(logger, "Listening for an empty message on socket (" << &(this->socket) << ").");
	zmq::message_t message;

	int flags = 0;
	if (not blocking) {
		LOG_DEBUG(logger, "Added ZMQ_DONTWAIT flag to socket.");
		flags = ZMQ_DONTWAIT;
	}

	else {
		LOG_DEBUG(logger, "No flags to add.");
	}

	return this->socket->recv(&message, flags);
}

bool ZMQReceivingSocket::receive(google::protobuf::Message & pb_message, bool blocking) {
	LOG_INFO(logger, "Listening for a " << pb_message.GetTypeName() << " on socket (" << &(this->socket) << ").");
	zmq::message_t message;

	int flags = 0;
	if (not blocking) {
		LOG_DEBUG(logger, "Added ZMQ_DONTWAIT flag to socket.");
		flags = ZMQ_DONTWAIT;
	}

	else {
		LOG_DEBUG(logger, "No flags to add.");
	}

	if (this->socket->recv(&message, flags))
		if (pb_message.ParseFromArray(message.data(), message.size()))
			return pb_message.IsInitialized();

	return false;
}

