#pragma once

#ifndef NetExclude
#include "CString.h"
#include "CArray.h"
#include "CMap.h"
#include "File.h"
enum NetMode {
	GET, POST, HEAD, PUT
};

struct NetPort {
private:
	CString port;
public:
	NetPort(CString port);
	NetPort(unsigned int port);
	bool operator==(CString port);
	bool operator==(unsigned int port);
	PCSTR get();
};

CWString GetMode(NetMode mode);

///////////////////////////////////////////////////////////////////////////////////
//HTTP - Структуры для урощения использования
///////////////////////////////////////////////////////////////////////////////////

//Структура интернет запроса
struct NetRequests {
private:
	CArray<HINTERNET> hRequests;
	CWString addr;
public:
	explicit NetRequests(CWString addr);
	~NetRequests();
	CWString GetAddr();
	void add(HINTERNET hReq);
	HINTERNET get(size_t num);
	void remove(size_t num);
	void clear();
	size_t count();
	HINTERNET operator[](size_t num);
	bool operator==(CWString addr);
	bool operator!=(CWString addr);
};

///////////////////////////////////////////////////////////////////////////////////
//Сокеты
///////////////////////////////////////////////////////////////////////////////////

//Самая главная структура - позволяет работать с сокетами
struct SocketWork {
	WSADATA wsaData; //Данные сокетов
	bool ready; //готовы ли сокеты работать
};

//Протокол сокета (перечисление)
enum ESocketProtocol {
	ESP_TCP, ESP_UDP, ESP_ICMP
};

enum SocketType {
	Sock_Server, //сервер
	Sock_Client, //клиент
	Sock_Connection //подлючение клиента к серверу
};

//Структура протокола сокета
struct NetSocketProtocol {
	NetSocketProtocol(ESocketProtocol protocol);
	NetSocketProtocol(IPPROTO protocol);
	NetSocketProtocol(NetSocketProtocol &protocol);
	ESocketProtocol value;
	IPPROTO operator=(NetSocketProtocol &protocol);
};

struct ConnectionList {
	CArray<size_t> list;
};

struct StringData {
private:
	CArray<CString> data;
public:
	void Add(CString data);
	CString Get();
	bool Empty();
};


//Структура сокета - спецальная структура, содержащая сокет и работает с ним
struct NetSocket {
private:
	SOCKET sock;
	NetSocketProtocol protocol;
	WSAEVENT Event;
	SocketType type;
	bool ready;
	bool closed;
	CMap<size_t, NetSocket*> connections;
	size_t last_connect_id;
	StringData ReceivedData; //Полученные данные
	StringData DataToSend; //Данные на отправку
	CMap<size_t, StringData> ReceivedData_Connections;
	CMap<size_t, StringData> DataToSend_Connections;
	void CreateConnectionSocket();
	void Init();
	void Destroy();
public:
	NetSocket(SOCKET sock, NetSocketProtocol protocol, SocketType type);
	~NetSocket();
	void Release();
	bool IsReady();
	void HanldeEvents();
	void Close();
	void SendData(CString &Data);
	void SendData(CString &Data, size_t ConnectId);
	StringData *GetData();
	StringData *GetData(size_t ConnectId);
	bool IsClosed();
	bool HasData();
	bool HasData(size_t ConnectId);
	ConnectionList GetConnectionList();
};

///////////////////////////////////////////////////////////////////////////////////
//Класс NetAPI - управление сетью
///////////////////////////////////////////////////////////////////////////////////

//API для сетевых подключений
class NetAPI { 
	HINTERNET hSession; //Сессия
	CMap<CWString, HINTERNET> hConnections; //Подключения
	CMap<CString, NetSocket*> sockets;
	CArray<NetRequests> requests; //Запросы
	SocketWork socket_work;
	bool ready;
public:
	NetAPI();
	~NetAPI();
	void Release();
	void Init();
	///////////////////////////////////////////////////////////////////////////////////
	void OpenSession(); //открытие сессии
	void CloseSession(); //закрытие сессии
	bool IsSessionOpen();
	bool Connect(CWString addr, INTERNET_PORT port); //подключение
	void Disconnect(CWString addr); //отключение
	///////////////////////////////////////////////////////////////////////////////////
	size_t OpenRequest(CWString addr, NetMode mode, CWString data); //открывает запрос
	void SendRequest(CWString addr, size_t req_num, CString *data); //посылает запрос, получает ответ
	void SendRequest(CWString addr, size_t req_num, 
		CString &data, size_t *downloaded_size = NULL); //посылает запрос, получает ответ
	void SendRequest(CWString addr, size_t req_num, CString req_data, 
		FileStream &data, size_t *downloaded_size = NULL); //посылает запрос, получает ответ
	bool Check(CWString addr); //проверяет соединение
	///////////////////////////////////////////////////////////////////////////////////
	void StartSocketWork();
	void StopSocketWork();
	bool ConnectToServer(CString server_name, NetPort port, NetSocketProtocol, CString str_id = "");
	bool CreateServer(CString server_name, NetPort port, NetSocketProtocol, CString str_id = "");
	void StopSocket(CString name);
	void HandleEvents();
	///////////////////////////////////////////////////////////////////////////////////
	void SendData(CString name, CString &Data);
	StringData *GetData(CString name);
	bool HasData(CString name);
	///////////////////////////////////////////////////////////////////////////////////
	void SendDataToClient(CString name, size_t id, CString &Data);
	StringData * GetDataFromClient(CString name, size_t id);
	ConnectionList GetConnectionList(CString server_name);
	bool HasData(CString name, size_t ConnectId);
};

#endif