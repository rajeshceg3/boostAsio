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

  return 0;
}