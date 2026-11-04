#include <boost/asio.hpp>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
 try {
  unsigned long port = 8080;
  if (argc > 2) throw std::invalid_argument("Usage: main [port 0-65535]");
  if (argc == 2) {
    const std::string value(argv[1]);
    if (value.empty() || value.find_first_not_of("0123456789") != std::string::npos)
      throw std::invalid_argument("Port must contain decimal digits");
    port = std::stoul(value);
    if (port > 65535) throw std::out_of_range("Port must be 0-65535");
  }
  boost::asio::io_context io_context;

  // Server setup
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), static_cast<unsigned short>(port));
  boost::asio::ip::tcp::acceptor acceptor(io_context, endpoint);

  std::cout << "Server listening on port " << acceptor.local_endpoint().port() << "..." << std::endl;

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
 } catch (const std::exception& error) {
   std::cerr << "Server error: " << error.what() << std::endl;
   return 1;
 }
}