#ifndef HEADER_RNETSOCKET
#define HEADER_RNETSOCKET
namespace net
{
	class RNetSocket : public RObject
	{
		
		// This class is a network socket.  It is capable of connection, and read-write.
		TCPsocket obj;
		bool objectExists;
		bool isServer;
		bool isConnected;
		bool previousData;

		TCPsocket server_socket; // (SERVER) Server socket.
		IPaddress server_ip;     // (SERVER) Server IP.
		TCPsocket client_socket; // (SERVER) Client socket.
		IPaddress *client_ip;    // (SERVER) Client IP.
		TCPsocket host_socket;   // (CLIENT) Server socket.
		IPaddress host_ip;       // (CLIENT) Server IP.
		SDLNet_SocketSet s_set;  // (BOTH)   Socket Monitor
        //IPaddress server_ip,*client_ip; 
		//TCPsocket server,client;

		public:
			RNetSocket(lua_State* L, bool instantedByUser);
			~RNetSocket();
			int createConnection(lua_State* L);				// connect/listen
			int destroyConnection(lua_State* L);			// disconnect
			int closeClient(lua_State* L);					// close client
			int acceptClient(lua_State* L);					// accept client
			int checkWaiting(lua_State* L);					// check for waiting
			int getClientAddress(lua_State* L);				// returns the address of the connected client
			int receiveData(lua_State* L);					// receive data over protocol
			int sendData(lua_State* L);						// send data over protocol

			irr::core::stringc convertIPToString(Uint32 ip);

			int setObject(TCPsocket object);
			TCPsocket getObject();

			static const char *className;
			static const Luna < RNetSocket >::FunctionType Functions[];
			static const Luna < RNetSocket >::PropertyType Properties[];
	};
}
#endif
