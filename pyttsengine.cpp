#include "pyttsengine.h"

PyTtsEngine::PyTtsEngine()
{

}

int addNumbers(int i, int j)
{
    return i + j;
}

PYBIND11_MODULE(example, m)
{
    m.def("add_numbers", &addNumbers, "basic addition");
}
