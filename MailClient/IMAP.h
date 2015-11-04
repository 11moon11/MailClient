#pragma once
class IMAP
{
public:
	IMAP(char *username, char *password, char *server);
	~IMAP();

	bool login();

	void fetchEmail();
	void fetchPage(int num);

	void loadFromFile(int mailID);
	void updateSaved();

	char *base64Decode(char *message);

	void processMail(char *rawData /* Pointer to special structure (With Body, From, To, Theme, Read/Unread, etc...) here */);


private:
	std::string iUsername;
	std::string iPassword;
	std::string iServer;
};

