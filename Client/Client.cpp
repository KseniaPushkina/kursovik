#include <../Client/Client.h>

// Получение хеша sha256
string CheckClient::get_sha256(string mess)
{
	// Обработка ошибок
	if(mess.empty())
		throw CCError("Empty string");
	for(auto s : mess)
		if(s < 0 || s > 127)
			throw CCError("Invalid character");
	
	string hash;
	using CryptoPP::SHA256;
	CryptoPP::SHA256 sha256;
	CryptoPP::StringSource(mess, true, new CryptoPP::HashFilter(sha256, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
	
	return hash;
}

bool CheckClient::check_license(string id, string pass)
{
	// Обработка ошибок
	if(id.empty() || pass.empty())
		throw CCError("Empty string");
	for(auto s : id)
		if(s < 49 || s > 57)
			throw CCError("Invalid character");
	
	if(pass.length() < 3)
		throw CCError("Short password");
	for(auto s : pass)
		if(s < 0 || s > 127)
			throw CCError("Invalid character");
	
	
	string idpass = id + "/" + pass;//Составление пароля
	idpass = get_sha256(idpass);
	
	//Запись хеша пароля и логина в буфер для отправки серверу
	size_t size = idpass.size();//размер строки
	char self_bufer[256] = {0};
	for(size_t i = 0; i < size; i++) {
		self_bufer[i] = idpass[i];
	}

	//Создание и заполнение структуры с адресом
	struct sockaddr_in serv_addr;
	serv_addr.sin_port = htons(this->port); //Перевод порта в сетевую форму
	serv_addr.sin_family = AF_INET; //Семейство

	// Сокет
	int sock = 0;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)//При ошибки
		throw CCError("Socket creation error");
	
	// Преобразование адресов IPv4  из текста в двоичную форму
	if (inet_pton(AF_INET, this->address.c_str(), &serv_addr.sin_addr) <= 0)
		throw CCError("Invalid address/ Address not supported");

	// Соединение
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		throw CCError("Connection Failed");

	//Отправка серверу хеша от пароля и id
	send(sock, self_bufer, sizeof(self_bufer), 0); 

	//Получение ответа
	unsigned char answer;
	int valread = read(sock, &answer, sizeof(answer));
	return (bool)answer;
}
