#include <string>
#include <iostream>

#include "../includes/light_pool.hpp"

// Exemples of allocated values, it doesn't need to rely on it. (Check your system specs for safetiness)
#define EX_MAX_THREADS 36
#define EX_MAX_TASKS 36

void sayHello() {
	std::cout << "Hello!" << std::endl;
}

void sayGoodbye() {
	std::cout << "Goodbye!" << std::endl;
}

int main() {
	// creating pool instance
	LP::LightPool l_pool;

	// registering EX_MAX_THREADS threads in pool
	for (int i = 0; i < EX_MAX_THREADS; i++) {
		l_pool.registerThread(new LP::LightThread(l_pool, std::to_string(i)));
	}

	// registering EX_MAX_TASKS tasks in pool -> if i multiple of 2 then register "sayHello" task if not register "sayGoodbye" task
	for (int i = 0; i < EX_MAX_TASKS; i++) {
		if (i % 2) {
			l_pool.registerTask(LP::Task { []() { sayGoodbye(); }});
		} else {
			l_pool.registerTask(LP::Task{ []() { sayHello(); } });
		}

		// sync execution (no data races) ex: std::cout in our case
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// waits for all the tasks to be executed by threads
	while (!l_pool.getTasks().empty());

	// stopping pools, tells threads to stop looking for tasks
	l_pool.stop();
	return 1;
}