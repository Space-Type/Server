/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ThreadQueue class
*/

#pragma once

#include <optional>
#include <vector>
#include <mutex>


namespace RType::Threads {
    /**
     * Note:
     * This queue is not ordered.
     */
    template <typename T>
    class ThreadQueue {
        std::vector<T> m_Queue;
        std::mutex m_QueueMutex;

        public:
            ThreadQueue() noexcept
                : m_Queue()
            {
            }

            inline void push(const T& value) { push(T(value)); }
            void push(T&& value)
            {
                std::unique_lock lock(m_QueueMutex);
                m_Queue.push_back(value);
            }

            std::optional<T> pop()
            {
                std::unique_lock lock(m_QueueMutex);
                if (m_Queue.empty())
                    return {};

                std::optional<T> value { std::move(m_Queue.front()) };

                // swap & pop
                m_Queue.front() = std::move(m_Queue.back());
                m_Queue.pop_back();

                return value;
            }
    };
}
