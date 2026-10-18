/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** SignalHandler class
*/

#pragma once

#include <csignal>
#include <atomic>


namespace RType::Utils {
    template <int S>
    class SignalHandler {
        static constexpr int SIGNAL = S;
        static inline std::atomic<bool> SIGNAL_ACTIVE;

        public:
            SignalHandler() = delete;

            /**
             * Initializes the signal handler.
             *
             * This also resets the current state
             * of all signal handlers of the same
             * signal.
             */
            static void initialize()
            {
                SIGNAL_ACTIVE.store(false);
                std::signal(SIGNAL, handleSignal);
            }

            static inline bool isActive() noexcept { return SIGNAL_ACTIVE.load(); }
            static inline void reset() noexcept { SIGNAL_ACTIVE.store(false); }

        private:
            static void handleSignal(int)
            {
                SIGNAL_ACTIVE.store(true);
            }
    };

    using InterruptHandler = SignalHandler<SIGINT>;
}
