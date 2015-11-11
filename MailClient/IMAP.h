#pragma once
class IMAP
{
public:
	IMAP(char *username, char *password, char *server);
	~IMAP();

	bool login();

	char *fetchEmail(int id);
	void fetchPage(int num);

	char *loadFromFile(int mailID);
	void updateSaved();

	char *base64Decode(char *message);

	void processMail(char *rawData /* Pointer to special structure (With Body, From, To, Theme, Read/Unread, etc...) here */);


private:
	std::string iUsername;
	std::string iPassword;
	std::string iServer;

	CURL *curl;

	struct MessageFile {
		const char *filename;
		FILE *stream;
		std::string rawMsg;
	};

	static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
	inline bool fileExist(const std::string& name);
};

