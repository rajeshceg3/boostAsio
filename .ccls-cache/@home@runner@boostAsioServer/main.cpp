#include <boost/asio.hpp>
#include <iostream>

int main() {
  boost::asio::io_context io_context;

  // Server setup
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080);
  boost::asio::ip::tcp::acceptor acceptor(io_context, endpoint);

  std::cout << "Server listening on port 8080..." << std::endl;

  // Handle incoming client connections
  while (true) {
    boost::asio::ip::tcp::socket socket(io_context);
    acceptor.accept(socket);

    std::cout << "Client connected!" << std::endl;

    // Handle client communication
    char data[1024];
    boost::system::error_code error;
    size_t length = socket.read_some(boost::asio::buffer(data), error);

    if (!error && length > 0) {
      std::string message(data, length);
      std::cout << "Received: " << message << std::endl;

      // Send the echo response to the client
      std::string response = "Echo: " + message;
      boost::asio::write(socket, boost::asio::buffer(response));
    } else if (error != boost::asio::error::eof) {
      std::cerr << "Error: " << error.message() << std::endl;
    }

    socket.close();
  }

  return 0;
}