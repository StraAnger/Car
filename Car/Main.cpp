#include <iostream>
#include <conio.h>
#include <thread>
#include <Windows.h>

using namespace std::chrono_literals;

#define Escape 27
#define Enter 13


#define MIN_TANK_VOLUME 20
#define MAX_TANK_VOLUME 80

#define MIN_ENGINE_CONSUMPTION 3
#define MAX_ENGINE_CONSUMPTION 25
#define DEFAULT_ENGINE_CONSUMPTION 10

#define MAX_SPEED_LOW_LIMIT  150
#define MAX_SPEED_HIGH_LIMIT 300
#define DEFAULT_MAX_SPEED    250



class Tank
{

	const unsigned int VOLUME; // ’арактеристика бака - его объЄм

	double fuel_level;

public:
	unsigned int get_VOLUME() const
	{

		return this->VOLUME;

	}

	double get_fuel_level() const
	{
		return fuel_level;

	}

	void fill(double fuel_level)
	{

		if (fuel_level <= 0) return;
		if (this->fuel_level + fuel_level <= VOLUME) this->fuel_level += fuel_level;
		else this->fuel_level = VOLUME;

	}

	double give_fuel(double amount)
	{
		if (fuel_level - amount > 0)fuel_level -= amount;
		else fuel_level = 0;
		return fuel_level;
	}


	Tank(unsigned int volume) :VOLUME(volume >= MIN_TANK_VOLUME && volume <= MAX_TANK_VOLUME ? volume : 60) // константу можем инициализировать только в заголовке
	{
		// конст. член класса то есть
		//if (volume >= MIN_TANK_VOLUME && volume <= MAX_TANK_VOLUME) this->VOLUME = volume;
		//else this->VOLUME = 60;
		this->fuel_level = 0;
		std::cout << "Tank is ready" << std::endl;
	}
	~Tank()
	{
		std::cout << "Tank is over" << std::endl;
	}

	void info() const
	{
		std::cout << "Tank volume: " << VOLUME << "liters\n";
		std::cout << "Fuel level: " << fuel_level << "liters\n";

	}

};


class Engine
{
	double consumption; //расход на 100 км
	double consumption_per_second; // расход за секунду
//	double default_consumption_per_second; 
	bool is_started; //двигатель заведЄн или заглушен
public:
	double get_consumption()const
	{
		return consumption;
	}
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}

	void set_consumption(double consumption)
	{
		if (consumption >= MIN_ENGINE_CONSUMPTION && consumption <= MAX_ENGINE_CONSUMPTION)
			this->consumption = consumption;
		else
			this->consumption = DEFAULT_ENGINE_CONSUMPTION;
		this->consumption_per_second = this->consumption * 3e-5;
//		this->default_consumption_per_second = this->consumption * 3e-5;
	}
	void start()
	{
		is_started = true;
	}

	void stop()
	{
		is_started = false;
	}

	bool started() const
	{
		return is_started;
	}

	void correct_consumption(int speed)
	{
		if (speed > 1 && speed <= 60 || speed >= 101 && speed <= 140)
			consumption_per_second = 0.002; //default_consumption_per_second * 7;
		if (speed >= 61 && speed <= 100)
			consumption_per_second = .0014; //default_consumption_per_second * 7;
		if (speed >= 141 && speed <= 200)
			consumption_per_second = .0025; //default_consumption_per_second * 7;
		if (speed >= 201 && speed <= 250)
			consumption_per_second = .003; //default_consumption_per_second * 7;
		if (speed == 0) consumption_per_second = 0.0003;
	}

	void info()const
	{
		std::cout << "Consumption per 100 km: " << consumption << " liters\n";
		std::cout << "Consumption per second " << consumption_per_second << " liters\n";
	}

	Engine(double consumption)
	{
		set_consumption(consumption);
		is_started = false;
		std::cout << "Engine is ready" << std::endl;
	}

	~Engine()
	{
		std::cout << "Engine is over " << std::endl;
	}

};

class Car
{
	Engine engine;
	Tank tank;
	bool driver_inside;
	int speed; // скорость
	const int MAX_SPEED; // максимальна€ скорость
	int acceleration; // ускорение

	struct Control
	{
		std::thread panel_thread;  //отображение панели приборов
		std::thread engine_idle_thread; //холостой ход двигател€
		std::thread free_wheeling_thread; //
	}control;

public:
	Car(double engine_consumption, unsigned int tank_volume, int max_speed = DEFAULT_MAX_SPEED, int acceleration = 10)
		:engine(engine_consumption), tank(tank_volume), 
		MAX_SPEED(max_speed>=MAX_SPEED_LOW_LIMIT&&max_speed<= MAX_SPEED_HIGH_LIMIT ? max_speed : 250)
	{
		driver_inside = false;
		speed = 0;
		//MAX_SPEED = max_speed;
		this->acceleration = acceleration >= 5 && acceleration <= 25 ? acceleration : 12;
		std::cout << "Your car is ready" << std::endl;
	}

	~Car()
	{
		std::cout << "Car is over" << std::endl;
	}

	void get_in()
	{
		driver_inside = true;
		//panel();
		control.panel_thread = std::thread(&Car::panel, this);
	}

	void get_out()
	{
		if (speed > 0)
		{
			std::cout << "Unable to get out of the car";
			return;
		}
		driver_inside = false;
		if (control.panel_thread.joinable()) control.panel_thread.join();
		system("CLS");
		std::cout << "You are out of your car" << std::endl;
	}

	void start_engine()
	{
		if (driver_inside && tank.get_fuel_level())
		{
			engine.start();
			control.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}

	}

	void stop_engine()
	{
		engine.stop();
		if (control.engine_idle_thread.joinable())control.engine_idle_thread.join();
		
	}

	void control_car()
	{
		std::cout << "Press Enter to get in or F to fill her in" << std::endl;
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case Enter:
				if (driver_inside)get_out();
				else get_in();
				break; // ¬ход выход из машины

			case 'i':
			case 'I':
				if (engine.started()) stop_engine();
				else start_engine();
				break;
			case 'f':
			case 'F':

				if (driver_inside)
				{
					std::cout << "To fill her up, you need to get out of the car" << std::endl;
				}
				else
				{
					double amount;
					std::cout << "Enter fuel level: "; std::cin >> amount;
					tank.fill(amount);
					std::cout << "Filled successfull ";
				}
				break;
			case 'W':case 'w':
				if (driver_inside && engine.started())
				{
				if (speed<MAX_SPEED) speed += acceleration;
				if (speed > MAX_SPEED) speed = MAX_SPEED;
					std::this_thread::sleep_for(1s);
					if (!control.free_wheeling_thread.joinable())
						control.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
					std::this_thread::sleep_for(1s);
				}
				
				break;
			case 'S':case 's':
				if (driver_inside && speed > 0)
				{
					speed -= acceleration;
					if (speed < 0)speed = 0;
					std::this_thread::sleep_for(1s);
				}
				break;
			case Escape:
				speed = 0;
				stop_engine();
				get_out();
				break;
			}
			
			if (speed==0&&control.free_wheeling_thread.joinable())
				control.free_wheeling_thread.join();

		} while (key != Escape);

	}

	void panel() const
	{
		while (driver_inside)
		{
			system("CLS");
			for (int i = 0; i < speed/3; i++)std::cout << "|";
			std::cout << std::endl;
			std::cout << "Speed\t" << speed << "km/h\n";
			std::cout << "Fuel level: " << tank.get_fuel_level() << " litters\t";
			if (tank.get_fuel_level() < 5)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 0xC7);
				std::cout << "LOW FUEL\n";
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			std::cout << "Engine is " << (engine.started() ? "started" : "stopped") << std::endl;
			if (engine.started()) std::cout << "Consumption per second: " << engine.get_consumption_per_second() << "1/s\n";
			std::this_thread::sleep_for(1s);
		}
	}

	void engine_idle()
	{
		while (engine.started() && tank.give_fuel(engine.get_consumption_per_second()))
		{

			std::this_thread::sleep_for(1s);
		}

	}

	void free_wheeling()
	{
		while (speed > 0)
		{
			speed--;
			if (speed < 0) speed = 0;
			std::this_thread::sleep_for(1s);
			engine.correct_consumption(speed);
		}
		//engine.correct_consumption(speed);
	}

	void correct_consumption()
	{


	}

	void info()const
	{
		engine.info();
		tank.info();
	}
};




//#define TANK_CHECK
//#define ENGINE_CHECK
#define CAR_CHECK

int main()
{
#ifdef TANK_CHECK
	Tank tank(40);
	tank.info();
	int fuel;
	while (true)
	{
		std::cout << "Enter fuel level: "; std::cin >> fuel;
		tank.fill(fuel);
		tank.info();
	}
#endif // TANK_CHECK



#ifdef ENGINE_CHECK
	Engine engine(10);
	engine.info();
#endif // ENGINE_CHECK

	Car bmw(12, 60);
	//	bmw.info();
	bmw.control_car();


	return 0;
}