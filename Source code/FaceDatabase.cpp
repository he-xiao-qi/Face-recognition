#include "FaceDatabase.h"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

FaceDatabase::FaceDatabase() {}
FaceDatabase::~FaceDatabase() {}

void FaceDatabase::add(const std::vector<float>& descriptor, const std::string& label) {
    descriptors.push_back(descriptor);
    labels.push_back(label);
}

void FaceDatabase::clear() {
    descriptors.clear();
    labels.clear();
}

bool FaceDatabase::save(const std::string& file) const {
    std::ofstream ofs(file);
    if (!ofs.is_open()) return false;
    for (size_t i = 0; i < descriptors.size(); ++i) {
        ofs << labels[i];
        for (float v : descriptors[i]) {
            ofs << ' ' << v;
        }
        ofs << '\n';
    }
    ofs.close();
    return true;
}

bool FaceDatabase::load(const std::string& file) {
    clear();
    std::ifstream ifs(file);
    if (!ifs.is_open()) return false;
    std::string line;
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string label;
        iss >> label;
        std::vector<float> desc;
        float v;
        while (iss >> v) desc.push_back(v);
        if (!desc.empty()) {
            labels.push_back(label);
            descriptors.push_back(desc);
        }
    }
    ifs.close();
    return !descriptors.empty();
}

std::vector<std::string> FaceDatabase::get_labels() const {
    return labels;
}

std::vector<std::vector<float>> FaceDatabase::get_descriptors() const {
    return descriptors;
}

bool FaceDatabase::remove(const std::string& name) {
    bool found = false;
    for (size_t i = 0; i < labels.size(); ) {
        if (labels[i] == name) {
            labels.erase(labels.begin() + i);
            descriptors.erase(descriptors.begin() + i);
            found = true;
        } else {
            ++i;
        }
    }
    return found;
} 