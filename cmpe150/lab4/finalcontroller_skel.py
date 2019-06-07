# Final Skeleton
#
# Hints/Reminders from Lab 3:
#
# To check the source and destination of an IP packet, you can use
# the header information... For example:
#
# ip_header = packet.find('ipv4')
#
# if ip_header.srcip == "1.1.1.1":
#   print "Packet is from 1.1.1.1"
#
# Important Note: the "is" comparison DOES NOT work for IP address
# comparisons in this way. You must use ==.
#
# To send an OpenFlow Message telling a switch to send packets out a
# port, do the following, replacing <PORT> with the port number the
# switch should send the packets out:
#
#    msg = of.ofp_flow_mod()
#    msg.match = of.ofp_match.from_packet(packet)
#    msg.idle_timeout = 30
#    msg.hard_timeout = 30
#
#    msg.actions.append(of.ofp_action_output(port = <PORT>))
#    msg.data = packet_in
#    self.connection.send(msg)
#
# To drop packets, simply omit the action.
#

from pox.core import core
import pox.openflow.libopenflow_01 as of

log = core.getLogger()


class Final (object):
    """
    A Firewall object is created for each switch that connects.
    A Connection object for that switch is passed to the __init__ function.
    """

    def __init__(self, connection):
        # Keep track of the connection to the switch so that we can
        # send it messages!
        self.connection = connection

        # This binds our PacketIn event listener
        connection.addListeners(self)

    def do_final(self, packet, packet_in, port_on_switch, switch_id):
        # This is where you'll put your code. The following modifications have
        # been made from Lab 3:
        #   - port_on_switch: represents the port that the packet was received on.
        #   - switch_id represents the id of the switch that received the packet.
        #      (for example, s1 would have switch_id == 1, s2 would have switch_id == 2, etc...)
        # You should use these to determine where a packet came from. To figure out where a packet
        # is going, you can use the IP header information.

        ip_header = packet.find('ipv4')
        icmp_header = packet.find('icmp')

        if ip_header:
##            print("port: {}   ||   sw_id: {}   ||   srcip: {}   ||   dstip: {}".format(port_on_switch, switch_id, ip_header.srcip, ip_header.dstip))
##            if ip_header.srcip == "123.45.67.89" and (icmp_header or ip_header.dstip == "10.5.5.50"):
##                print("BLOCKED")
##                self.send_packet(packet, packet_in, of.OFPP_FLOOD)
            if ip_header.srcip == "123.45.67.89":
                if ip_header.dstip == "10.5.5.50":
                    print("BLOCKED: srcip: {} --> dstip: {}".format(ip_header.srcip, ip_header.dstip))
                elif icmp_header:
                    print("BLOCKED: srcip: {} --> dstip: {}  is ICMP".format(ip_header.srcip, ip_header.dstip))
                else:
                    self.do_helper(ip_header.dstip, switch_id, packet, packet_in)
            else:
                self.do_helper(ip_header.dstip, switch_id, packet, packet_in)
        else:
            self.send_packet(packet, packet_in, of.OFPP_FLOOD)

    def do_helper(self, dstip, switch_id, packet, packet_in):
        port_SWequalH= 1
        port_GoToSW4 = 2
        port_SW4toSW1 = 3
        port_SW4toSW2 = 4
        port_SW4toSW3 = 5
        port_SW4toSW5 = 6
        if dstip == "123.45.67.89":
            if switch_id == 4:
                self.send_packet(packet, packet_in, port_SWequalH)
            else:
                self.send_packet(packet, packet_in, port_GoToSW4)
        elif dstip == "10.1.1.10":
            if switch_id == 1:
                self.send_packet(packet, packet_in, port_SWequalH)
            elif switch_id == 4:
                self.send_packet(packet, packet_in, port_SW4toSW1)
            else:
                self.send_packet(packet, packet_in, port_GoToSW4)
        elif dstip == "10.2.2.20":
            if switch_id == 2:
                self.send_packet(packet, packet_in, port_SWequalH)
            elif switch_id == 4:
                self.send_packet(packet, packet_in, port_SW4toSW2)
            else:
                self.send_packet(packet, packet_in, port_GoToSW4)
        elif dstip == "10.3.3.30":
            if switch_id == 3:
                self.send_packet(packet, packet_in, port_SWequalH)
            elif switch_id == 4:
                self.send_packet(packet, packet_in, port_SW4toSW3)
            else:
                self.send_packet(packet, packet_in, port_GoToSW4)
        elif dstip == "10.5.5.50":
            if switch_id == 5:
                self.send_packet(packet, packet_in, port_SWequalH)
            elif switch_id == 4:
                self.send_packet(packet, packet_in, port_SW4toSW5)
            else:
                self.send_packet(packet, packet_in, port_GoToSW4)

    def send_packet(self, packet, packet_in, port):
        msg = of.ofp_flow_mod()
        msg.match = of.ofp_match.from_packet(packet)
        msg.actions.append(of.ofp_action_output(port = port))
        msg.data = packet_in
        self.connection.send(msg)

    def _handle_PacketIn(self, event):
        """
        Handles packet in messages from the switch.
        """
        packet = event.parsed  # This is the parsed packet data.
        if not packet.parsed:
            log.warning("Ignoring incomplete packet")
            return

        packet_in = event.ofp  # The actual ofp_packet_in message.
        self.do_final(packet, packet_in, event.port, event.dpid)


def launch():
    """
    Starts the component
    """
    def start_switch(event):
        log.debug("Controlling %s" % (event.connection,))
        Final(event.connection)
    core.openflow.addListenerByName("ConnectionUp", start_switch)
