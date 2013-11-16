#include <cstdlib>
#include <fstream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>


using boost::asio::ip::tcp;


const int max_length = 1024;
int R,G,B;
std::ofstream o("/dev/spidev0.0");
struct rgb{
	rgb (int r, int g, int b) :r(r), g(g), b(b){};
	int r : 12;
	int g : 12;
	int b : 12;

};

void set_values(int R, int G, int B)
{
	std::stringstream sstr;
	// RED
	sstr << std::hex << (R & 0xFFF);
	std::string wr; 
	sstr >> wr;
	if (R < 0xFF) {
		wr = "0" +wr;
	} 
	if (R < 0x0F) {
		wr = "0" +wr;
	}
	if (R == 00){
		wr = "000";
	}
	//GREEN
	sstr << std::hex << (G & 0xFFF);
	std::string wg; 
	sstr >> wg;
	if (G < 0xFF) {
		wg = "0" +wg;
	} 
	if (G < 0x0F) {
		wg = "0" +wg;
	}	
	if (G == 0) {
		wg = "000";
	}
	//BLUE
	sstr << std::hex << (B & 0xFFF);
	std::string wb; 
	sstr >> wb;
	if (B < 0xFF) {
		wb = "0" +wb;
	} 
	if (B < 0x0F) {
		wb = "0" +wb;
	}
	if (B == 0){
		wb = "000";
	}
	std::string w = wr + wg + wb;
	std::cout << "W: " << w <<std::endl;
	for (int i = 0; i < 8; i++) {
		w += w;
	}
	o << w;

}

void writetospi(char * data)
{
	std::stringstream sstr(data);
	std::string type;
	int val;
	sstr >> type >> val;
	std::cout << "Setting " << type << " to " << val << std::endl;
	if (type == "r") {
		R = val;
	} else if (type == "g") {
		G = val;
	} else if (type == "b") {
		B = val;
	} 
	set_values(R,G,B);
}


void session(tcp::socket sock)
{
	try
	{
		for (;;)
		{
			char data[max_length];

			boost::system::error_code error;
			size_t length = sock.read_some(boost::asio::buffer(data), error);
			if (error == boost::asio::error::eof)
        		break; // Connection closed cleanly by peer.
    		else if (error)
        	throw boost::system::system_error(error); // Some other error.

    
    	writetospi(data);


    	boost::asio::write(sock, boost::asio::buffer(data, length));
		}		
	}

catch (std::exception& e)
{
	std::cerr << "Exception in thread: " << e.what() << "\n";
}
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
	for (;;)
	{
		tcp::socket sock(io_service);
		a.accept(sock);
		std::thread(session, std::move(sock)).detach();
	}
}

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;

		server(io_service, 4711);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}