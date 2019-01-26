#include "frameworks.h"
#include "engine_data.h"
#include "subsystems.h"
#include "application_interface.h"


int main( int argc, const char* argv[] )
{
    init_frameworks();
    init_engine_data( argc, argv );
    init_subsystems();
    init_application();

    while ( not engine_data->quit_requested )
    {
        handle_sys_events();
    //     update_application_logic();
    //     render< draw_application_view >();
        time_sync();
    }

    close_application();
    close_subsystems();
    close_engine_data();
    close_frameworks();

    return 0;
}
