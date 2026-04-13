#include "Logger.h"
#include <unistd.h>
u64 program_epoch_ns;

int main(int argc, char** argv) {
    program_epoch_ns = get_current_ns();
}
