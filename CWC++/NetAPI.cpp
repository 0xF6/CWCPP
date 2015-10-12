#define _WINSOCKAPI_
#include <Windows.h>
#include <Winhttp.h>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "Symbols.h"
#include "Types.h"
#include "NetAPI.h"
#include "EIdent.h"

#pragma comment(lib, "Winhttp.lib")
#pragma comment(lib, "Ws2_32.lib")

#define BUFF_LEN 2048


NetPort::NetPort(CString port)
{
	this->port = port;
}

NetPort::NetPort(unsigned int port)
{
	if (port <= 65535) this->port = DTTS(port);
}

PCSTR NetPort::get()
{
	return port.data();
}

bool NetPort::operator==(CString port)
{
	return (this->port == port);
}

bool NetPort::operator==(unsigned int port)
{
	return (this->port == DTTS(port));
}

CWString GetMode(NetMode mode)
{
	switch (mode)
	{
	case NetMode::GET:
		return L"GET";
	case NetMode::POST:
		return L"POST";
	case NetMode::HEAD:
		return L"HEAD";
	case NetMode::PUT:
		return L"PUT";
	}
	return L"";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

NetRequests::NetRequests(CWString addr)
{
	this->addr = addr;
}

NetRequests::~NetRequests()
{
	clear();
}

CWString NetRequests::GetAddr()
{
	return this->addr;
}

void NetRequests::add(HINTERNET hReq)
{
	this->hRequests.push_back(hReq);
}

size_t NetRequests::count()
{
	return this->hRequests.size();
}

void NetRequests::remove(size_t num)
{
	if (num >= hRequests.size()) return;
	WinHttpCloseHandle(hRequests[num]);
	hRequests.remove(num);
}

void NetRequests::clear()
{
	if (this->hRequests.empty()) return;
	for (register size_t pos = 0; pos < this->hRequests.size(); pos++)
	{
		WinHttpCloseHandle(hRequests[pos]);
	}
	this->hRequests.clear();
}

HINTERNET NetRequests::get(size_t num)
{
	return *this->hRequests[num];
}

HINTERNET NetRequests::operator[](size_t num)
{
	return *this->hRequests[num];
}

bool NetRequests::operator==(CWString addr)
{
	return (this->addr == addr);
}

bool NetRequests::operator!=(CWString addr)
{
	return (this->addr != addr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void StringData::Add(CString data)
{
	this->data.push_back(data);
}

CString StringData::Get()
{
	if (this->data.empty()) return "";
	CString temp = *this->data[0];
	this->data.erase(0);
	return temp;
}

bool StringData::Empty()
{
	return this->data.empty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

//Коструктор копии протокола
NetSocketProtocol::NetSocketProtocol(NetSocketProtocol &protocol)
{
	this->value = protocol.value;
}

//Коструктор протокола - перечисление в структуру
NetSocketProtocol::NetSocketProtocol(ESocketProtocol protocol)
{
	this->value = protocol;
}

//Костурктор протокола - стандартное перечесление в структуру
NetSocketProtocol::NetSocketProtocol(IPPROTO protocol)
{
	switch (protocol)
	{
		case IPPROTO::IPPROTO_TCP:
		{
			this->value = ESocketProtocol::ESP_TCP;
		}
		case IPPROTO::IPPROTO_UDP:
		{
			this->value = ESocketProtocol::ESP_UDP;
		}
		case IPPROTO::IPPROTO_ICMP:
		{
			this->value = ESocketProtocol::ESP_ICMP;
		}
		default:
		{
			this->value = ESocketProtocol::ESP_TCP;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

//Конструктор сокета
NetSocket::NetSocket(SOCKET sock, //сокет
					 NetSocketProtocol protocol, //протокол
					 SocketType type //тип (клиент/сервер)
					 ): Event(WSA_INVALID_EVENT), ready(false), protocol(protocol), last_connect_id(0)
{
	if (sock == INVALID_SOCKET) return;
	this->type = type;
	this->sock = sock;
	this->Init();
}

//Иницилизация
void NetSocket::Init()
{
	this->Event = WSACreateEvent();
	this->connections.clear();
	WSAEventSelect(this->sock, this->Event, FD_ALL_EVENTS);
	closed = false;
	ready = true;
}


//Обработка событий
void NetSocket::HanldeEvents()
{
	DWORD time = 1;
	int nReturnCode = WSAWaitForMultipleEvents(1, &this->Event, FALSE, time, FALSE); 
	switch (this->type)
	{
		case SocketType::Sock_Server:
		{
			WSANETWORKEVENTS ConnectEvent;
			WSAEnumNetworkEvents(this->sock, this->Event, &ConnectEvent);
			///////////////////////////////////////////////////////////
			if( (ConnectEvent.lNetworkEvents & FD_ACCEPT) &&
				(ConnectEvent.iErrorCode[FD_ACCEPT_BIT] == 0) )
			{
				this->CreateConnectionSocket();
			}
			///////////////////////////////////////////////////////////
start_from_connections:
			if (!this->connections.empty())
			{
				CString data;
				bool bSendData = false;
				if (!this->DataToSend.Empty())
				{
					data = this->DataToSend.Get();
					if (!data.empty())
					{
						if (data[data.size() - 1] != '\0') data += '\0'; 
						bSendData = true;
					}
				}
				///////////////////////////////////////////////////////////
				for (register size_t _pos = 0; _pos < this->connections.size(); _pos++)
				{
					Pair<size_t, NetSocket*> *p = this->connections.get(_pos);
					if (p->object->IsClosed())
					{
						this->connections.erase(p);
						goto start_from_connections;
					}
					else
					{
						if (bSendData) 
						{
							int iSResult = 0;
							iSResult = send(p->object->sock, data.data(), (int) data.size(), 0);
						}
						///////////////////////////////////////////////////////////
						if (DataToSend_Connections.find(p->key) != NULL)
						{
							CString data;
							bool bSendData = false;
							data = DataToSend_Connections[p->key]->object.Get();
							if (!data.empty())
							{
								if (data[data.size() - 1] != '\0') data += '\0'; 
								bSendData = true;
							}
							if (bSendData)
							{
								int iSResult = 0;
								iSResult = send(p->object->sock, data.data(), (int) data.size(), 0);
							}
						}
						///////////////////////////////////////////////////////////
						int nReturnCode = WSAWaitForMultipleEvents(1, &p->object->Event, FALSE, time, FALSE); 
						WSANETWORKEVENTS ProcessEvent;
						WSAEnumNetworkEvents(p->object->sock, p->object->Event, &ProcessEvent);
						if ( (ProcessEvent.lNetworkEvents & FD_CLOSE) &&
								(ProcessEvent.iErrorCode[FD_CLOSE_BIT] == 0) )
						{
							p->object->Close();
							this->connections.erase(p);
							printf_s("\nConnection closed.\n");
							goto start_from_connections;
						}
						else if ( (ProcessEvent.lNetworkEvents & FD_READ) &&
								(ProcessEvent.iErrorCode[FD_READ_BIT] == 0) )
						{
							CString RData;
							char *recvbuf = new char[BUFF_LEN];
							int iRResult = recv(p->object->sock, recvbuf, BUFF_LEN, 0);
							if (iRResult > 0)
							{
								RData = recvbuf;
							}
							delete [] recvbuf;
							if (ReceivedData_Connections.find(p->key) != NULL)
							{
								ReceivedData_Connections[p->key]->object.Add(RData);
							}
							else
							{
								ReceivedData.Add(RData);
							}
						}
					}
					///////////////////////////////////////////////////////////
					p++;
				}
			}
			///////////////////////////////////////////////////////////
			break;
		}
		case SocketType::Sock_Client:
		{
			WSANETWORKEVENTS ProcessEvent;
			WSAEnumNetworkEvents(this->sock, this->Event, &ProcessEvent);
			///////////////////////////////////////////////////////////
			if( (ProcessEvent.lNetworkEvents & FD_CLOSE) &&
				(ProcessEvent.iErrorCode[FD_CLOSE_BIT] == 0) )
			{
				this->Close();
				this->closed = true;
				printf_s("\nConnection closed.\n");
			}
			///////////////////////////////////////////////////////////
			if( (ProcessEvent.lNetworkEvents & FD_READ) &&
				(ProcessEvent.iErrorCode[FD_READ_BIT] == 0) )
			{
				char *recvbuf = new char[BUFF_LEN];
				int iRResult = recv(this->sock, recvbuf, BUFF_LEN, 0);
				if (iRResult > 0)
				{
					this->ReceivedData.Add(recvbuf);
				}
				delete [] recvbuf;
			}
			///////////////////////////////////////////////////////////
			CString data;
			bool bSendData = false;
			if (!this->DataToSend.Empty())
			{
				data = this->DataToSend.Get();
				if (!data.empty())
				{
					if (data[data.size() - 1] != '\0') data += '\0';
					bSendData = true;
				}
			}
			///////////////////////////////////////////////////////////
			if (bSendData) 
			{
				int size = (int) data.size();
				char *cdata = (char *) data.data();
				int iSResult = send(this->sock, data.data(), size, 0);
			}
			///////////////////////////////////////////////////////////
			break;
		}
		case SocketType::Sock_Connection:
		{
			//There is nothing to do, because HandleEvents aren't calls for Connections
			break;
		}
	}
	///////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

//Добавляет данные в очередь на отправку
void NetSocket::SendData(CString &Data)
{
	this->DataToSend.Add(Data);
}

void NetSocket::SendData(CString &Data, size_t ConnectId)
{
	Pair<size_t, StringData> *p = this->DataToSend_Connections.find(ConnectId);
	if (p != NULL)
		p->object.Add(Data);
}


//Возвращает полученные данные
StringData *NetSocket::GetData()
{
	if (this->ReceivedData.Empty()) return NULL;
	return &this->ReceivedData;
}

StringData *NetSocket::GetData(size_t ConnectId)
{
	Pair<size_t, StringData> *p = this->ReceivedData_Connections.find(ConnectId);
	if (p != NULL)
		return &p->object;
	else
		return NULL;
}

//
bool NetSocket::HasData()
{
	return (!this->ReceivedData.Empty());
}

//
bool NetSocket::HasData(size_t ConnectionId)
{
	if (this->ReceivedData_Connections.empty()) return false;
	Pair<size_t, StringData> *p;
	p = this->ReceivedData_Connections.find(ConnectionId);
	if (p != NULL)
	{
		return !p->object.Empty();
	}
	else
	{
		return false;
	}
}

ConnectionList NetSocket::GetConnectionList()
{
	ConnectionList list;
	if (this->connections.empty()) return list;
	Pair<size_t, NetSocket*> *p;
	for (register size_t _pos = 0; _pos < this->connections.size(); _pos)
	{
		p = this->connections.get(_pos);
		list.list.push_back(p->key);
	}
	return list;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

//Проверяет, закрыт ли сокет
bool NetSocket::IsClosed()
{
	return this->closed;
}

//Создание сокета соеденения с сервером
void NetSocket::CreateConnectionSocket()
{
	SOCKET ClientSocket = INVALID_SOCKET;
	// Accept a client socket
	ClientSocket = accept(this->sock, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("Accept failed: %d\n", WSAGetLastError());

	}
	NetSocket *connection = new NetSocket(ClientSocket, this->protocol, SocketType::Sock_Connection);
	this->connections.insert(last_connect_id, connection);
	this->DataToSend_Connections.insert(last_connect_id, StringData());
	this->ReceivedData_Connections.insert(last_connect_id, StringData());
	this->last_connect_id++;
	printf_s("\nClient are connected.\n");

}

//Уничтожает сокет
void NetSocket::Destroy()
{
	WSACloseEvent(this->Event);
	closesocket(this->sock);
	ready = false;
}

//Закрывает сокет
void NetSocket::Close()
{
	if (!ready) return;
	if (!this->connections.empty())
	{
		for (register size_t _pos = 0; _pos < this->connections.size(); _pos++)
		{
			Pair<size_t, NetSocket*> *p = this->connections.get(_pos);
			p->object->Close();
			delete (p->object);
			p++;
		}
		this->connections.clear();
	}
	this->DataToSend_Connections.clear();
	this->ReceivedData_Connections.clear();
	this->Destroy();
	this->closed = true;
}




//Деструктор
NetSocket::~NetSocket()
{
	this->Close();
}

//Возвращает готовность сокета к использованию
bool NetSocket::IsReady()
{
	return ready;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

//Открытие сессии
void NetAPI::OpenSession() 
{
	if (!hSession)
	// Create the session handle using the default settings.
    hSession = WinHttpOpen( L"ARA", 
                            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                            WINHTTP_NO_PROXY_NAME,
                            WINHTTP_NO_PROXY_BYPASS,
                            NULL);
}

//Закрытие сессии
void NetAPI::CloseSession()
{
	if (hSession) 
	{
		WinHttpCloseHandle(hSession);
		this->hSession = NULL;
	}
}

bool NetAPI::IsSessionOpen()
{
#pragma warning(disable:4800)
	return this->hSession;
}

//Подключение к адресу
bool NetAPI::Connect(CWString addr, INTERNET_PORT port)
{
	if (!hSession)
		return false;
	HINTERNET hConnection = NULL;
	if (hConnections.find(addr) != NULL)
		return false;
	hConnection = WinHttpConnect( hSession, addr.data(), port, 0);
	if (hConnection)
	{
		hConnections.insert(addr, hConnection);
		requests.push_back(NetRequests(addr));
		return true;
	}
	return false;
}

//Отключаение от адреса
void NetAPI::Disconnect(CWString addr)
{
	for (register size_t pos = 0; pos < requests.size(); pos++)
	{
		if (*requests[pos] == addr)
		{
			requests[pos]->clear();
			requests.remove(pos);
			break;
		}
	}
	Pair<CWString, HINTERNET> *p = hConnections.find(addr);
	if (p != NULL) //поиск удачен
	{
		if (p->object) 
		{
			WinHttpCloseHandle(p->object);
			hConnections.erase(p);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

NetAPI::NetAPI()
{
	Init();
}


void NetAPI::Init()
{
	this->hSession = NULL;
	this->hConnections.clear();
	this->requests.clear();
	this->sockets.clear();
	ready = true;
}

void NetAPI::Release()
{
	if (!ready) return;
	/////////////////////////////////////////////
	if (!this->sockets.empty())
	{
		try {
			this->sockets.clear();
		}
		catch (...)
		{

		}
	}
	this->StopSocketWork();
	/////////////////////////////////////////////
	//Закрываем все запросы
	if (!this->requests.empty())
	{
		while(!requests.empty())
		{
			requests[0]->clear();
			requests.remove(0);
		}
	}
	/////////////////////////////////////////////
	if (!this->hConnections.empty()) //Закрываем подключения
	{
		for (register size_t _pos = 0; _pos < this->hConnections.size(); _pos++)
		{
			Pair<CWString, HINTERNET> *pc = hConnections.get(_pos);
			WinHttpCloseHandle(pc->object);
		}
		this->hConnections.clear();
	}
	/////////////////////////////////////////////
	this->CloseSession(); //Закрываем сессию
	/////////////////////////////////////////////
	ready = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


//Создание запроса
size_t NetAPI::OpenRequest(CWString addr, NetMode mode, CWString data)
{
	if (hConnections.empty())
	{
		throw "EMPTY";
		return 0;
	}
	Pair<CWString, HINTERNET> *p = hConnections.find(addr); //ищем подключение
	if (p != NULL) //поиск удачен
	{
		HINTERNET hConnect = ((HINTERNET) (p->object));
		if (hConnect) 
		{
			HINTERNET req = WinHttpOpenRequest( hConnect, (LPWSTR) GetMode(mode).data(), 
												data.data(), 
												NULL, WINHTTP_NO_REFERER, 
												WINHTTP_DEFAULT_ACCEPT_TYPES, NULL);
			if (!req)
			{
				printf("Error - cannot open request: %u\n", GetLastError());
				throw "ERROR_OPEN_REQUEST";
				return 0;
			}
			else
			{
				for (register size_t pos = 0; pos < requests.size(); pos++)
				{
					if (*requests[pos] == addr) 
					{
						requests[pos]->add(req);
						return requests[pos]->count() - 1;
					}
				}
			}
		}
	}
	return 0;
}

//Отсылка запроса, получение ответа
void NetAPI::SendRequest(CWString addr, size_t num, CString *data)
{
	DWORD size = 0;
	register size_t pos = 0;
	for (pos; pos < requests.size(); pos++)
	{
		if (*(requests[pos]) == addr) 
		{
			break;
		}
	}
	BOOL result = FALSE;
	HINTERNET req = requests[pos]->get(num);
	if (req)
	{
		result = WinHttpSendRequest( req,
							WINHTTP_NO_ADDITIONAL_HEADERS,
							0, WINHTTP_NO_REQUEST_DATA, 0, 
							data->size(), 0);
		if (result)
		{
			if (data->size())
			{
				DWORD dwBytesWritten;
				result = WinHttpWriteData( req, data->data(), 
					data->size(), &dwBytesWritten);
				if (result) result = WinHttpReceiveResponse( req, NULL);
			}
			else
			{
				result = WinHttpReceiveResponse( req, NULL);
				DWORD downloaded = 0;
				do {
					if (!WinHttpQueryDataAvailable(req, &size))
					{
						printf("Error: %u", GetLastError());
						return;
					}
					char *buff = new char[size+1];
					ZeroMemory(buff, size+1);
					WinHttpReadData(req, (LPVOID) buff, size, &downloaded);
					buff[downloaded] = '\0';
					*data += buff;
					delete [size+1] buff;
				} while (size > 0);
			}
		}
	}
}

//Отсылка запроса, получение ответа
void NetAPI::SendRequest(CWString addr, size_t num, CString &data, size_t *downloaded_size)
{
	DWORD size = 0;
	register size_t pos = 0;
	for (pos; pos < requests.size(); pos++)
	{
		if (*requests[pos] == addr) 
		{
			break;
		}
	}
	BOOL result = FALSE;
	HINTERNET req = requests[pos]->get(num);
	if (req)
	{
		result = WinHttpSendRequest( req,
							WINHTTP_NO_ADDITIONAL_HEADERS,
							0, WINHTTP_NO_REQUEST_DATA, 0, 
							data.len(), 0);
		if (result)
		{
			if (data.len())
			{
				DWORD dwBytesWritten;
				result = WinHttpWriteData( req, data.data(), 
					data.len(), &dwBytesWritten);
				if (result) result = WinHttpReceiveResponse( req, NULL);
			}
			else
			{
				result = WinHttpReceiveResponse( req, NULL);
				do {
					DWORD downloaded;
					if (!WinHttpQueryDataAvailable(req, &size))
					{
						printf("Error: %u", GetLastError());

						return;
					}
					char *buff = new char[size+1];
					buff[size] = '\0';
					WinHttpReadData(req, (LPVOID) buff, size, &downloaded);
					if (downloaded_size) *downloaded_size += downloaded;
					static size_t tryed = 0;
					new_try:
					try {
						tryed++;
						data.add(buff, (size_t) downloaded);
						delete [size+1] buff;
						tryed = 0;
					}
					catch (std::bad_alloc e)
					{
						if (tryed < 100)
							goto new_try;
						else
							throw "DOWNLOAD_FAILED:BAD_ALLOC";
					}
				} while (size > 0);
			}
		}
	}
}

//Отсылка запроса, получение ответа
void NetAPI::SendRequest(CWString addr, size_t num, CString req_data, FileStream &data, size_t *downloaded_size)
{
	DWORD size = 0;
	register size_t pos = 0;
	for (pos; pos < requests.size(); pos++)
	{
		if (*requests[pos] == addr) 
		{
			break;
		}
	}
	BOOL result = FALSE;
	HINTERNET req = requests[pos]->get(num);
	if (req)
	{
		result = WinHttpSendRequest( req,
							WINHTTP_NO_ADDITIONAL_HEADERS,
							0, WINHTTP_NO_REQUEST_DATA, 0, 
							req_data.len(), 0);
		if (result)
		{
			if (req_data.len())
			{
				DWORD dwBytesWritten;
				result = WinHttpWriteData( req, req_data.data(), 
					req_data.len(), &dwBytesWritten);
				if (result) result = WinHttpReceiveResponse( req, NULL);
			}
			else
			{
				result = WinHttpReceiveResponse( req, NULL);
				do {
					DWORD downloaded;
					if (!WinHttpQueryDataAvailable(req, &size))
					{
						printf("Error: %u", GetLastError());

						return;
					}
					char *buff = new char[size];
					WinHttpReadData(req, (LPVOID) buff, size, &downloaded);
					if (downloaded_size) *downloaded_size += downloaded;
					static size_t tryed = 0;
					data.WriteData(buff, (size_t) downloaded, true);
					delete [size] buff;
				} while (size > 0);
			}
		}
	}
}

//Проверяет подключение
bool NetAPI::Check(CWString addr)
{
	if (!hSession) return false;
	if (this->Connect(addr, INTERNET_DEFAULT_PORT))
	{
		CString data;
		size_t num;
		num = this->OpenRequest(addr, NetMode::GET, L"");
		this->SendRequest(addr, num, &data);
		if (!data.empty()) return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////




//Запустить работу сокетов
void NetAPI::StartSocketWork()
{
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &this->socket_work.wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);

		return;
	}
	this->socket_work.ready = true;
}

//Остановить работу сокетов
void NetAPI::StopSocketWork()
{
	WSACleanup();
}

//Создание клиента
bool NetAPI::ConnectToServer(CString server_name, //имя сервера
							 NetPort port, //порт
							 NetSocketProtocol protocol, //протокол
							 CString str_id //имя сокета (идентификатор)
							 )
{
	//SOCKET ListenSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	SOCKET ConnectSocket = INVALID_SOCKET;
	int iResult;
	///////////////////////////////////////////////////////////
	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = protocol.value;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(server_name.data(), port.get(), &hints, &result);
	///////////////////////////////////////////////////////////
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);

		return false;
	}
	///////////////////////////////////////////////////////////
	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
    ptr->ai_protocol);
	///////////////////////////////////////////////////////////
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());

		freeaddrinfo(result);
		return false;
	}
	///////////////////////////////////////////////////////////
	// Connect to server.
	iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	///////////////////////////////////////////////////////////
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");

		freeaddrinfo(result);
		return false;
	}
	///////////////////////////////////////////////////////////
	NetSocket *sock = new NetSocket(ConnectSocket, protocol, SocketType::Sock_Client);
	this->sockets.insert(str_id.empty() ? server_name : str_id, sock);
	printf_s("Connected to server.");

	return true;
}

//Создание сервера
bool NetAPI::CreateServer(CString server_name, //имя сервера
						  NetPort port, //порт
						  NetSocketProtocol protocol, //протокол
						  CString str_id //имя сокета (идентификатор)
						  )
{
	SOCKET ListenSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	int iResult;
	///////////////////////////////////////////////////////////
	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = protocol.value;
	hints.ai_flags = AI_PASSIVE;
	iResult = getaddrinfo(NULL, port.get(), &hints, &result);
	if (iResult != 0) {
		printf("Getaddrinfo failed: %d\n", iResult);

		this->StopSocketWork();
		return false;
	}
	///////////////////////////////////////////////////////////
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return false;
	}
	///////////////////////////////////////////////////////////
	// Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());

        freeaddrinfo(result);
        closesocket(ListenSocket);
        return false;
    }
	///////////////////////////////////////////////////////////
	if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
		printf( "Listen failed with error: %ld\n", WSAGetLastError() );
		closesocket(ListenSocket);

		return false;
	}
	///////////////////////////////////////////////////////////
	NetSocket *sock = new NetSocket(ListenSocket, protocol, SocketType::Sock_Server);
	this->sockets.insert(str_id.empty() ? server_name : str_id, sock);
	freeaddrinfo(result);
	return true;
}


//Останавливает сокет
void NetAPI::StopSocket(
	CString name //имя сокета (идентификатор)
	)
{
	Pair<CString, NetSocket *> *p = this->sockets.find(name);
	if (p == NULL) return;
	p->object->Close();
	delete (p->object); //освобождаем память
	this->sockets.erase(p); //удаляем из списка
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

//Обрабатывает события
void NetAPI::HandleEvents()
{
	if (this->sockets.empty()) 
		return;
	for (register size_t _pos = 0; _pos < this->sockets.size(); _pos++)
	{
		this->sockets.get(_pos)->object->HanldeEvents();
	}
}

//Отправляет данные
void NetAPI::SendData(CString name, CString &Data)
{
	if (this->sockets.empty())
		return;
	Pair<CString, NetSocket*> *p = this->sockets.find(name);
	if (p == NULL)
		return;
	p->object->SendData(Data);
}

//Получает данные
StringData *NetAPI::GetData(CString name)
{
	if (this->sockets.empty())
		return NULL;
	Pair<CString, NetSocket*> *p = this->sockets.find(name);
	if (p == NULL)
		return NULL;
	return p->object->GetData();
}

StringData * NetAPI::GetDataFromClient(CString name, size_t id)
{
	if (this->sockets.empty())
		return NULL;
	Pair<CString, NetSocket*> *p = this->sockets.find(name);
	if (p == NULL)
		return NULL;
	return p->object->GetData(id);
}

bool NetAPI::HasData(CString name)
{
	if (this->sockets.empty())
		return false;
	Pair<CString, NetSocket*> *p = this->sockets.find(name);
	if (p == NULL)
		return false;
	return p->object->HasData();
}

bool NetAPI::HasData(CString name, size_t ConnectId)
{
	if (this->sockets.empty())
		return false;
	Pair<CString, NetSocket*> *p = this->sockets.find(name);
	if (p == NULL)
		return false;
	return p->object->HasData(ConnectId);
}

ConnectionList NetAPI::GetConnectionList(CString server_name)
{
	if (sockets.empty())
		return ConnectionList();
	Pair<CString, NetSocket*> *p = sockets.find(server_name);
	if (p == NULL)
		return ConnectionList();
	return p->object->GetConnectionList();
}

NetAPI::~NetAPI()
{
	this->Release();
}