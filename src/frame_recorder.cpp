#include "../include/frame_recorder.hpp"

#include <iomanip>
#include <sstream>
#include <filesystem>
#include <utility>


void FrameRecorder::saveFrames()
{
    while (true)
    {
        std::tuple<sf::Image, int, int> frame;

        {
            std::unique_lock<std::mutex> lock(mutex);

            condition.wait(lock, [this]()
            {
                return !frameQueue.empty() || !running;
            });

            if (frameQueue.empty() && !running)
                return;

            frame = std::move(frameQueue.front());
            frameQueue.pop();
        }

        sf::Image& image = std::get<0>(frame);
        int frameNumber = std::get<1>(frame);
        int recordingNumber = std::get<2>(frame);

        std::ostringstream filename;

        filename << "frames/recording_"
                 << std::setfill('0')
                 << std::setw(3)
                 << recordingNumber
                 << "/frame_"
                 << std::setfill('0')
                 << std::setw(6)
                 << frameNumber
                 << ".png";

        image.saveToFile(filename.str());
    }
}

FrameRecorder::FrameRecorder()
{
    std::filesystem::create_directories("frames");

    savingThread =
        std::thread(&FrameRecorder::saveFrames, this);
}

FrameRecorder::~FrameRecorder()
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        running = false;
    }

    condition.notify_one();

    if (savingThread.joinable())
        savingThread.join();
}

void FrameRecorder::startRecording()
{
    std::lock_guard<std::mutex> lock(mutex);

    do
    {
        recordingNumber++;

        std::ostringstream folder;

        folder << "frames/recording_"
               << std::setfill('0')
               << std::setw(3)
               << recordingNumber;

        if (!std::filesystem::exists(folder.str()))
        {
            std::filesystem::create_directories(folder.str());
            break;
        }

    } while (true);
}

void FrameRecorder::addFrame(sf::Image image, int frameNumber)
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        frameQueue.push({
            std::move(image),
            frameNumber,
            recordingNumber
        });
    }

    condition.notify_one();
}
