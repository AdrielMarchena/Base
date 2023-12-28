#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <queue>

#include "uuid/UUID.h"

namespace Base {

	class Message
	{
	public:
		Message(const std::string message);

		std::string getEvent() const;
	private:
		std::string m_MessageEvent;
	};

	class MessageBus
	{
	public:
		MessageBus() = default;
		~MessageBus() = default;

		UUID addReceiver(std::function<void(Message)> messageReceiver);
		void removeReceiver(UUID messageReceiverId);

		void sendMessage(Message message);

		void notify();

	private:
		std::unordered_map<UUID, std::function<void(Message)>> m_Receivers;
		std::queue<Message> m_Messages;
	};

	class GlobalMessenger
	{
	public:
		GlobalMessenger() = delete;

		static bool ChannelExists(const std::string& channel);

		/* returns false if channel already exists */
		static bool AddChannel(const std::string& channel);
		static void RemoveChannel(const std::string& channel);

		/* return false if channel dos not exists */
		static bool NotifyChannel(const std::string& channel);

		static bool NotifyAllChannel();

		/* return false if channel dos not exists */
		static bool SendMessageC(const std::string& channel, Message message);

		[[nodiscard("You need de UUID to remove the receiver if you plan on doing that")]]
		static UUID ListenChannel(const std::string& channel, std::function<void(Message)> messageReceiver);
		static void StopListenChannel(const std::string& channel, UUID messageReceiverId);
	};
}