#include <sstream>
#include "misc_util.h"

std::mutex timer::l;

timer::timer(std::string label_) :
        begin(std::chrono::steady_clock::now()),
        label(std::move(label_)),
        done(false) {}


void timer::stop() {
    destroy();
}

void timer::kill() {
    destroy(false);
}

timer::~timer() {
    destroy();
}

std::string timer::toReadable(long ns) {
    std::string suffixes[]{"ns", "µs", "ms", "s"};

    auto exp = std::log10(ns);
    exp = std::min(9.0, exp - std::fmod(exp, 3));
    auto elapsed = static_cast<double>(ns) / pow(10, exp);

    auto suffixIdx = static_cast<int>(std::min(3.0, exp / 3));
    auto suffix = suffixes[suffixIdx];

    std::ostringstream retStream;
    retStream << std::fixed << std::setprecision(3) << elapsed << suffix;
    return retStream.str();
}

void timer::destroy(bool print) {
    if (!done) {
        if (print) {
            auto end = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
            std::lock_guard<std::mutex> lock(l);
            std::cout << label << ": " << toReadable(elapsed) << std::endl;
        }
        done = true;
    }
}