
int Sender(EA_SOKET _S, const char *buf)
{
	char _len = -1; /*255 olur.*/
	int _de, _fe = 0;
	
	_de = EA_Send(_S, &_len, 1);

	if(_de == -1 || _de == 0)
		return _de;
	
	while((unsigned char)_len > _fe)
	{
		_de = EA_Send(_S, buf + _fe, (unsigned char)_len - _fe);

		if(_de == -1 || _de == 0)
			return _de;
		_fe = _de + _fe;
	}
	
	return _de;
}

int Recver(EA_SOKET _S, char *buf)
{
	char _len;
	int _de, _fe = 0;

	_de = EA_Recv(_S, &_len, 1);
	
	if(_de == -1 || _de == 0)
		return _de;

	while((unsigned char)_len > _fe)
	{
		_de = EA_Recv(_S, buf + _fe, (unsigned char)_len - _fe);

		if(_de == -1 || _de == 0)
			return _de;
		_fe = _de + _fe;
	}

	return _de;
}
