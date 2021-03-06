require 'sctp/socket'

# Adjust IP addresses as needed
begin
  port = 62324
  socket = SCTP::Socket.new
  socket.bind(:port => port, :addresses => ['10.0.5.5', '10.0.6.5'])
  socket.set_initmsg(:output_streams => 5, :input_streams => 5, :max_attempts => 4)
  socket.listen

  while true
    info = socket.recvmsg
    p info
  end
ensure
  socket.close
end
