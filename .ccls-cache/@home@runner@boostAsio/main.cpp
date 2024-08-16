#include <boost/asio.hpp>
#include <iostream>

int main() {
boost::asio::io_context io_context;
boost::asio::ip::tcp::socket socket(io_context); 
std::cout << "Hello, Asio world!" << std::endl;

boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"),8080); 
socket.connect(endpoint);
  
std::string message = "Hello from client!";
boost::asio::write(socket, boost::asio::buffer(message));

// Receiving Data
char data[1024];
boost::system::error_code error;
size_t length = socket.read_some(boost::asio::buffer(data), error);

if (!error) {
  std::cout << "Received: " << std::string(data, length) << std::endl;
} else {
  std::cerr << "Error: " << error.message() << std::endl;
}
  return 0;
}