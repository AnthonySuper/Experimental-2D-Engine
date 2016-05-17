#include "vector.hpp"

namespace NM {
    const struct mrb_data_type Vector::mrb_type =  {"Vector", &NM::mrb::destructor_value<Vector>};
}
