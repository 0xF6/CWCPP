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
//HTTP - ��������� ��� �������� �������������
///////////////////////////////////////////////////////////////////////////////////

//��������� �������� �������
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
//������
///////////////////////////////////////////////////////////////////////////////////

//����� ������� ��������� - ��������� �������� � ��������
struct SocketWork {
	WSADATA wsaData; //������ �������
	bool ready; //������ �� ������ ��������
};

//�������� ������ (������������)
enum ESocketProtocol {
	ESP_TCP, ESP_UDP, ESP_ICMP
};

enum SocketType {
	Sock_Server, //������
	Sock_Client, //������
	Sock_Connection //���������� ������� � �������
};

//��������� ��������� ������
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


//��������� ������ - ���������� ���������, ���������� ����� � �������� � ���
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
	StringData ReceivedData; //���������� ������
	StringData DataToSend; //������ �� ��������
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
//����� NetAPI - ���������� �����
///////////////////////////////////////////////////////////////////////////////////

//API ��� ������� �����������
class NetAPI { 
	HINTERNET hSession; //������
	CMap<CWString, HINTERNET> hConnections; //�����������
	CMap<CString, NetSocket*> sockets;
	CArray<NetRequests> requests; //�������
	SocketWork socket_work;
	bool ready;
public:
	NetAPI();
	~NetAPI();
	void Release();
	void Init();
	///////////////////////////////////////////////////////////////////////////////////
	void OpenSession(); //�������� ������
	void CloseSession(); //�������� ������
	bool IsSessionOpen();
	bool Connect(CWString addr, INTERNET_PORT port); //�����������
	void Disconnect(CWString addr); //����������
	///////////////////////////////////////////////////////////////////////////////////
	size_t OpenRequest(CWString addr, NetMode mode, CWString data); //��������� ������
	void SendRequest(CWString addr, size_t req_num, CString *data); //�������� ������, �������� �����
	void SendRequest(CWString addr, size_t req_num, 
		CString &data, size_t *downloaded_size = NULL); //�������� ������, �������� �����
	void SendRequest(CWString addr, size_t req_num, CString req_data, 
		FileStream &data, size_t *downloaded_size = NULL); //�������� ������, �������� �����
	bool Check(CWString addr); //��������� ����������
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