#ifndef SHAREDSTATE_H_DEFINED
#define SHAREDSTATE_H_DEFINED

#include <atomic>

// singleton class for storing concurrently accessed state
class SharedState
{
private:
    std::atomic<double> headPositionX { 0.5 }; // 0..1
    std::atomic<double> headPositionY { 0.5 }; // 0..1

    SharedState() { }

public:
    static SharedState& getInstance();

    SharedState(const SharedState&) = delete;
    SharedState& operator=(const SharedState&) = delete;

    void setHeadPositionX(double);
    double getHeadPositionX() const;

    void setHeadPositionY(double);
    double getHeadPositionY() const;
};

#endif // SHAREDSTATE_H_DEFINED