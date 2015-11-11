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


bool IMAP::login()
{
	bool result = false;
	CURLcode res = CURLE_OK;

	struct MessageFile msgfile = {
		"imap-Message.txt", // name to store the file as if successful
		NULL
	};

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_USERNAME, iUsername.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, iPassword.c_str());

		curl_easy_setopt(curl, CURLOPT_URL, iServer.c_str());

		// Define our callback to get called when there's data to be written
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &msgfile);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/2.0");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

		// Turn on debug information
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		// Perform connection attempt
		res = curl_easy_perform(curl);

		// Check for errors
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		else
		{
			printf("%s\n", msgfile.rawMsg.c_str());

			if (msgfile.rawMsg.find("failed") == -1)
			{
				printf("Success!\n");
				result = true;
			}
		}

		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return result;
}

char *IMAP::fetchEmail(int id)
{
	bool result = false;
	CURLcode res = CURLE_OK;
	std::string messageFile = "mailbox\\msg";
	messageFile += std::to_string(id) + ".txt";

	struct MessageFile msgfile = {
		messageFile.c_str(),
		NULL
	};

	if (fileExist(messageFile))
	{
		return loadFromFile(id);
	}
	else
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();

		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_USERNAME, iUsername.c_str());
			curl_easy_setopt(curl, CURLOPT_PASSWORD, iPassword.c_str());

			curl_easy_setopt(curl, CURLOPT_URL, (iServer + "/INBOX/;UID=" + std::to_string(id)).c_str());

			// Define our callback to get called when there's data to be written
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &msgfile);

			curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/2.0");
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

			// Turn on debug information
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

			// Perform connection attempt
			res = curl_easy_perform(curl);

			// Check for errors
			if (res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
			else
			{
				printf("%s\n", msgfile.rawMsg.c_str());
			}

			curl_easy_cleanup(curl);
		}

		curl_global_cleanup();

		return (char *)msgfile.rawMsg.c_str();
	}
}

void IMAP::fetchPage(int num)
{
}

char *IMAP::loadFromFile(int mailID)
{
	FILE *msg;
	long lSize;
	char *buffer;
	std::string messageFile = "mailbox\\msg";
	messageFile += std::to_string(mailID) + ".txt";

	if (msg = fopen(messageFile.c_str(), "rb"))
	{
		// obtain file size:
		fseek(msg, 0, SEEK_END);
		lSize = ftell(msg);
		rewind(msg);

		buffer = (char*)malloc(sizeof(char)*lSize);
		if (buffer == NULL) 
		{ 
			printf("Memory error\n");
			return nullptr;
		}

		if (fread(buffer, 1, lSize, msg) != lSize)
		{
			printf("Reading error\n");
			return nullptr;
		}
		fclose(msg);

		return buffer;
	}
	else
		return nullptr;
}

void IMAP::updateSaved()
{
}

char * IMAP::base64Decode(char * message)
{
	return nullptr;
}

void IMAP::processMail(char * rawData)
{
}



size_t IMAP::my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct MessageFile *out = (struct MessageFile *)stream;
	if (out && !out->stream)
	{
		/* open file for writing */
		out->stream = fopen(out->filename, "wb");
		out->rawMsg += (char *)buffer;

		if (!out->stream)
			return -1; /* failure, can't open file to write */
	}

	return fwrite(buffer, size, nmemb, out->stream);
}

inline bool IMAP::fileExist(const std::string& name) 
{
	if (FILE *file = fopen(name.c_str(), "r")) 
	{
		fclose(file);

		return true;
	}
	else {
		return false;
	}
}