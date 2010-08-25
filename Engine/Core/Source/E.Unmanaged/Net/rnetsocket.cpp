#include "roket3d.h"
#include "rnetsocket.h"

namespace net
{
	const char *RNetSocket::className = TYPE_RNETSOCKET;
	const Luna < RNetSocket >::FunctionType RNetSocket::Functions[] = {
		{"createConnection", &RNetSocket::createConnection},
  {"destroyConnection", &RNetSocket::destroyConnection},
  {"checkWaiting", &RNetSocket::checkWaiting},
  {"closeClient", &RNetSocket::closeClient},
  {"acceptClient", &RNetSocket::acceptClient},
  {"getClientAddress", &RNetSocket::getClientAddress},
  {"receiveData", &RNetSocket::receiveData},
  {"sendData", &RNetSocket::sendData},
		{0}
	};
	const Luna < RNetSocket >::PropertyType RNetSocket::Properties[] = {
	//{ "visible", &layer::get_visible, &layer::set_visible },
		{ 0 }
	};
	
	RNetSocket::RNetSocket(lua_State* L, bool instantedByUser)
	{
		if (NetInitalized==false)
			debug("You cannot create RNetSockets because Roket3D could not initalise SDL_net earlier.  Please restart the application to access network resources.",LVL_ERR,L);
		objectExists=true;
		isConnected=false;
		isServer=false;
		previousData=false;
	}

	RNetSocket::~RNetSocket()
	{
		if (isConnected)
		{
			if (isServer)
			{
				if (previousData)
					SDLNet_TCP_Close(client_socket);
				SDLNet_TCP_Close(server_socket);
				SDLNet_FreeSocketSet(s_set);
			}
			else
			{
				SDLNet_TCP_Close(host_socket);
				SDLNet_FreeSocketSet(s_set);
			}	
		}
	}
	
	int RNetSocket::setObject(TCPsocket object)
	{
		obj=object;
		objectExists=true;
		return 0;
	}
	
	TCPsocket RNetSocket::getObject()
	{
		if (objectExists)
			return obj;
		else
			return NULL;
	}
	
	int RNetSocket::createConnection(lua_State* L)
	{
		ifistrue(objectExists);

		if (isConnected)
		{
			debug("Already connected.  Disconnect first, or use another RNetSocket.",LVL_ERR,L);
		}
		else if (lua_gettop(L)!=2 && lua_gettop(L)!=3)
		{
			// Not enough arguments to determine client or server.
			isServer=false;
			debug("Cannot create connection.  Not enough arguments to determine connection type.",LVL_ERR,L);
		}
		else if (lua_isnumber(L,2))
		{
			int port = getmandatoryargument_cNumber(L,1);
			isServer=true; // listens on localhost, port specified by number
			if(SDLNet_ResolveHost(&server_ip,NULL,port) == -1)
			{
				debug("Unable to listen on port " << port << ".",LVL_ERR,L);
				lua_pushboolean(L,false);
				return 1;
			}
			server_socket=SDLNet_TCP_Open(&server_ip); 
			if(!server_socket)
			{ 
				debug("Cannot create server. " << SDLNet_GetError(),LVL_ERR,L);
				lua_pushboolean(L,false);
				return 1;
			}
			s_set = SDLNet_AllocSocketSet(2);
			int numused = SDLNet_TCP_AddSocket(s_set, server_socket);
			if (numused == -1) {
				debug("Cannot create socket monitor. " << SDLNet_GetError(),LVL_ERR,L);
				SDLNet_TCP_Close(server_socket);
				SDLNet_FreeSocketSet(s_set);
				lua_pushboolean(L,false);
				return 1;
			}
			debug("Successfully created server on port " << port << ".",LVL_INFO,L);
			isConnected=true;
			lua_pushboolean(L,true);
			return 1;
		}
		else
		{
			const char* server = getmandatoryargument_constchar(L,1);
			int port = getmandatoryargument_cNumber(L,2);

			if (SDLNet_ResolveHost(&host_ip, server, port) < 0)
			{
				debug("Unable to connect to server " << server << ":" << port << " " << SDLNet_GetError(),LVL_ERR,L);
				lua_pushboolean(L,false);
				return 1;
			}
			else if (!(host_socket = SDLNet_TCP_Open(&host_ip)))
			{
				debug("Unable to connect to server " << server << ":" << port << " " << SDLNet_GetError(),LVL_ERR,L);
				lua_pushboolean(L,false);
				return 1;
			}
			s_set = SDLNet_AllocSocketSet(2);
			int numused = SDLNet_TCP_AddSocket(s_set, host_socket);
			if (numused == -1) {
				debug("Cannot create socket monitor. " << SDLNet_GetError(),LVL_ERR,L);
				SDLNet_TCP_Close(host_socket);
				SDLNet_FreeSocketSet(s_set);
				lua_pushboolean(L,false);
				return 1;
			}
			else
			{
				isConnected=true;
				isServer=false;
				debug("Connected to server.",LVL_INFO,L);
				lua_pushboolean(L,true);
				return 1;
			}
		}
		return 0;
		
		endifisvalid();
	}

	int RNetSocket::destroyConnection(lua_State* L)
	{
		ifistrue(objectExists);

		if (isConnected)
		{
			if (isServer)
			{
				if (previousData)
					SDLNet_TCP_Close(client_socket);
				SDLNet_TCP_Close(server_socket);
				SDLNet_FreeSocketSet(s_set);
			}
			else
			{
				SDLNet_TCP_Close(host_socket);
				SDLNet_FreeSocketSet(s_set);
			}	
		}
		return 0;
		
		endifisvalid();
	}

	int RNetSocket::closeClient(lua_State* L)
	{
		ifistrue(objectExists);

		if (isServer)
		{
			if (previousData)
			{
				SDLNet_TCP_Close(client_socket);
				SDLNet_TCP_DelSocket(s_set,client_socket);
				previousData = false;
			}
		}

		endifisvalid();
	}

	int RNetSocket::acceptClient(lua_State* L)
	{
		ifistrue(objectExists);

		if (isServer)
		{
			int waittime = getmandatoryargument_cNumber(L,1);
			
			int numsets = SDLNet_CheckSockets(s_set, waittime);
			if (numsets == -1)
			{
				debug("Could not check sockets.",LVL_ERR,L);
				lua_pushboolean(L,false);
			}
			else if (numsets)
			{
				if (SDLNet_SocketReady(server_socket)>0)
				{
					client_socket = SDLNet_TCP_Accept(server_socket);
					previousData=false;
					if (client_socket)
					{
						int numused = SDLNet_TCP_AddSocket(s_set, client_socket);
						if (numused == -1)
						{
							SDLNet_TCP_Close(client_socket);
							lua_pushboolean(L,false);
						}
						else
						{
							previousData=true;
							lua_pushboolean(L,true);
						}
					}
					else
						lua_pushboolean(L,false);
				}
			}
			else
				lua_pushboolean(L,false);
		}

		return 1;

		endifisvalid();
	}

	int RNetSocket::checkWaiting(lua_State* L)
	{
		ifistrue(objectExists);

		if (!isConnected)
		{
			debug("Could not check for waiting sockets.  No connection available.",LVL_ERR,L);
			lua_pushboolean(L,false);
			return 1;
		}
		if (isServer)
		{
			if (previousData)
			{
				int waittime = getmandatoryargument_cNumber(L,1);
				
				int numsets = SDLNet_CheckSockets(s_set, waittime);
				if (numsets == -1)
				{
					debug("Could not check sockets.",LVL_ERR,L);
					lua_pushboolean(L,false);
				}
				else if (numsets)
				{
					if (SDLNet_SocketReady(client_socket))
						lua_pushboolean(L,true);
					else
						lua_pushboolean(L,false);
				}
				else
					lua_pushboolean(L,false);
			}
			else
				lua_pushboolean(L,false);
		}
		else
		{
			int waittime = getmandatoryargument_cNumber(L,1);
			int numsets = SDLNet_CheckSockets(s_set, waittime);
			if (numsets == -1)
			{
				debug("Could not check sockets.",LVL_ERR,L);
				lua_pushboolean(L,false);
			}
			else if (numsets)
			{
				if (SDLNet_SocketReady(host_socket))
					lua_pushboolean(L,true);
				else
					lua_pushboolean(L,false);
			}
			else
				lua_pushboolean(L,false);
		}
		return 1;
		
		endifisvalid();
	}

	int RNetSocket::getClientAddress(lua_State* L)
	{
		ifistrue(objectExists);

		if (!isConnected)
		{
			debug("Could not get client address.  No connection available.",LVL_ERR,L);
			lua_pushboolean(L,false);
			return 1;
		}
		if (isServer)
		{
			if (!previousData)
			{
				debug("Cannot check client address.  Please use acceptClient first.",LVL_ERR,L);
				lua_pushboolean(L,false);
				return 1;
			}	
			else
			{
				client_ip = SDLNet_TCP_GetPeerAddress(client_socket);
				lua_pushstring(L,convertIPToString(SDLNet_Read32(&client_ip->host)).c_str());
				lua_pushnumber(L,SDLNet_Read16(&client_ip->port));
				return 2;
			}
		}
		else
		{
			debug("getClientAddress cannot be called on client sockets.",LVL_ERR,L);
			lua_pushboolean(L,false);
			return 1;
		}
		
		endifisvalid();
	}

	int RNetSocket::receiveData(lua_State* L)
	{
		ifistrue(objectExists);

		if (!isConnected)
		{
			debug("Could not recieve data.  No connection available.",LVL_ERR,L);
			lua_pushboolean(L,false);
			return 1;
		}
		if (isServer)
		{
			char buffer[512];
			if (previousData)
			{
				int ret = SDLNet_TCP_Recv(client_socket, buffer, 512);
				if (ret > 0)
				{
					lua_pushstring(L,buffer);
					lua_pushnumber(L,ret);
					return 2;
				}
				else
				{
					lua_pushnil(L);
					lua_pushnumber(L,ret);
					return 2;
				}
			}
			else
			{
				debug("Cannot receive data.  Please use acceptClient first.",LVL_ERR,L);
			}
		}
		else
		{
			char buffer[512];
			int waittime = getmandatoryargument_cNumber(L,1);
			if (SDLNet_CheckSockets(s_set, waittime)<1)
			{
				debug("No activity on host socket.",LVL_WARN,L);
				lua_pushnil(L);
				return 1;
			}
			else if (SDLNet_SocketReady(host_socket)==0)
			{
				debug("No activity on host socket.",LVL_WARN,L);
				lua_pushnil(L);
				return 1;
			}
			else if (SDLNet_TCP_Recv(host_socket, buffer, 512) > 0)
			{
				lua_pushstring(L,buffer);
				return 1;
			}
			else
			{
				lua_pushnil(L);
				return 1;
			}
		}
		
		endifisvalid();
	}

	int RNetSocket::sendData(lua_State* L)
	{
		ifistrue(objectExists);
 
		if (!isConnected)
		{
			debug("Could not send data.  No connection available.",LVL_ERR,L);
			lua_pushboolean(L,false);
			return 1;
		}
		if (isServer)
		{
			if (previousData)
			{
				const char* buffer = getmandatoryargument_constchar(L,1);
				int len = strlen(buffer) + 1;
				if (SDLNet_TCP_Send(client_socket, buffer, len) < len)
				{
					debug("Failed to send " << len << " bytes of data. " << SDLNet_GetError(),LVL_ERR,L);
					lua_pushboolean(L,false);
				}
				else
					lua_pushboolean(L,true);
			}
			else
			{
				debug("Cannot send data.  Please use acceptClient first.",LVL_ERR,L);
			}
		}
		else
		{
			const char* buffer = getmandatoryargument_constchar(L,1);
			int len = strlen(buffer) + 1;
			if (SDLNet_TCP_Send(host_socket, buffer, len) < len)
			{
				debug("Failed to send " << len << " bytes of data. " << SDLNet_GetError(),LVL_ERR,L);
				lua_pushboolean(L,false);
			}
			else
				lua_pushboolean(L,true);
		}
		return 1;
			
		endifisvalid();
	}

	irr::core::stringc RNetSocket::convertIPToString(Uint32 ip)
	{
		irr::core::stringc result;
		Uint32 longg = ip;
		if (longg < 0 || longg > 4294967295) return "invalid";
        result = "";
        for (int i=3;i>=0;i--) {
            result += (int)(longg / pow((float)256,i));
            longg -= (int)(longg / pow((float)256,i))*pow((float)256,i);
            if (i>0) result += ".";
        }
		return result;
	}
}
