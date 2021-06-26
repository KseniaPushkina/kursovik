#include <iostream>
#include <fstream>
#include <exception>
#include <unistd.h> //close()
#include <arpa/inet.h> //inet_aton()
#include <string.h>

using namespace std;

//Анализ ошибок
static void Error(const char* why,int exitcode)
{
	cerr << why << endl;
	exit(exitcode);
}

//Структура для пути к базе данных и порта на котором будет работать сервер
struct ServerConfig {
	string database_path = "Data.txt";//адрес Базы Данных
	int port = 44177;//адрес порта (число)
};

//Класс проверяющий номер по базе данных
class DB
{
private:
	string idnomer;
	string database_path;
public:
	DB(string database_path, const string IDPASS)
	{
		this->idnomer = IDPASS;
		this->database_path = database_path;
	}
	bool check()
	{
		ifstream file;
		file.open(database_path);
		if (!file.good()) {
			cerr<<"Ошибка открытия файла Data"<<endl;
			return false;
		}

		string ch;
		while (true) {
			file >> ch;//Читать строку из БД
			if (ch == idnomer) {//Сравнивать хеш с этой строкой
				return true;
			}
			if (file.eof()) {//Если конец файла, вернуть false
				return false;
			}
		}
	}
};



int main (int argc, char **argv)
{
	ServerConfig config;
	struct sockaddr_in addr;

	int err, ls;

	// Сокет
	if(-1 ==(ls = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP)))
		Error("create socket failed", -1);

	int rc;
	setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &rc, sizeof(rc));
	memset( &addr, 0, sizeof(addr) );

	addr.sin_family = AF_INET;
	addr.sin_port = htons(config.port);
	addr.sin_addr.s_addr = INADDR_ANY;

	// Привязка
	if (err = bind(ls, (struct sockaddr*) &addr, sizeof(addr)) < 0) {//привязать адрес к сокету
		close(ls);//закрыть сокет
		Error("bind failed", -1);
	}
	// Ожидание запросов
	if (listen(ls, 25) < 0) {
		close(ls);
		Error("listen failed", -1);
	}

	while (true) {
		rc = accept(ls, NULL, NULL);//Блокировка всех остальных запросов
		int pid = fork();
		if (pid < 0)
			cerr << "fork abort" << endl;
		if (pid == 0) {
			close( ls );
			char *remote_bufer = new char[256];//Буфер для записи Id и номера, отправленного сервером
			int rv = recv(rc,remote_bufer,256,0);//Принять Id, номер
				cerr << "recv failed" << endl;
			string str = string(remote_bufer);//Перевести отправленные Id и номер в строку
			DB x(config.database_path, str);//В качестве параметров класс с портом и адресом файла
			bool result = x.check();//Проверка лицензии
			unsigned char res_buf = result ? 1 : 0;
			rv=send(rc,&res_buf,sizeof(res_buf),0);//Отправить клиенту результат проверки
			if (rv < 0)
				cerr << "send failed" << endl;
			close(rc);//Закрыть сокет
			return -1;
		} else {
			close( rc );
		}
	}

	return 0;
}
