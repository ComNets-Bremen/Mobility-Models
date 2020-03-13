//
// Copyright (C) 2005 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

package inet.examples.mobility.SOLAR;

import inet.mobility.SOLAR.SOLARcoordinator;


network SOLARNetwork
{
    parameters:
        int numUsers;
        @display("bgb=5000,5000");
    submodules:
        SOLARcoordinator: SOLARcoordinator {
            @display("p=148.592,48.191998;is=n;i=status/yellow");
        }
        user[numUsers]: UserHost {
            @display("p=594.368,616.456");
        }
}
