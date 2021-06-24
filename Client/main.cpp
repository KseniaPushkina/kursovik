#include <Client.h>
using namespace std;

int main (int argc, char **argv)
{
	try {
		string ID, Pass;
		cout << "Введите логин и пароль" << endl;
		cin >> ID;
		cin >> Pass;
		
		CheckClient obj;
		if (!obj.check_license(ID, Pass))
			cout << "Invalid license" << endl;
		else
			cout << "Valid license" << endl;
		
	} catch (const CCError & e) {
		cerr << "Ошибка: " << e.what() << endl;
	}
		
	return 0;
}