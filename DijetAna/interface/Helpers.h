#include <vector>

template<typename T>
class Logspace {
private:
    T curValue, base;

public:
    Logspace(T first, T base) : curValue(first), base(base) {}

    T operator()() {
        T retval = curValue;
        curValue *= base;
        return retval;
    }
};

std::vector<double> logspace(double start, double stop, int num = 50, double base = 10) {
    double realStart = pow(base, start);
    double realBase = pow(base, (stop-start)/num);

    std::vector<double> retval;
    retval.reserve(num);
    std::generate_n(std::back_inserter(retval), num, Logspace<double>(realStart,realBase));
    return retval;
}

std::vector<double> linspace(double start, double end, double num)
{
  double delta = (end - start) / (num - 1);

  std::vector<double> linspaced(num);
  for(int i=0; i < num; ++i)
    {
      linspaced[i] = start + delta * i;
    }
  linspaced.push_back(end);
  return linspaced;
}
