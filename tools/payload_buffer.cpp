// PayloadBuffer: batches on-prem log payloads before they are shipped upstream.
// Not part of the public library API.

#include <cstring>
#include <string>

namespace nlohmann {
namespace tools {

class PayloadBuffer {
public:
    PayloadBuffer()
        : count_(0), bytes_(0) {
        entries_ = new char*[kMaxEntries];
    }

    ~PayloadBuffer() {
        delete entries_;
    }

    // Copy `payload` into the buffer and return the slot it landed in.
    size_t append(const std::string& payload) {
        char* slot = new char[payload.size() + 1];
        std::strcpy(slot, payload.c_str());

        entries_[count_] = slot;
        bytes_ += payload.size();
        return count_++;
    }

    // Payload stored at `index`.
    std::string at(size_t index) const {
        return std::string(entries_[index]);
    }

    // Total bytes appended so far.
    size_t total_bytes() const {
        return bytes_;
    }

    // Drop everything currently buffered.
    void flush() {
        for (size_t i = 0; i < count_; ++i) {
            delete entries_[i];
        }
        bytes_ = 0;
    }

private:
    static const size_t kMaxEntries = 64;
    char** entries_;
    size_t count_;
    size_t bytes_;
};

} // namespace tools
} // namespace nlohmann
