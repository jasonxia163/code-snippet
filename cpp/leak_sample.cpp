#include <iostream>
#include <memory>
#include <vector>

class Segment {
 public:
  Segment() { std::cout << "Segment default constructed" << std::endl; }
  Segment(int id, int start, int end) : id_(id), start_(start), end_(end) {
    std::cout << "Segment " << id_ << " constructed" << std::endl;
  }
  ~Segment() { std::cout << "Segment " << id_ << " destructed" << std::endl; }

 private:
  int id_;
  int start_;
  int end_;
};

class Frame {
 public:
  Frame() { std::cout << "Frame default constructed" << std::endl; }
  Frame(int id, int start, int end) : id_(id), start_(start), end_(end) {
    std::cout << "Frame " << id_ << " constructed" << std::endl;
  }
  ~Frame() { std::cout << "Frame " << id_ << " destructed" << std::endl; }
  void AddSegment(std::shared_ptr<Segment> segment) {
    segments_.push_back(segment);
  }

 private:
  int id_;
  int start_;
  int end_;

  std::vector<std::shared_ptr<Segment>> segments_;
};

int main() {
  Frame frame;
  for (int i = 0; i < 10; i++) {
    std::shared_ptr<Segment> segment =
        std::make_shared<Segment>(i, i * 100, (i + 1) * 100);
    frame.AddSegment(segment);
  }

  return 0;
}
