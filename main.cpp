#include <boost/asio.hpp>
#include <iostream>

int main() {
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::socket socket(io_context); 
  std::cout << "Hello, Asio world!" << std::endl;
  return 0;
}