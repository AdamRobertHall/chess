#pragma once

// CChessSocket ÃüÁîÄ¿±ê

class CChessSocket : public CAsyncSocket
{
public:
	CChessSocket();
	virtual ~CChessSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


