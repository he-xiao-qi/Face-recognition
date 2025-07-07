#ifndef FACEDATABASE_H
#define FACEDATABASE_H

#include <vector>
#include <string>

class FaceDatabase {
public:
    FaceDatabase();
    ~FaceDatabase();
    void add(const std::vector<float>& descriptor, const std::string& label);
    void clear();
    bool save(const std::string& file) const;
    bool load(const std::string& file);
    std::vector<std::string> get_labels() const;
    std::vector<std::vector<float>> get_descriptors() const;
    bool remove(const std::string& name);
private:
    std::vector<std::vector<float>> descriptors;
    std::vector<std::string> labels;
};

#endif // FACEDATABASE_H 