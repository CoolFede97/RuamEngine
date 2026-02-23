#pragma once

#include <typeindex>
#include <functional>
#include <queue>
#include <memory>
#include <unordered_map>

#include "../Input/KeyCode.h"
#include "../Input/Cursor.h"

#include "Vec2.h"

namespace RuamEngine
{
    struct Event {
        virtual ~Event() = default;
    };

    struct OnKeyPressEvent : public Event {
        int key;

        OnKeyPressEvent(int key) : key(key) {}
    };

    struct OnKeyReleaseEvent : public Event {
        int key;

        OnKeyReleaseEvent(int key) : key(key) {}
    };

    struct OnMouseMoveEvent : public Event {
        Vec2 positionPix;
        Vec2 positionNorm;

        OnMouseMoveEvent(Vec2 posPix, Vec2 posNorm) : positionPix(posPix), positionNorm(posNorm) {}
    };

    struct OnMouseButtonDownEvent : public Event {
        Vec2 positionPix;
        Vec2 positionNorm;
        MouseCode button;

        OnMouseButtonDownEvent(Vec2 posPix, Vec2 posNorm, MouseCode button) : positionPix(posPix), positionNorm(posNorm), button(button) {}
    };

    struct OnMouseButtonUpEvent : public Event {
        Vec2 positionPix;
        Vec2 positionNorm;
        MouseCode button;

        OnMouseButtonUpEvent(Vec2 posPix, Vec2 posNorm, MouseCode button) : positionPix(posPix), positionNorm(posNorm), button(button) {}
    };

    struct OnMouseScrollEvent : public Event {
        Vec2 offset;
        Vec2 positionPix;
        Vec2 positionNorm;

        OnMouseScrollEvent(Vec2 offset, Vec2 posPix, Vec2 posNorm) : offset(offset), positionPix(posPix), positionNorm(posNorm) {}
    };

    struct OnMouseEnterWindowEvent : public Event {
        Vec2 positionPix;
        Vec2 positionNorm;

        OnMouseEnterWindowEvent(Vec2 posPix, Vec2 posNorm) : positionPix(posPix), positionNorm(posNorm) {}
    };

    struct OnMouseLeaveWindowEvent : public Event {
        Vec2 positionPix;
        Vec2 positionNorm;

        OnMouseLeaveWindowEvent(Vec2 posPix, Vec2 posNorm) : positionPix(posPix), positionNorm(posNorm) {}
    };

    struct OnCharEvent : public Event {
        char32_t unicodeChar;

        OnCharEvent(unsigned int cp) : unicodeChar((char32_t)cp) {}
    };

    class EventManager {
    public:

        template <typename EventType>
        static void InstantSubscribe(std::function<void(const EventType&)> listener) {
            auto& listeners = instantSubscribers[typeid(EventType)];
            instantSubscribers[typeid(EventType)].push_back(
                [listener](const Event& e) {
                    listener(static_cast<const EventType&>(e));
                }
            );
        }

        template <typename EventType>
        static void Subscribe(std::function<void(const EventType&)> listener) {
            subscribers[typeid(EventType)].push_back(
                [listener](const Event& e) {
                    listener(static_cast<const EventType&>(e));
                }
            );
        }


        template <typename EventType>
        static void InstantPublish(const EventType& event) {
            auto thisTypeEvents = instantSubscribers.find(typeid(EventType));
            if (thisTypeEvents != instantSubscribers.end()) {
                for (auto& listener : thisTypeEvents->second) {
                    listener(event);
                }
            }
        }

        template <typename EventType>
        static void Publish(const EventType& event) {
            eventQueue.push(std::make_unique<EventType>(event));
        }

        static void HandleEvents() {
            while (!eventQueue.empty()) {
                auto& eventPtr = eventQueue.front();
                auto thisTypeEvents = subscribers.find(typeid(*eventPtr));
                if (thisTypeEvents != subscribers.end()) {
                    for (auto& listener : thisTypeEvents->second) {
                        listener(*eventPtr);
                    }
                }
                eventQueue.pop();
            }
        }

        static void OnKeyPress(KeyCode key, std::function<void(const OnKeyPressEvent&)> callback);

    private:
        static std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> instantSubscribers;
        static std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> subscribers;
        static std::queue<std::unique_ptr<Event>> eventQueue;
    };
}
