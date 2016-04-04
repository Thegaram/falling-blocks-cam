#ifndef SHAREDSTATE_H_DEFINED
#define SHAREDSTATE_H_DEFINED

#include <atomic>
#include <mutex>
#include <queue>

enum class Command {
    SHOOT,
    NOOP
};

// singleton class for storing concurrently accessed state
class SharedState
{
private:
    std::atomic<double> headPositionX { 0.5 }; // 0..1
    std::atomic<double> headPositionY { 0.5 }; // 0..1

    std::atomic_bool gameOver { false };

    std::queue<Command> commandQueue;
    std::mutex queueMutex;

    SharedState() { }

public:
    static SharedState& getInstance();

    SharedState(const SharedState&) = delete;
    SharedState& operator=(const SharedState&) = delete;

    void setHeadPositionX(double);
    double getHeadPositionX() const;

    void setHeadPositionY(double);
    double getHeadPositionY() const;

    void setGameOver()
    {
        gameOver = true;
    }

    bool isGameOver() const
    {
        return gameOver;
    }

    void pushCommand(Command command)
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        commandQueue.push(command);
    }

    Command getCommand()
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        Command result;

        if (commandQueue.empty())
        {
            result = Command::NOOP;
        }
        else
        {
            result = commandQueue.front();
            commandQueue.pop();
        }

        return result;
    }
};

#endif // SHAREDSTATE_H_DEFINED