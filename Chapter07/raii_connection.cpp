#include <iostream>
#include <gtest/gtest.h>

class Connection {
public:
  Connection(const std::string& /*url*/) {
    std::cout << "Connection constructed" << '\n';
  }
  ~Connection() {
    std::cout << "Connection destructed" << '\n';
  }
  //...
};

auto close(Connection& /*connection*/) {
  //...
}

auto open_connection(const std::string& url) {
  return Connection{url};
}


auto send_request(Connection& /*connection*/, const std::string& /*message*/) {
  // ...
}

class RAIIConnection {
public:
  RAIIConnection(const std::string& url)
    : connection_{open_connection(url)} {}
  ~RAIIConnection() {
    try {
      close(connection_);
    }
    catch (const std::exception&) {
      // Handle error, but never throw from a destructor
    }
  }

  auto& get() {
    return connection_;
  }

private:
  Connection connection_;
};

auto send(const std::string& message) {
  auto connection = RAIIConnection("http://www.example.com/");
  send_request(connection.get(), message);
  // No need to close the connection, it is implicitly handled
  // by the RAIIConnection destructor
}

TEST(RaiiConnection, SendMessage) {
  send("a message");
  send("another message");
}
