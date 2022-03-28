#include <iostream>
#include <thread>

using namespace std::chrono_literals;

bool finish = false;

void Hello()
{
	while (!finish)
	{
		std::cout << "Hello";
		std::this_thread::sleep_for(1s);
	}
}

void World()
{
	while (!finish)
	{
		std::cout << "World";
		std::this_thread::sleep_for(1s);
	}
}


int main()
{
	// ������������� ������� ��� ����������:
	/*std::thread helloThread(Hello);   
	std::thread worldThread(World);*/

	//���������� �������
	std::thread helloThread;
	std::thread worldThread;

// ������������� ������� ����� ����������:	
	helloThread = std::thread(Hello);
	worldThread = std::thread(World);

	std::cin.get(); //������� ������� Enter
	finish = true;

	helloThread.join();
	worldThread.join();

	return 0;
}