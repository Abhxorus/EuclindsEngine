#pragma once
#include <string>
#include <algorithm>

/*
 * Simple race countdown that returns "3","2","1","GO!"
 * Does not block Track or ImGui; you decide what systems to gate in BaseApp.
 */
class RaceCountdown {
public:
    RaceCountdown(float seconds = 3.f)
        : total_(seconds), remaining_(seconds), started_(false), finished_(false) {
    }
    ~RaceCountdown() = default;

    void
        start() {
        started_ = true; finished_ = false; remaining_ = total_;
    }
    void stop() {
        started_ = false;
    }
    bool isStarted() const {
        return started_;
    }
    bool isFinished() const {
        return finished_;
    }

    /* Advance and return the display text for this frame */
    std::string tick(float dt) {
        if (!started_ || finished_) return "";
        remaining_ = std::max(0.f, remaining_ - dt);
        if (remaining_ == 0.f) { finished_ = true; started_ = false; }
        int num = static_cast<int>(remaining_ + 0.999f); /* ceil-like display */
        if (finished_) return "GO!";
        return (num >= 1) ? std::to_string(num) : "GO!";
    }

private:
    float total_;
    float remaining_;
    bool  started_;
    bool  finished_;
};