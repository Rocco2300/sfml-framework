#pragma once

#include <vector>
#include <unordered_map>

template <typename Identifier, typename Callback>
class EventBroker {
private:
    std::unordered_map<Identifier, std::vector<Callback>> m_subscribers;

public:
    void subscribe(Identifier id, Callback callback) {
        if (m_subscribers.contains(id)) {
            m_subscribers[id].push_back(callback);
            return;
        }

        std::vector<Callback> callbacks;
        callbacks.push_back(callback);
        m_subscribers.emplace(id, callbacks);
    }

    template <typename ...Args>
    void notify(Identifier id, Args ...args) {
        for (const auto& callback : m_subscribers[id]) {
            callback(args...);
        }
    }
};