#include "pch.h"
#include "GlobalMessenger.h"

namespace Base {
	static std::unordered_map<std::string, MessageBus> s_Channels;

	Message::Message(const std::string message)
		:m_MessageEvent(message)
	{
	}

	std::string Message::getEvent() const
	{
		return m_MessageEvent;
	}

	UUID MessageBus::addReceiver(std::function<void(Message)> messageReceiver)
	{
		UUID id = UUID();
		m_Receivers[id] = messageReceiver;
		return id;
	}

	void MessageBus::removeReceiver(UUID messageReceiverId)
	{
		if (m_Receivers.find(messageReceiverId) == m_Receivers.end())
			return;

		m_Receivers.erase(messageReceiverId);
	}

	void MessageBus::sendMessage(Message message)
	{
		m_Messages.push(message);
	}

	void MessageBus::notify()
	{
		while (!m_Messages.empty())
		{
			for (auto& receiver : m_Receivers)
			{
				receiver.second(m_Messages.front());
			}

			m_Messages.pop();
		}
	}

	bool GlobalMessenger::ChannelExists(const std::string& channel)
	{
		return s_Channels.find(channel) != s_Channels.end();
	}

	bool GlobalMessenger::AddChannel(const std::string& channel)
	{
		if (ChannelExists(channel)) return false;

		s_Channels[channel] = MessageBus();

		return true;
	}

	void GlobalMessenger::RemoveChannel(const std::string& channel)
	{
		if (!ChannelExists(channel)) return;
		s_Channels.erase(channel);
	}

	bool GlobalMessenger::NotifyChannel(const std::string& channel)
	{
		if (!ChannelExists(channel)) return false;

		s_Channels[channel].notify();

		return true;
	}

	bool GlobalMessenger::NotifyAllChannel()
	{
		for (auto& channel : s_Channels)
		{
			channel.second.notify();
		}
		return false;
	}

	bool GlobalMessenger::SendMessageC(const std::string& channel, Message message)
	{
		if (!ChannelExists(channel)) return false;

		s_Channels[channel].sendMessage(message);

		return true;
	}

	UUID GlobalMessenger::ListenChannel(const std::string& channel, std::function<void(Message)> messageReceiver)
	{
		if (!ChannelExists(channel)) return 0;

		return s_Channels[channel].addReceiver(messageReceiver);
	}

	void GlobalMessenger::StopListenChannel(const std::string& channel, UUID messageReceiverId)
	{
		if (!ChannelExists(channel)) return;

		s_Channels[channel].removeReceiver(messageReceiverId);
	}

}

