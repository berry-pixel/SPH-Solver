#pragma once

#include <SFML/Graphics.hpp>

#include <queue>
#include <tuple>
#include <mutex>
#include <condition_variable>
#include <thread>

class FrameRecorder
{
private:
    std::queue<std::tuple<sf::Image, int, int>> frameQueue;

    std::mutex mutex;
    std::condition_variable condition;

    std::thread savingThread;

    bool running = true;
    int recordingNumber = 0;

    void saveFrames();

public:
    FrameRecorder();
    ~FrameRecorder();

    void startRecording();

    void addFrame(sf::Image image, int frameNumber);
};
