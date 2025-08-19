#include <iostream>
#include <memory>

class MyObserver {
 public:
  MyObserver() {
    std::cout << "MyObserver constructor" << std::endl;
  }

  ~MyObserver() {
    std::cout << "MyObserver destructor" << std::endl;
  }
};

class MyServer {
 public:
  MyServer() {
    std::cout << "MyServer constructor" << std::endl;
  }

  ~MyServer() {
    std::cout << "MyServer destructor" << std::endl;
  }

  void addObserver(const std::shared_ptr<MyObserver>& observer) {
    auto old_observer = observer_.lock();
    if (old_observer) {
      std::cout << "old_observer is not nullptr" << std::endl;
    } else {
      std::cout << "old_observer is nullptr" << std::endl;
    }
    observer_ = observer;
    std::cout << "observer_ is: " << observer_.lock().get() << std::endl;
  }
 private:
  std::weak_ptr<MyObserver> observer_;
};

int main() {
  std::shared_ptr<MyObserver> observer = std::make_shared<MyObserver>();
  std::shared_ptr<MyServer> server = std::make_shared<MyServer>();
  server->addObserver(observer);
  server->addObserver(nullptr);
  return 0;
}