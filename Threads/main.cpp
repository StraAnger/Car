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
	// Инициализация потоков при объявлении:
	/*std::thread helloThread(Hello);   
	std::thread worldThread(World);*/

	//объявление потоков
	std::thread helloThread;
	std::thread worldThread;

// Инициализация потоков после объявления:	
	helloThread = std::thread(Hello);
	worldThread = std::thread(World);

	std::cin.get(); //Ожидает нажатие Enter
	finish = true;

	helloThread.join();
	worldThread.join();

	return 0;
}