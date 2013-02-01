// config DAM
//    create_transport( "/dev/ttyS0", 9600, 500 ); //    host, port, timeaut
    create_transport( "10.0.10.46", 4002, 550 ); //    host, port, timeaut
//  --
// Устройства в цикле из БД или конфиг-файла:
    max_device = 2;	//  !!!
//  --
    Device dev[max_device];
    init_device( &dev[0], "EL_TM5132", 11, 11 );
    init_device( &dev[1], "EL_TM5132", 12, 14 );
//  --
    time_wait = 1;
//    err = 0;
//  ---------------
