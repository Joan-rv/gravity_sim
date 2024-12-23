#include <gravity_sim.hpp>

int main() {
    GravitySim gravity_sim;

    while (gravity_sim.is_running()) {
        gravity_sim.loop();
    }
    return 0;
}
