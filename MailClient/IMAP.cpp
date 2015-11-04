#include "stdafx.h"
#include "IMAP.h"


IMAP::IMAP(char *username, char *password, char *server)
{
	iUsername = username;
	iPassword = password;
	iServer = server;
	iServer = "imaps://" + iServer;
}

IMAP::~IMAP()
{
}


void IMAP::fetchEmail()
{
}
