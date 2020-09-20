/* EA_InitW, Windows'da soket uygulamasý baþlatabilmek 
 * için windows soket api'den WSAStartupu çaðýrýr.
 */
void EA_InitW()
{
	#ifdef _WIN32
		
	WSADATA _wsdata;
	if (WSAStartup(MAKEWORD(2, 0), &_wsdata) != 0)
	{
		puts("WSAStartup failed.");
		exit(0);
	}
	
	#endif
}
/* EA_InitW, Windows'da soket uygulamasý bitirmek için 
 * için windows soket api'den WSACleanupu çaðýrýr.
 */
void EA_CleanW()
{
	#ifdef _WIN32
    
	WSACleanup();
	
    #endif
}
/* EA_Close, EA_SOKET tipinde olan soketi kapatýr.
 */
void EA_Close(EA_SOKET _EA_S)
{
	#ifdef _WIN32
	
	closesocket(_EA_S);
	
	#else
	
	close(_EA_S);
	
	#endif
}
/* EA_StoIP, EA_FDSET yapýsýndaki ilgili EA_SOKET 
 * tipindeki soketin IP'sini almak için kullanýlýr.
 * Eðer gönderilen soket yoksa 0 varsa 1 dönecektir.
 */
int EA_StoIP(EA_SOKET _A, EA_FDSET *_SET, char _I[16])
{
	int _j;
	
	for(_j = 0; (unsigned int)_j < (*_SET).fd_count; _j++)
		if((*_SET).fd_array[_j] == _A)
			break;
			
	if((unsigned int)_j != (*_SET).fd_count)
		strcpy(_I, (*_SET).ip[_j]);
	else
		return 0;
	
	return 1;
}
/* Recv gibidir.*/
int EA_Recv(EA_SOKET _S, char *_buf, int _EA_LEN)
{
	return recv(_S, _buf, _EA_LEN, 0);
}
/* Send gibidir.*/
int EA_Send(EA_SOKET S, const char *_buf, int _EA_LEN)
{
	return send(S, _buf, _EA_LEN, 0);
}
/*socket gibidir.*/
EA_SOKET EA_Socket(int _EA_FAMILY, int _EA_TYPE, int _EA_PROTOCOL)
{
	EA_SOKET _EA_SKT;
	
	if((_EA_SKT = socket(_EA_FAMILY, _EA_TYPE, _EA_PROTOCOL)) == (unsigned int)-1)
	{
		perror("socket");
		exit(0);
	}
	return _EA_SKT;
}
/*setsockopt gibidir.*/
void EA_Setsockopt(EA_SOKET _EA_SCKT, int _LEVEL, int _OPTNAME, const void *_OPTVAL, EA_LEN _OPTLEN)
{
	if (setsockopt(_EA_SCKT, _LEVEL, _OPTNAME, _OPTVAL, _OPTLEN) == -1)
	{
		perror("setsockopt");
		EA_Close(_EA_SCKT);
		exit(0);
	}
}
/*EA_SockaddrIn, family tipi portuyla addr yapýsýyla birleþtirir.*/
void EA_SockaddrIn(struct sockaddr_in *_EA_S_IN, short _S_FMLY, unsigned short _S_PORT, unsigned long _SS_ADDR)
{
	(*_EA_S_IN).sin_family = _S_FMLY;
	(*_EA_S_IN).sin_port = _S_PORT;
	(*_EA_S_IN).sin_addr.s_addr = _SS_ADDR;
	memset(&((*_EA_S_IN).sin_zero), '\0', 8);
}
/*Bind gibidir.*/
void EA_Bind(EA_SOKET _S, struct  sockaddr *_NAME)
{
	if(bind(_S, _NAME, sizeof(*_NAME)) == -1)
	{
		perror("bind");
		EA_Close(_S);
		exit(0);
	}
}
/*listen gibidir.*/
void EA_Listen(EA_SOKET _S, int _BACKLOG)
{
	if((listen(_S, _BACKLOG)) == -1)
	{
		perror("listen");
		EA_Close(_S);
		exit(0);
	}
}

void EA_Fdzero(EA_FDSET *_SET)
{
	(*_SET).maks = 0;
	(*_SET).fd_count = 0;
	memset(&((*_SET).ip), 0, __ELEMAN * 16);
	memset(&((*_SET).fd_array), 0, __ELEMAN);
}

void EA_Fdset(EA_SOKET _S, EA_FDSET *_SET)
{
	struct sockaddr_in _veri;
	int _sayi = sizeof(_veri);
	
	if(getpeername(_S, (struct  sockaddr *)&_veri, &_sayi) != -1)
		strcpy((*_SET).ip[(*_SET).fd_count], inet_ntoa(_veri.sin_addr));
	
	if ((*_SET).fd_count <= __ELEMAN)
	{
		(*_SET).fd_array[(*_SET).fd_count++] = _S;
		if(_S > (*_SET).maks)
			(*_SET).maks = _S;
	}
}

void EA_Fdclr(EA_SOKET _S, EA_FDSET *_SET)
{
	unsigned int _i;
	
	for(_i = 0; _i < (*_SET).fd_count; _i++)
	{
		if ((*_SET).fd_array[_i] == _S)
		{
			while (_i < (*_SET).fd_count - 1)
			{
				(*_SET).fd_array[_i] = (*_SET).fd_array[_i + 1];
				strcpy((*_SET).ip[_i], (*_SET).ip[_i + 1]);
				_i++;
			}
			(*_SET).fd_count--;
			break;
		}
	}
}

EA_SOKET EA_Select(EA_FDSET *_READ, EA_FDSET *_WRITE, EA_FDSET *_EXCEPT, struct timeval *_T_OUT)
{
	int _i = 0;/*, _j = 0*/
	EA_FDWL _tmpR, _tmpW, _tmpE;
	
	memset(&_tmpR, '\0', sizeof(_tmpR));
	memset(&_tmpW, '\0', sizeof(_tmpW));
	memset(&_tmpE, '\0', sizeof(_tmpE));
	
	#ifdef _WIN32
		if(_READ != NULL)
		{
			_tmpR.fd_count = (*_READ).fd_count;
			for(_i = 0; (unsigned int)_i < (*_READ).fd_count; _i++)
    			_tmpR.fd_array[_i] = (*_READ).fd_array[_i];
		}
		/*
		if(_WRITE != NULL)
		{
			_tmpW.fd_count = (*_WRITE).fd_count;
			for(_i = 0; _i < (*_WRITE).fd_count; _i++)
    			_tmpW.fd_array[_i] = (*_WRITE).fd_array[_i];
    	}
		*/
    	if(select(0, (fd_set *)(&_tmpR), NULL/*(fd_set *)(&_tmpW)*/, NULL/*(fd_set *)(&_tmpE)*/, _T_OUT) == -1)
		{
			perror("select");
			exit(0);
		}
		/*
		if(_EXCEPT != NULL)
		{
			(*_EXCEPT).fd_count = _tmpE.fd_count;
			(*_EXCEPT).fd_array[0] = _tmpE.fd_array[0];
    	}
    	*/
		return _tmpR.fd_array[0]/* + _tmpW.fd_array[0] + tmpE.fd_array[0]*/;
	#else
		if(_READ != NULL)
		{
			for(_i = 0; _i < (*_READ).fd_count; _i++)
           		FD_SET((*_READ).fd_array[_i], (fd_set *)_tmpR.fd_array);
        }
        /*
        if(_WRITE != NULL)
		{
        	for(_i = 0; _i < (*_WRITE).fd_count; _i++)
           		FD_SET((*_WRITE).fd_array[_i], (fd_set *)_tmpW.fd_array);
        }
        */
		if(select((*_READ).maks + 1, (fd_set *)_tmpR.fd_array, NULL /*(fd_set *)_tmpW.fd_array*/, NULL/*(fd_set *)_tmpE.fd_array*/, _T_OUT)==-1)
		{
			perror("select");
			exit(0);
		}
		
		if(_READ != NULL)
		{
				for(_i = 0; _i <= (*_READ).maks; _i++) 
           			if (FD_ISSET(_i, (fd_set *)_tmpR.fd_array))
           				break;
        		if(_i > (*_READ).maks)
        			_i = 0;
		}/*
        if(_WRITE != NULL)
		{
				for(_j = 0; _j <= (*_WRITE).maks; _j++) 
           			if (FD_ISSET(_j, (fd_set *)_tmpW.fd_array))
           				break;
        		if(_j>(*_WRITE).maks)
        			_j = 0;
		}*/
        return _i /*+ _j*/;
	#endif
}
/*accept gibidir.*/
EA_SOKET EA_Accept(EA_SOKET _S, struct  sockaddr *_ADDR)
{
	EA_SOKET _gelen;
	int _sayi = sizeof(*_ADDR);
	
	if((_gelen = accept(_S, _ADDR, &_sayi)) == (unsigned int)-1)
    {
		perror("accept");
		return 0;
	}
	return _gelen;
}
/*connect gibidir.*/
int EA_Connect(EA_SOKET _S, struct  sockaddr *_ADDR)
{
	return connect(_S, _ADDR, sizeof(*_ADDR));
}
