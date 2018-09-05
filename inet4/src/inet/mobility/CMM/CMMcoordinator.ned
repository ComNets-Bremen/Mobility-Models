package inet.mobility.CMM;
simple CMMcoordinator{
    parameters:
        int numhost = default(0); //Number of members
        double threshold = default(0); //Threshold for conectivity
        double hrange = default(0); //gridLength
        double tupdate = default(0); //updateTime
        double tschedule = default(0); //reconfigurationTime
        double xmax = default(0); //Maximum area
        double ymax = default(0); //Maximum area
        string address = default("");
        int numMatrix = default(0);
        bool isRandom = default(true);
        @display("i=old/x_yellow;is=n");
}
