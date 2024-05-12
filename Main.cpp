#include <iostream>
#include <string>
#include <iomanip>
#include <pqxx/pqxx>
#include <curl/curl.h>


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
	size_t totalSize = size * nmemb;
	buffer->append((char*)contents, totalSize);
	return totalSize;
}

int main() {
	// API code
	/*curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();
	CURLcode res;
	std::string responseBuffer;

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://skatteverket.entryscape.net/rowstore/dataset/70ccea31-b64c-4bf5-84c7-673f04f32505");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}
		else {
			std::cout << "Response received : " << std::endl;
			std::cout << responseBuffer << std::endl;
		}
		
		curl_easy_cleanup(curl);
	}
	else {
		std::cerr << "Failed to initialize cURL" << std::endl;
	}
	
	curl_global_cleanup();*/

	// Database code
	try {
		pqxx::connection conn("dbname=testone user=milo password=1Plommon \ hostaddr=127.0.0.1 port=5432");
	
		if (conn.is_open()) {
			std::cout << "Connected to PostgreSQL successfully\n";

			pqxx::work txn(conn);
			std::string sql = "INSERT INTO traktamente (land, belopp, year) VALUES ($1, $2, $3)";

			try {
				std::string land = "Albanien";
				std::string belopp = "230";
				std::string year = "2018";

				pqxx::result result = txn.exec_params(sql, land, belopp, year);

				txn.commit();
				std::cout << "Data inserted into database." << std::endl;
			}
			catch (const std::exception& e) {
				std::cerr << "Failed to insert data : " << e.what() << std::endl;
				txn.abort();
			}

			

		}
		else {
			std::cerr << "Failed to connect to PostgreSQL\n";
			return 1;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}



	return 0;
}