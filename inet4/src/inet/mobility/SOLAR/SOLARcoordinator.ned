package inet.mobility.SOLAR;
simple SOLARcoordinator{
    parameters:
        int numhost = default(0); //Number of members
        int numplaces = default(0); //Number of groups
        double hrange = default(0); //Range for internal position
        double tschedule = default(0); //Update Groups
        double xmax = default(0); //Maximum area
        double ymax = default(0); //Maximum area
        string positionsAddress = default("");
        string probabilitiesAddress = default("");
        int numMatrix = default(0);
        bool isPositionRandom = default(true);
        bool isProbabilityRandom = default(true);        
        @display("i=old/x_yellow;is=n");
}
