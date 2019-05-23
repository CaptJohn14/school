###########################################
# Name: Johnson Le
# Desc: Firewall to allow only ARP and TCP.
#       Most of the code was provided. The
#       Modified part is do_firewall
###########################################


from pox.core import core
import pox.openflow.libopenflow_01 as of

log = core.getLogger()

class Firewall (object):
  """
  A Firewall object is created for each switch that connects.
  A Connection object for that switch is passed to the __init__ function.
  """
  def __init__ (self, connection):
    # Keep track of the connection to the switch so that we can
    # send it messages!
    self.connection = connection

    # This binds our PacketIn event listener
    connection.addListeners(self)

  def do_firewall (self, packet, packet_in):
    # The code in here will be executed for every packet.
    fm = of.ofp_flow_mod()
    fm.match = of.ofp_match.from_packet(packet)
    fm.idle_timeout = 25
    fm.hard_timeout = 50

    if (fm.match.dl_type == 0x0806) or (fm.match.dl_type == 0x0800 and fm.match.nw_proto == 6):  #check ARP first cause can be any ip
##      print("DL: {}   ||   PROTO: {}".format(fm.match.dl_type, fm.match.nw_proto))
##      print("hello")
      fm.actions.append(of.ofp_action_output(port = of.OFPP_ALL))
      self.connection.send(fm)
      
    else:
##      print("bleep bloop, not ARP or TCP")
      self.connection.send(fm)
      
  
  

  def _handle_PacketIn (self, event):
    """
    Handles packet in messages from the switch.
    """

    packet = event.parsed # This is the parsed packet data.
    if not packet.parsed:
      log.warning("Ignoring incomplete packet")
      return

    packet_in = event.ofp # The actual ofp_packet_in message.
    self.do_firewall(packet, packet_in)

def launch ():
  """
  Starts the component
  """
  def start_switch (event):
    log.debug("Controlling %s" % (event.connection,))
    Firewall(event.connection)
  core.openflow.addListenerByName("ConnectionUp", start_switch)
