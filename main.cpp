#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
//==========================================================================
uint64_t res{};
//-------------------------------------------------------------------------
void sum(const std::vector<int>& v, int p_s, int p_end) {
	for (size_t i = p_s; i <= p_end; i++) res += v[i];
}
//-------------------------------------------------------------------------------------------------------
void threads_run(const std::vector<int>& v,double& total, size_t thread_cnt=0) {
	size_t step{};
	if (!thread_cnt) {
		thread_cnt = v.size()-1;
		while (v.size() % thread_cnt || thread_cnt % 2)
			thread_cnt--;
	}
	step = (v.size() / thread_cnt);

	std::vector<std::thread> v_threads;
	for (size_t i = 0,j=0; i < v.size() && j< v.size(); i+= step){
		j = i + step-1;
		std::thread t([&v, i, j]() {
			for (size_t g = i; g <= j; g++) res += v[g];
			});
		v_threads.push_back(std::move(t));
	}

	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < v_threads.size(); i++){
		std::cout << "Started thread: " << i+1 << std::endl;
		v_threads[i].join();
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	total+= elapsed.count();


	std::cout << "Total sum is: " << res << std::endl;
}
//-------------------------------------------------------------------------
void create_random_int_vector(std::vector<int>& v,size_t sz = 20) {
	srand(time(nullptr));
	for (size_t i = 0; i < sz; i++){
		v.push_back(rand() % sz);
	}
}
//-------------------------------------------------------------------------
void show(const std::vector<int>& v) {
	for (const auto& i : v)
		std::cout << i << std::endl;

}
//-------------------------------------------------------------------------
int main(int argc, char** argv) {
	if (argc < 2) {
		std::string path{ argv[0] };
		auto const pos = path.find_last_of('\\');
		const auto leaf = path.substr(pos + 1);
		std::cout << "Usage: " << leaf << " [array size] [num threads]" << std::endl;
		return 0;
	}
	
	std::string s_sz{ argv[1] };
	std::string s_th{ argv[2] };

	double total_sec{ 0 };	
	size_t sz = std::stoll(s_sz);
	size_t th_cnt = std::stoll(s_th);
	std::vector<int> v1;//{ 5, 3, 2, 4, 8, 9, 6, 1, 1, 7, 9, 3, 8 ,2 ,6, 3, 5, 5, 4, 7 };

	create_random_int_vector(v1,sz);
	threads_run(v1, total_sec, th_cnt);
	std::cout << "Operation took " << total_sec << " sec.\n";

	return 0;

}