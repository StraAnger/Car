#include <iostream>
#include <conio.h>


#define Escape 27
#define Enter 13
#define F_key 70

#define MIN_TANK_VOLUME 20
#define MAX_TANK_VOLUME 80

#define MIN_ENGINE_CONSUMPTION 3
#define MAX_ENGINE_CONSUMPTION 25

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
			this->consumption = 10;
		this->consumption_per_second = this->consumption * 3e-5;
	}
	void start()
	{
		is_started = true;
	}
	bool started() const
	{
		return is_started;
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
public:
	Car(double engine_consumption, unsigned int tank_volume)
		:engine(engine_consumption), tank(tank_volume)
	{
		driver_inside = false;
		std::cout << "Your car is ready" << std::endl;
	}

	~Car()
	{
		std::cout << "Car is over" << std::endl;
	}

	void get_in()
	{
		driver_inside = true;
		panel();
	}

	void get_out()
	{
		driver_inside = false;
		system("CLS");
		std::cout << "You are out of your car" << std::endl;
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
			case F_key:
				fillHerUp();
				break; 
			}
		} while (key != Escape);

	}

	void panel() const
	{
		system("CLS");
		std::cout << "Fuel level: " << tank.get_fuel_level() << " litters\n";
		std::cout << "Engine is " << (engine.started() ? "started" : "stopped") << std::endl;
	}

	void fillHerUp()
	{
		int fuel;
				std::cout << "Enter fuel level: "; std::cin >> fuel;
				tank.fill(fuel);
				tank.info();
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